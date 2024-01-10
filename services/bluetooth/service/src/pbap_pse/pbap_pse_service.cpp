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

#include "bluetooth_errorcode.h"
#include "pbap_pse_service.h"

namespace OHOS {
namespace bluetooth {
PbapPseService::PbapPseService() : utility::Context(PROFILE_NAME_PBAP_PSE, "1.2.3") {}

PbapPseService::~PbapPseService() {}

utility::Context *PbapPseService::GetContext()
{
    return this;
}

PbapPseService *PbapPseService::GetService()
{
    auto servManager = IProfileManager::GetInstance();
    return static_cast<PbapPseService *>(servManager->GetProfileService(PROFILE_NAME_PBAP_PSE));
}

void PbapPseService::EnableService()
{
    GetContext()->OnEnable(PROFILE_NAME_PBAP_PSE, true);
}

void PbapPseService::Enable()
{
    GetDispatcher()->PostTask(std::bind(&PbapPseService::EnableService, this));
}

void PbapPseService::DisableService()
{
    GetContext()->OnDisable(PROFILE_NAME_PBAP_PSE, true);
}

void PbapPseService::Disable()
{
    GetDispatcher()->PostTask(std::bind(&PbapPseService::DisableService, this));
}

void PbapPseService::RegisterObserver(IPbapPseObserver &observer) {}

void PbapPseService::DeregisterObserver(IPbapPseObserver &observer) {}

int32_t PbapPseService::Connect(const RawAddress &device)
{
    return RET_NO_SUPPORT;
}

int32_t PbapPseService::Disconnect(const RawAddress &device)
{
    return RET_NO_SUPPORT;
}

std::vector<RawAddress> PbapPseService::GetDevicesByStates(const std::vector<int32_t> &states)
{
    std::vector<RawAddress> devices {};
    return devices;
}

int32_t PbapPseService::GetDeviceState(const RawAddress &device)
{
    return static_cast<int32_t>(BTConnectState::DISCONNECTED);
}

int32_t PbapPseService::GetConnectState()
{
    return static_cast<int32_t>(BTConnectState::DISCONNECTED);
}

int32_t PbapPseService::GetMaxConnectNum()
{
    return 0;
}

std::list<RawAddress> PbapPseService::GetConnectDevices()
{
    std::list<RawAddress> devices;
    return devices;
}

int32_t PbapPseService::SetConnectionStrategy(const RawAddress &device, int32_t strategy)
{
    return RET_NO_SUPPORT;
}

int32_t PbapPseService::GetConnectionStrategy(const RawAddress &device, int32_t &strategy)
{
    return RET_NO_SUPPORT;
}

int32_t PbapPseService::SetShareType(const RawAddress &device, int32_t shareType)
{
    return RET_NO_SUPPORT;
}

int32_t PbapPseService::GetShareType(const RawAddress &device, int32_t &shareType)
{
    return RET_NO_SUPPORT;
}

int32_t PbapPseService::SetPhoneBookAccessAuthorization(const RawAddress &device, int32_t accessAuthorization)
{
    return RET_NO_SUPPORT;
}

int32_t PbapPseService::GetPhoneBookAccessAuthorization(const RawAddress &device, int32_t &accessAuthorization)
{
    return RET_NO_SUPPORT;
}

REGISTER_CLASS_CREATOR(PbapPseService);
}  // namespace bluetooth
}  // namespace OHOS
