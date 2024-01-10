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

#include "bluetooth_def.h"
#include "bluetooth_errorcode.h"
#include "bluetooth_hitrace.h"
#include "bluetooth_log.h"
#include "bluetooth_utils_server.h"
#include "hisysevent.h"
#include "interface_profile_manager.h"
#include "interface_profile_pbap_pse.h"
#include "remote_observer_list.h"
#include "interface_adapter_manager.h"
#include "permission_utils.h"
#include "bluetooth_pbap_pse_server.h"

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
class PbapPseObserver : public IPbapPseObserver {
public:
    PbapPseObserver() = default;
    ~PbapPseObserver() override = default;

    void OnConnectionStateChanged(const RawAddress &device, int32_t state) override
    {
        HILOGI("addr: %{public}s, state: %{public}d", GET_ENCRYPT_ADDR(device), state);
        observers_->ForEach([device, state](sptr<IBluetoothPbapPseObserver> observer) {
            observer->OnConnectionStateChanged(device, state);
        });
    }

    void SetObserver(RemoteObserverList<IBluetoothPbapPseObserver> *observers)
    {
        observers_ = observers;
    }

private:
    RemoteObserverList<IBluetoothPbapPseObserver> *observers_;
};

struct BluetoothPbapPseServer::impl {
    impl();
    ~impl();

    class SystemStateObserver;
    std::unique_ptr<SystemStateObserver> systemStateObserver_ = nullptr;

    RemoteObserverList<IBluetoothPbapPseObserver> observers_;
    std::unique_ptr<PbapPseObserver> observerImp_{std::make_unique<PbapPseObserver>()};
    IProfilePbapPse *pbapPseService_ = nullptr;
};

class BluetoothPbapPseServer::impl::SystemStateObserver : public ISystemStateObserver {
public:
    SystemStateObserver(BluetoothPbapPseServer::impl *pimpl) : pimpl_(pimpl) {};
    ~SystemStateObserver() override = default;

    void OnSystemStateChange(const BTSystemState state) override
    {
        CHECK_AND_RETURN_LOG((pimpl_ != nullptr), "pimpl_ is null");
        IProfileManager *serviceMgr = IProfileManager::GetInstance();
        CHECK_AND_RETURN_LOG((serviceMgr != nullptr), "serviceMgr is null");

        switch (state) {
            case BTSystemState::ON:
                pimpl_->pbapPseService_ =
                    (IProfilePbapPse *)serviceMgr->GetProfileService(PROFILE_NAME_PBAP_PSE);
                CHECK_AND_RETURN_LOG((pimpl_->pbapPseService_ != nullptr), "pbapPseService_ is null");
                pimpl_->pbapPseService_->RegisterObserver(pimpl_->observerImp_.get());
                break;
            case BTSystemState::OFF:
                pimpl_->pbapPseService_ = nullptr;
                break;
            default:
                break;
        }
    }

private:
    BluetoothPbapPseServer::impl *pimpl_ = nullptr;
};

BluetoothPbapPseServer::impl::impl()
{
    HILOGI("starts");
}

BluetoothPbapPseServer::impl::~impl()
{
    HILOGI("starts");
}

BluetoothPbapPseServer::BluetoothPbapPseServer()
{
    HILOGI("starts");
    pimpl = std::make_unique<impl>();
    pimpl->observerImp_->SetObserver(&(pimpl->observers_));
    pimpl->systemStateObserver_ = std::make_unique<impl::SystemStateObserver>(pimpl.get());
    IAdapterManager::GetInstance()->RegisterSystemStateObserver(*(pimpl->systemStateObserver_));

    IProfileManager *serviceMgr = IProfileManager::GetInstance();
    CHECK_AND_RETURN_LOG((serviceMgr != nullptr), "serviceMgr is null");
    pimpl_->pbapPseService_ = (IProfilePbapPse *)serviceMgr->GetProfileService(PROFILE_NAME_PBAP_PSE);
    CHECK_AND_RETURN_LOG((pimpl_->pbapPseService_ != nullptr), "pbapPseService_ is null");
    pimpl_->pbapPseService_->RegisterObserver(pimpl_->observerImp_.get());
}

BluetoothPbapPseServer::~BluetoothPbapPseServer()
{
    HILOGI("starts");
    IAdapterManager::GetInstance()->DeregisterSystemStateObserver(*(pimpl->systemStateObserver_));
    CHECK_AND_RETURN_LOG((pimpl_->pbapPseService_ != nullptr), "pbapPseService_ is null");
    pimpl->pbapPseService_->DeregisterObserver(pimpl->observerImp_.get());
}

void BluetoothPbapPseServer::RegisterObserver(const sptr<IBluetoothPbapPseObserver> &observer)
{
    HILOGI("starts");
    CHECK_AND_RETURN_LOG((observer != nullptr), "observer is null");
    CHECK_AND_RETURN_LOG((pimpl != nullptr), "pimpl is null");
    auto func = std::bind(&BluetoothPbapPseServer::DeregisterObserver, this, std::placeholders::_1);
    pimpl->observers_.Register(observer, func);
}

void BluetoothPbapPseServer::DeregisterObserver(const sptr<IBluetoothPbapPseObserver> &observer)
{
    HILOGI("starts");
    CHECK_AND_RETURN_LOG((observer != nullptr), "observer is null");
    CHECK_AND_RETURN_LOG((pimpl != nullptr), "pimpl is null");
    pimpl->observers_.Deregister(observer);
}

int32_t BluetoothPbapPseServer::Disconnect(const BluetoothRawAddress &device)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothPbapPseServer::GetDeviceState(const BluetoothRawAddress &device, int &state)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothPbapPseServer::GetDevicesByStates(const std::vector<int32_t> &states,
    std::vector<BluetoothRawAddress> &rawDevices)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothPbapPseServer::SetConnectionStrategy(const BluetoothRawAddress &device, int32_t strategy)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothPbapPseServer::GetConnectionStrategy(const BluetoothRawAddress &device, int32_t &strategy)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothPbapPseServer::SetShareType(const BluetoothRawAddress &device, int32_t shareType)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothPbapPseServer::GetShareType(const BluetoothRawAddress &device, int32_t &shareType)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothPbapPseServer::SetPhoneBookAccessAuthorization(const BluetoothRawAddress &device,
    int32_t accessAuthorization)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothPbapPseServer::GetPhoneBookAccessAuthorization(const BluetoothRawAddress &device,
    int32_t &accessAuthorization)
{
    return BT_ERR_API_NOT_SUPPORT;
}
}  // namespace Bluetooth
}  // namespace OHOS
