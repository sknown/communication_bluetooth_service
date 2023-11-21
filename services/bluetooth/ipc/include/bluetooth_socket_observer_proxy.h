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

#ifndef OHOS_BLUETOOTH_STANDARD_SOCKET_OBSERVER_PROXY_H
#define OHOS_BLUETOOTH_STANDARD_SOCKET_OBSERVER_PROXY_H

#include "i_bluetooth_socket_observer.h"
#include "iremote_proxy.h"
#include "bluetooth_raw_address.h"
#include "bluetooth_bt_uuid.h"

namespace OHOS {
namespace Bluetooth {
class BluetoothClientSocketObserverProxy : public IRemoteProxy<IBluetoothClientSocketObserver> {
public:
    explicit BluetoothClientSocketObserverProxy(const sptr<IRemoteObject> &impl)
        : IRemoteProxy<IBluetoothClientSocketObserver>(impl) {}
    ~BluetoothClientSocketObserverProxy() {}

    void OnConnectionStateChanged(const BluetoothRawAddress &dev, bluetooth::Uuid uuid,
        int status, int result) override;

private:
    static inline BrokerDelegator<BluetoothClientSocketObserverProxy> delegator_;
};

class BluetoothServerSocketObserverProxy : public IRemoteProxy<IBluetoothServerSocketObserver> {
public:
    explicit BluetoothServerSocketObserverProxy(const sptr<IRemoteObject> &impl)
        : IRemoteProxy<IBluetoothServerSocketObserver>(impl) {}
    ~BluetoothServerSocketObserverProxy() {}

private:
    static inline BrokerDelegator<BluetoothServerSocketObserverProxy> delegator_;
};
}  // namespace Bluetooth
}  // namespace OHOS
#endif  // OHOS_BLUETOOTH_STANDARD_HOST_OBSERVER_PROXY_H
