/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OHOS_BLUETOOTH_STANDARD_BLE_CENTRAL_MANAGER_SERVER_H
#define OHOS_BLUETOOTH_STANDARD_BLE_CENTRAL_MANAGER_SERVER_H

#include <cstdint>
#include <mutex>
#include <set>

#include "bluetooth_ble_central_manager_stub.h"
#include "bluetooth_types.h"
#include "i_bluetooth_ble_central_manager.h"
#include "if_system_ability_manager.h"
#include "iservice_registry.h"
#include "system_ability.h"

namespace OHOS {
namespace Bluetooth {
class BluetoothBleCentralManagerServer : public BluetoothBleCentralManagerStub {
public:
    BluetoothBleCentralManagerServer();
    ~BluetoothBleCentralManagerServer() override;

    void RegisterBleCentralManagerCallback(int32_t &scannerId, bool enableRandomAddrMode,
        const sptr<IBluetoothBleCentralManagerCallback> &callback) override;
    void DeregisterBleCentralManagerCallback(int32_t scannerId,
        const sptr<IBluetoothBleCentralManagerCallback> &callback) override;
    int StartScan(int32_t scannerId, const BluetoothBleScanSettings &settings,
        const std::vector<BluetoothBleScanFilter> &filters) override;
    int StopScan(int32_t scannerId) override;
    void RemoveScanFilter(int32_t scannerId) override;
    bool FreezeByRss(int32_t uid, bool isProxy) override;
    bool ResetAllProxy() override;
    static bool IsProxyUid(int32_t uid);
    int SetLpDeviceAdvParam(int duration, int maxExtAdvEvents, int window, int interval, int advHandle) override;
    int SetScanReportChannelToLpDevice(int32_t scannerId, bool enable) override;
    int EnableSyncDataToLpDevice() override;
    int DisableSyncDataToLpDevice() override;
    int SendParamsToLpDevice(const std::vector<uint8_t> &dataValue, int32_t type) override;
    bool IsLpDeviceAvailable() override;
    int SetLpDeviceParam(const BluetoothLpDeviceParamSet &paramSet) override;
    int RemoveLpDeviceParam(const bluetooth::Uuid &uuid) override;
    void DeregisterBleCentralManagerCallbackInner(const sptr<IBluetoothBleCentralManagerCallback> &callback,
        int32_t scannerId);
private:
    BLUETOOTH_DECLARE_IMPL();
    BLUETOOTH_DISALLOW_COPY_AND_ASSIGN(BluetoothBleCentralManagerServer);
    static std::mutex proxyMutex_;
    static std::set<int32_t> proxyUids_;
    void SetScanParams(const BluetoothBleScanSettings &settings);
    void SetWindowAndInterval(const int mode, uint16_t &window, uint16_t &interval);
    bool IsNewScanParams();
    bool IsAllStop();
    int ConfigScanFilterInner(int32_t scannerId, const std::vector<BluetoothBleScanFilter> &filters);
};
}  // namespace Bluetooth
}  // namespace OHOS
#endif  // OHOS_BLUETOOTH_STANDARD_BLE_CENTRAL_MANAGER_SERVER_H
