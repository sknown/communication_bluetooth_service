/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_BLUETOOTH_STANDARD_PBAP_PSE_SERVER_H
#define OHOS_BLUETOOTH_STANDARD_PBAP_PSE_SERVER_H

#include "bluetooth_pbap_pse_stub.h"
#include "bluetooth_types.h"
#include "i_bluetooth_pbap_pse.h"
#include "if_system_ability_manager.h"
#include "iservice_registry.h"
#include "system_ability.h"

namespace OHOS {
namespace Bluetooth {
class BluetoothPbapPseServer : public BluetoothPbapPseStub {
public:
    explicit BluetoothPbapPseServer();
    ~BluetoothPbapPseServer() override;

    void RegisterObserver(const sptr<IBluetoothPbapPseObserver> &observer) override;
    void DeregisterObserver(const sptr<IBluetoothPbapPseObserver> &observer) override;
    int32_t GetDevicesByStates(const std::vector<int32_t> &states,
        std::vector<BluetoothRawAddress>& rawDevices) override;
    int32_t GetDeviceState(const BluetoothRawAddress &device, int32_t &state) override;
    int32_t Disconnect(const BluetoothRawAddress &device) override;
    int32_t SetConnectionStrategy(const BluetoothRawAddress &device, int32_t strategy) override;
    int32_t GetConnectionStrategy(const BluetoothRawAddress &device, int32_t &strategy) override;
    int32_t SetShareType(const BluetoothRawAddress &device, int32_t shareType) override;
    int32_t GetShareType(const BluetoothRawAddress &device, int32_t &shareType) override;
    int32_t SetPhoneBookAccessAuthorization(const BluetoothRawAddress &device, int32_t accessAuthorization) override;
    int32_t GetPhoneBookAccessAuthorization(const BluetoothRawAddress &device, int32_t &accessAuthorization) override;

private:
    BLUETOOTH_DISALLOW_COPY_AND_ASSIGN(BluetoothPbapPseServer);
    BLUETOOTH_DECLARE_IMPL();
};
}  // namespace Bluetooth
}  // namespace OHOS
#endif  // OHOS_BLUETOOTH_STANDARD_PBAP_PSE_SERVER_H