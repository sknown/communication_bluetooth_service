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

#ifndef REMOTE_OBSERVER_LIST_H
#define REMOTE_OBSERVER_LIST_H

#include "bluetooth_log.h"
#include <functional>
#include <map>
#include <mutex>

#include "refbase.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"

namespace OHOS {
namespace Bluetooth {
const std::string HOST_SERVER_OBSERVER = "hostServer";
const std::string HOST_SERVER_REMOTE_DEVICE_OBSERVER = "hostServerRemoteDevice";
const std::string HOST_SERVER_BLE_ADAPTER_OBSERVER = "hostServerBleAdapter";
const std::string HOST_SERVER_BLE_PERIPHERAL_CALLBACK_OBSERVER = "hostServerBlePeripheralCallback";

template <typename T, typename Y>
class RemoteObserverList2 final {
public:
    RemoteObserverList2() = default;
    ~RemoteObserverList2();

    bool Register(const sptr<T> &observer, Y* btServer, const std::string profileName);
    bool Deregister(const sptr<T> &observer);

    void ForEach(const std::function<void(sptr<T>)> &observer);

    class ObserverDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        ObserverDeathRecipient(const sptr<T> &observer, RemoteObserverList2 *owner);

        sptr<T> GetObserver() const
        {
            return observer_;
        };

        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;

    private:
        sptr<T> observer_{};
        RemoteObserverList2<T, Y> *owner_{};
    };

    using ObserverMap = std::map<sptr<T>, sptr<ObserverDeathRecipient>>;
    std::mutex lock_{};
    ObserverMap observers_{};
    struct btServerInfo {
        std::string observerName;
        Y* serverPtr;
    };

    using BtServerMap = std::map<sptr<T>, struct btServerInfo>;
    BtServerMap btServers_{};

    RemoteObserverList2(const RemoteObserverList2 &) = delete;
    RemoteObserverList2 &operator=(const RemoteObserverList2 &) = delete;

private:
    bool UnregisterInternal(typename ObserverMap::iterator iter);
};

template <typename T, typename Y>
RemoteObserverList2<T, Y>::~RemoteObserverList2()
{
    HILOGI("RemoteObserverList2<T>::~RemoteObserverList2() called");
    std::lock_guard<std::mutex> lock(lock_);
    for (auto it = observers_.begin(); it != observers_.end(); ++it) {
        sptr<ObserverDeathRecipient> dr = it->second;
        if (!dr->GetObserver()->AsObject()->RemoveDeathRecipient(dr)) {
            HILOGE("Failed to unlink death recipient from observer");
        }
    }
    observers_.clear();
}

template <typename T, typename Y>
bool RemoteObserverList2<T, Y>::Register(const sptr<T> &observer, Y* pBtServer, std::string observerName)
{
    HILOGI("RemoteObserverList2<T>::Register called");
    std::lock_guard<std::mutex> lock(lock_);
    bool isMatch = false;
    for (const auto &it : observers_) {
        if (it.first != nullptr && it.first->AsObject() == observer->AsObject()) {
            HILOGW("Observer list already contains given observer");
            isMatch = true;
            break;
        }
    }
    if (!isMatch) {
        sptr<ObserverDeathRecipient> dr(new ObserverDeathRecipient(observer, this));
        if (!observer->AsObject()->AddDeathRecipient(dr)) {
            HILOGE("Failed to link death recipient to observer");
            return false;
        }
        observers_[observer] = dr;
    }

    bool isExist = false;
    for (const auto &it : btServers_) {
        if (it.first != nullptr && it.first->AsObject() == observer->AsObject()) {
            HILOGW("btServers_ list already contains given observer");
            isExist = true;
            break;
        }
    }
    if (!isExist) {
        struct btServerInfo tmp = {observerName, pBtServer};
        btServers_[observer] = tmp;
    }

    return true;
}

template <typename T, typename Y>
bool RemoteObserverList2<T, Y>::Deregister(const sptr<T> &observer)
{
    HILOGI("RemoteObserverList2<T>::Deregister called");
    //std::lock_guard<std::mutex> lock(lock_);
    for (auto it = observers_.begin(); it != observers_.end();) {
        if (it->first != nullptr && it->first->AsObject() == observer->AsObject()) {
            UnregisterInternal(it++);
        } else {
            it++;
        }
    }

    HILOGW("Given observer not registered with this list");
    return false;
}

template <typename T, typename Y>
void RemoteObserverList2<T, Y>::ForEach(const std::function<void(sptr<T>)> &observer)
{
    std::lock_guard<std::mutex> lock(lock_);
    for (const auto &it : observers_) {
        if (it.first != nullptr) {
            observer(it.first);
        }
    }
}

template <typename T, typename Y>
RemoteObserverList2<T, Y>::ObserverDeathRecipient::ObserverDeathRecipient(
    const sptr<T> &observer, RemoteObserverList2<T, Y> *owner)
    : observer_(observer), owner_(owner)
{
    HILOGI("RemoteObserverList2<T>::ObserverDeathRecipient::ObserverDeathRecipient called");
}

template <typename T, typename Y>
void RemoteObserverList2<T, Y>::ObserverDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &object)
{
    // Remove the observer but no need to call unlinkToDeath.
    std::lock_guard<std::mutex> lock(owner_->lock_);
    HILOGI("zhangsz OnRemoteDied");
    for (auto it = owner_->btServers_.begin(); it != owner_->btServers_.end();) {
        if (it->first != nullptr && it->first->AsObject() == object) {
            if (it->second.observerName == "hostServer") {
                it->second.serverPtr->DeregisterObserver(object);
            } else if (it->second.observerName == "hostServerRemoteDevice") {
                it->second.serverPtr->DeregisterRemoteDeviceObserver(object);
            } else if (it->second.observerName == "hostServerBleAdapter") {
                it->second.serverPtr->DeregisterBleAdapterObserver(object);
            } else if (it->second.observerName == "hostServerBlePeripheralCallback") {
                it->second.serverPtr->DeregisterBlePeripheralCallback(object);
            }
            it = owner_->btServers_.erase(it);
        } else {
            it++;
        }
    }

    HILOGI("Callback from dead process unregistered");
}

