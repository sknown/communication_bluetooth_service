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

    void RegisterBleCentralManagerCallback(const sptr<IBluetoothBleCentralManagerCallback> &callback) override;
    void DeregisterBleCentralManagerCallback(
        const sptr<IBluetoothBleCentralManagerCallback> &callback) override;
    int StartScan() override;
    int StartScan(const BluetoothBleScanSettings &settings) override;
    int StopScan() override;
    int ConfigScanFilter(int &clientId, const std::vector<BluetoothBleScanFilter> &filters) override;
    void RemoveScanFilter(const int clientId) override;
    bool ProxyUid(int32_t uid, bool isProxy) override;
    bool ResetAllProxy() override;
    static bool IsProxyUid(int32_t uid);
    int SetBurstParam(int duration, int maxExtAdvEvents, int burstWindow, int burstInterval, int advHandle) override;
    int SetScanReportChannelToSensorHub(const int clientId, const int isToAp)override;
    int StartScanInShSync() override;
    int StopScanInShSync() override;
    int SendParamsToSensorhub(const std::vector<uint8_t> &dataValue, int32_t type) override;
    bool IsSupportSensorAdvertiseFilter() override;
    int SetAdvFilterParam(const BluetoothBleFilterParamSet &paramSet) override;
    int RemoveAdvFilter(const bluetooth::Uuid &uuid) override;

private:
    BLUETOOTH_DECLARE_IMPL();
    BLUETOOTH_DISALLOW_COPY_AND_ASSIGN(BluetoothBleCentralManagerServer);
    static std::mutex proxyMutex_;
    static std::set<int32_t> proxyUids_;
    void SetScanParams(const BluetoothBleScanSettings &settings);
    void SetWindowAndInterval(const int mode, uint16_t &window, uint16_t &interval);
    bool IsNewScanParams();
    bool IsAllStop();
};
}  // namespace Bluetooth
}  // namespace OHOS
#endif  // OHOS_BLUETOOTH_STANDARD_BLE_CENTRAL_MANAGER_SERVER_H