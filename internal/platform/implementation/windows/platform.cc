// Copyright 2021-2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "internal/platform/implementation/platform.h"

// clang-format off
#include <windows.h>
#include <winver.h>
#include <PathCch.h>
#include <knownfolders.h>
#include <psapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
// clang-format on

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>  // NOLINT
#include <memory>
#include <sstream>
#include <string>

#include "absl/base/attributes.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "internal/base/files.h"
#include "internal/platform/implementation/atomic_boolean.h"
#include "internal/platform/implementation/atomic_reference.h"
#include "internal/platform/implementation/awdl.h"
#include "internal/platform/implementation/ble.h"
#include "internal/platform/implementation/ble_v2.h"
#include "internal/platform/implementation/bluetooth_adapter.h"
#include "internal/platform/implementation/bluetooth_classic.h"
#include "internal/platform/implementation/condition_variable.h"
#include "internal/platform/implementation/count_down_latch.h"
#include "internal/platform/implementation/credential_storage.h"
#include "internal/platform/implementation/http_loader.h"
#include "internal/platform/implementation/input_file.h"
#include "internal/platform/implementation/mutex.h"
#include "internal/platform/implementation/output_file.h"
#include "internal/platform/implementation/scheduled_executor.h"
#include "internal/platform/implementation/server_sync.h"
#include "internal/platform/implementation/shared/count_down_latch.h"
#include "internal/platform/implementation/submittable_executor.h"
#include "internal/platform/implementation/wifi.h"
#include "internal/platform/implementation/wifi_lan.h"
#include "internal/platform/implementation/windows/atomic_boolean.h"
#include "internal/platform/implementation/windows/atomic_reference.h"
#include "internal/platform/implementation/windows/ble_medium.h"
#include "internal/platform/implementation/windows/ble_v2.h"
#include "internal/platform/implementation/windows/bluetooth_adapter.h"
#include "internal/platform/implementation/windows/bluetooth_classic_medium.h"
#include "internal/platform/implementation/windows/condition_variable.h"
#include "internal/platform/implementation/windows/device_info.h"
#include "internal/platform/implementation/windows/file.h"
#include "internal/platform/implementation/windows/file_path.h"
#include "internal/platform/implementation/windows/http_loader.h"
#include "internal/platform/implementation/windows/mutex.h"
#include "internal/platform/implementation/windows/preferences_manager.h"
#include "internal/platform/implementation/windows/scheduled_executor.h"
#include "internal/platform/implementation/windows/server_sync.h"
#include "internal/platform/implementation/windows/string_utils.h"
#include "internal/platform/implementation/windows/submittable_executor.h"
#include "internal/platform/implementation/windows/timer.h"
#include "internal/platform/implementation/windows/utils.h"
#include "internal/platform/implementation/windows/wifi.h"
#include "internal/platform/implementation/windows/wifi_hotspot.h"
#include "internal/platform/implementation/windows/wifi_lan.h"
#include "internal/platform/logging.h"
#include "internal/platform/os_name.h"
#include "internal/platform/payload_id.h"

namespace nearby {
namespace api {

namespace {

constexpr char kNCRelativePath[] = "Google\\Nearby\\Connections";

std::string GetApplicationName(DWORD pid) {
  HANDLE handle =
      OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE,
                  pid);  // Modify pid to the pid of your application
  if (!handle) {
    return "";
  }

  std::string szProcessName("", MAX_PATH);
  DWORD len = MAX_PATH;

  if (NULL != handle) {
    GetModuleFileNameExA(handle, nullptr, szProcessName.data(), len);
  }

  szProcessName.resize(szProcessName.find_first_of('\0') + 1);

  auto just_the_file_name_and_ext = szProcessName.substr(
      szProcessName.find_last_of('\\') + 1,
      szProcessName.length() - szProcessName.find_last_of('\\') + 1);