template <typename T, typename Y>
bool RemoteObserverList2<T, Y>::UnregisterInternal(typename ObserverMap::iterator iter)
{
    HILOGI("RemoteObserverList2<T>::UnregisterInternal called");
    sptr<ObserverDeathRecipient> dr = iter->second;

    if (!dr->GetObserver()->AsObject()->RemoveDeathRecipient(dr)) {
        HILOGE("Failed to unlink death recipient from observer");
        //return false;
    }

    observers_.erase(iter);
    return true;
}

///////////////////////////////////////////////////////////////

template <typename T>
class RemoteObserverList final {
public:
    RemoteObserverList() = default;
    ~RemoteObserverList();

    bool Register(const sptr<T> &observer);
    bool Deregister(const sptr<T> &observer);

    void ForEach(const std::function<void(sptr<T>)> &observer);

    class ObserverDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        ObserverDeathRecipient(const sptr<T> &observer, RemoteObserverList *owner);

        sptr<T> GetObserver() const
        {
            return observer_;
        };

        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;

    private:
        sptr<T> observer_{};
        RemoteObserverList<T> *owner_{};
    };

    using ObserverMap = std::map<sptr<T>, sptr<ObserverDeathRecipient>>;
    std::mutex lock_{};
    ObserverMap observers_{};

    RemoteObserverList(const RemoteObserverList &) = delete;
    RemoteObserverList &operator=(const RemoteObserverList &) = delete;

private:
    bool UnregisterInternal(typename ObserverMap::iterator iter);
};

template <typename T>
RemoteObserverList<T>::~RemoteObserverList()
{
    HILOGI("RemoteObserverList<T>::~RemoteObserverList() called");
    std::lock_guard<std::mutex> lock(lock_);
    for (auto it = observers_.begin(); it != observers_.end(); ++it) {
        sptr<ObserverDeathRecipient> dr = it->second;
        if (!dr->GetObserver()->AsObject()->RemoveDeathRecipient(dr)) {
            HILOGE("Failed to unlink death recipient from observer");
        }
    }
    observers_.clear();
}

template <typename T>
bool RemoteObserverList<T>::Register(const sptr<T> &observer)
{
    HILOGI("RemoteObserverList<T>::Register called");
    std::lock_guard<std::mutex> lock(lock_);

    for (const auto &it : observers_) {
        if (it.first != nullptr && it.first->AsObject() == observer->AsObject()) {
            HILOGW("Observer list already contains given observer");
            return false;
        }
    }

    sptr<ObserverDeathRecipient> dr(new ObserverDeathRecipient(observer, this));

    if (!observer->AsObject()->AddDeathRecipient(dr)) {
        HILOGE("Failed to link death recipient to observer");
        return false;
    }

    observers_[observer] = dr;

    return true;
}

template <typename T>
bool RemoteObserverList<T>::Deregister(const sptr<T> &observer)
{
    HILOGI("RemoteObserverList<T>::Deregister called");
    std::lock_guard<std::mutex> lock(lock_);

    for (auto it = observers_.begin(); it != observers_.end(); ++it) {
        if (it->first != nullptr && it->first->AsObject() == observer->AsObject()) {
            return UnregisterInternal(it);
        }
    }
    HILOGW("Given observer not registered with this list");
    return false;
}

template <typename T>
void RemoteObserverList<T>::ForEach(const std::function<void(sptr<T>)> &observer)
{
    std::lock_guard<std::mutex> lock(lock_);
    for (const auto &it : observers_) {
        if (it.first != nullptr) {
            observer(it.first);
        }
    }
}



template <typename T>
RemoteObserverList<T>::ObserverDeathRecipient::ObserverDeathRecipient(
    const sptr<T> &observer, RemoteObserverList<T> *owner)
    : observer_(observer), owner_(owner)
{
    HILOGI("RemoteObserverList<T>::ObserverDeathRecipient::ObserverDeathRecipient called");
}

template <typename T>
void RemoteObserverList<T>::ObserverDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &object)
{
    // Remove the observer but no need to call unlinkToDeath.
    std::lock_guard<std::mutex> lock(owner_->lock_);

    for (auto it = owner_->observers_.begin(); it != owner_->observers_.end(); ++it) {
        if (it->first != nullptr && it->first->AsObject() == object) {
            if (!it->first->AsObject()->RemoveDeathRecipient(it->second)) {
                HILOGE("Failed to unlink death recipient from observer");
            }
            owner_->observers_.erase(it);
            break;
        }
    }
    HILOGI("Callback from dead process unregistered");
}

template <typename T>
bool RemoteObserverList<T>::UnregisterInternal(typename ObserverMap::iterator iter)
{
    HILOGI("RemoteObserverList<T>::UnregisterInternal called");
    sptr<ObserverDeathRecipient> dr = iter->second;
    
    if (!dr->GetObserver()->AsObject()->RemoveDeathRecipient(dr)) {
        HILOGE("Failed to unlink death recipient from observer");
        return false;
    }
    observers_.erase(iter);

    return true;
}

}  // namespace Bluetooth
}  // namespace OHOS

#endif  // REMOTE_OBSERVER_LIST_H
