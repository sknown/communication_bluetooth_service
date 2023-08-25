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

namespace OHOS {
namespace Bluetooth {
class BluetoothSocketObserverProxy : public IRemoteProxy<IBluetoothSocketObserver> {
public:
    explicit BluetoothSocketObserverProxy(const sptr<IRemoteObject> &impl)
        : IRemoteProxy<IBluetoothSocketObserver>(impl) {}
    ~BluetoothSocketObserverProxy() {}

private:
    static inline BrokerDelegator<BluetoothSocketObserverProxy> delegator_;
};
}  // namespace Bluetooth
}  // namespace OHOS
#endif  // OHOS_BLUETOOTH_STANDARD_HOST_OBSERVER_PROXY_H