  return just_the_file_name_and_ext.substr(
      0, just_the_file_name_and_ext.find_last_of('.'));
}

}  // namespace

std::string ImplementationPlatform::GetCustomSavePath(
    const std::string& parent_folder, const std::string& file_name) {
  auto parent = windows::string_utils::StringToWideString(parent_folder);
  auto file = windows::string_utils::StringToWideString(file_name);

  return windows::string_utils::WideStringToString(
      windows::FilePath::GetCustomSavePath(parent, file));
}

std::string ImplementationPlatform::GetDownloadPath(
    const std::string& parent_folder, const std::string& file_name) {
  auto parent = windows::string_utils::StringToWideString(parent_folder);
  auto file = windows::string_utils::StringToWideString(file_name);

  return windows::string_utils::WideStringToString(
      windows::FilePath::GetDownloadPath(parent, file));
}

std::string ImplementationPlatform::GetDownloadPath(
    const std::string& file_name) {
  std::wstring fake_parent_path;
  auto file = windows::string_utils::StringToWideString(file_name);

  return windows::string_utils::WideStringToString(
      windows::FilePath::GetDownloadPath(fake_parent_path, file));
}

std::string ImplementationPlatform::GetAppDataPath(
    const std::string& file_name) {
  PWSTR basePath;

  // Retrieves the full path of a known folder identified by the folder's
  // KNOWNFOLDERID.
  // https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
  SHGetKnownFolderPath(
      FOLDERID_LocalAppData,  //  rfid: A reference to the KNOWNFOLDERID that
                              //  identifies the folder.
      0,           // dwFlags: Flags that specify special retrieval options.
      nullptr,     // hToken: An access token that represents a particular user.
      &basePath);  // ppszPath: When this method returns, contains the address
                   // of a pointer to a null-terminated Unicode string that
                   // specifies the path of the known folder. The calling
                   // process is responsible for freeing this resource once it
                   // is no longer needed by calling CoTaskMemFree, whether
                   // SHGetKnownFolderPath succeeds or not.
  size_t bufferSize;
  wcstombs_s(&bufferSize, nullptr, 0, basePath, 0);
  std::string fullpathUTF8(bufferSize - 1, '\0');
  wcstombs_s(&bufferSize, fullpathUTF8.data(), bufferSize, basePath, _TRUNCATE);
  CoTaskMemFree(basePath);

  // Check if our folder exists
  std::replace(fullpathUTF8.begin(), fullpathUTF8.end(), '\\', '/');

  std::stringstream path("");

  path << fullpathUTF8.c_str() << "/" << kNCRelativePath << "/"
       << file_name.data();

  return path.str();
}

OSName ImplementationPlatform::GetCurrentOS() { return OSName::kWindows; }

std::unique_ptr<AtomicBoolean> ImplementationPlatform::CreateAtomicBoolean(
    bool initial_value) {
  return std::make_unique<windows::AtomicBoolean>(initial_value);
}

std::unique_ptr<AtomicUint32> ImplementationPlatform::CreateAtomicUint32(
    std::uint32_t value) {
  return std::make_unique<windows::AtomicUint32>(value);
}

std::unique_ptr<CountDownLatch> ImplementationPlatform::CreateCountDownLatch(
    std::int32_t count) {
  return std::make_unique<shared::CountDownLatch>(count);
}

#pragma push_macro("CreateMutex")
#undef CreateMutex
std::unique_ptr<Mutex> ImplementationPlatform::CreateMutex(Mutex::Mode mode) {
  return std::make_unique<windows::Mutex>(mode);
}
#pragma pop_macro("CreateMutex")

std::unique_ptr<ConditionVariable>
ImplementationPlatform::CreateConditionVariable(Mutex* mutex) {
  return std::make_unique<windows::ConditionVariable>(mutex);
}

ABSL_DEPRECATED("This interface will be deleted in the near future.")
std::unique_ptr<InputFile> ImplementationPlatform::CreateInputFile(
    PayloadId payload_id, std::int64_t total_size) {
  std::string file_name(std::to_string(payload_id));
  return windows::IOFile::CreateInputFile(GetDownloadPath(file_name),
                                          total_size);
}

std::unique_ptr<InputFile> ImplementationPlatform::CreateInputFile(
    const std::string& file_path, size_t size) {
  return windows::IOFile::CreateInputFile(file_path, size);
}

ABSL_DEPRECATED("This interface will be deleted in the near future.")
std::unique_ptr<OutputFile> ImplementationPlatform::CreateOutputFile(
    PayloadId payload_id) {
  std::string parent_folder("");
  std::string file_name(std::to_string(payload_id));
  return windows::IOFile::CreateOutputFile(
      GetDownloadPath(parent_folder, file_name));
}

std::unique_ptr<OutputFile> ImplementationPlatform::CreateOutputFile(
    const std::string& file_path) {
  std::filesystem::path path = std::filesystem::u8path(file_path);
  std::filesystem::path folder_path = path.parent_path();
  // Verifies that a path is a valid directory.
  if (!sharing::DirectoryExists(folder_path)) {
    if (!sharing::CreateDirectories(folder_path)) {
      LOG(ERROR) << "Failed to create directory: "
                 << windows::string_utils::WideStringToString(
                        folder_path.wstring());
      return nullptr;
    }
  }
  return windows::IOFile::CreateOutputFile(file_path);
}

std::unique_ptr<SubmittableExecutor>
ImplementationPlatform::CreateSingleThreadExecutor() {
  return std::make_unique<windows::SubmittableExecutor>();
}

std::unique_ptr<SubmittableExecutor>
ImplementationPlatform::CreateMultiThreadExecutor(
    std::int32_t max_concurrency) {
  return std::make_unique<windows::SubmittableExecutor>(max_concurrency);
}

std::unique_ptr<ScheduledExecutor>
ImplementationPlatform::CreateScheduledExecutor() {
  return std::make_unique<windows::ScheduledExecutor>();
}

std::unique_ptr<BluetoothAdapter>
ImplementationPlatform::CreateBluetoothAdapter() {
  return std::make_unique<windows::BluetoothAdapter>();
}

std::unique_ptr<BluetoothClassicMedium>
ImplementationPlatform::CreateBluetoothClassicMedium(
    nearby::api::BluetoothAdapter& adapter) {
  return std::make_unique<windows::BluetoothClassicMedium>(adapter);
}

std::unique_ptr<BleMedium> ImplementationPlatform::CreateBleMedium(
    BluetoothAdapter& adapter) {
  return std::make_unique<windows::BleMedium>(adapter);
}

// TODO(b/184975123): replace with real implementation.
std::unique_ptr<api::ble_v2::BleMedium>
ImplementationPlatform::CreateBleV2Medium(api::BluetoothAdapter& adapter) {
  return std::make_unique<windows::BleV2Medium>(adapter);
}

std::unique_ptr<api::CredentialStorage>
ImplementationPlatform::CreateCredentialStorage() {
  return nullptr;
}

// TODO(b/184975123): replace with real implementation.
std::unique_ptr<ServerSyncMedium>
ImplementationPlatform::CreateServerSyncMedium() {
  return std::unique_ptr<windows::ServerSyncMedium>();
}

// TODO(b/184975123): replace with real implementation.
std::unique_ptr<WifiMedium> ImplementationPlatform::CreateWifiMedium() {
  return std::make_unique<windows::WifiMedium>();
}

std::unique_ptr<WifiLanMedium> ImplementationPlatform::CreateWifiLanMedium() {
  return std::make_unique<windows::WifiLanMedium>();
}

std::unique_ptr<AwdlMedium> ImplementationPlatform::CreateAwdlMedium() {
  return nullptr;
}

std::unique_ptr<WifiHotspotMedium>
ImplementationPlatform::CreateWifiHotspotMedium() {
  return std::make_unique<windows::WifiHotspotMedium>();
}

std::unique_ptr<WifiDirectMedium>
ImplementationPlatform::CreateWifiDirectMedium() {
  return nullptr;
}

// TODO(b/261663238) replace with real implementation.
std::unique_ptr<WebRtcMedium> ImplementationPlatform::CreateWebRtcMedium() {
  return nullptr;
}

absl::StatusOr<WebResponse> ImplementationPlatform::SendRequest(
    const WebRequest& request) {
  windows::HttpLoader http_loader{request};
  return http_loader.GetResponse();
}

std::unique_ptr<Timer> ImplementationPlatform::CreateTimer() {
  return std::make_unique<windows::Timer>();
}

std::unique_ptr<DeviceInfo> ImplementationPlatform::CreateDeviceInfo() {
  return std::make_unique<windows::DeviceInfo>();
}

std::unique_ptr<nearby::api::PreferencesManager>
ImplementationPlatform::CreatePreferencesManager(absl::string_view path) {
  return std::make_unique<windows::PreferencesManager>(path);
}

}  // namespace api
}  // namespace nearby
