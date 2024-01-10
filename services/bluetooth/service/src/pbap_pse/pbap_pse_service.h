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

#ifndef PBAP_PSE_SERVICE_H
#define PBAP_PSE_SERVICE_H

#include <cstring>
#include <list>
#include <map>
#include <vector>
#include "context.h"
#include "interface_profile_pbap_pse.h"
#include "message.h"
#include "raw_address.h"
#include "profile_service_manager.h"

namespace OHOS {
namespace bluetooth {
class PbapPseService : public IProfilePbapPse, public utility::Context {
public:
    PbapPseService();
    virtual ~PbapPseService();
    void RegisterObserver(IPbapPseObserver &observer) override;
    void DeregisterObserver(IPbapPseObserver &observer) override;
    std::vector<RawAddress> GetDevicesByStates(const std::vector<int32_t> &states) override;
    int32_t GetDeviceState(const RawAddress &device) override;
    int32_t Connect(const RawAddress &device) override;
    int32_t Disconnect(const RawAddress &device) override;

    utility::Context *GetContext() override;
    void Enable(void) override;
    void Disable(void) override;
    int32_t GetConnectState(void) override;
    int32_t GetMaxConnectNum(void) override;
    std::list<RawAddress> GetConnectDevices() override;
    int32_t SetConnectionStrategy(const RawAddress &device, int32_t strategy) override;
    int32_t GetConnectionStrategy(const RawAddress &device, int32_t &strategy) override;
    int32_t SetShareType(const RawAddress &device, int32_t shareType) override;
    int32_t GetShareType(const RawAddress &device, int32_t &shareType) override;
    int32_t SetPhoneBookAccessAuthorization(const RawAddress &device, int32_t accessAuthorization) override;
    int32_t GetPhoneBookAccessAuthorization(const RawAddress &device, int32_t &accessAuthorization) override;
     
    static PbapPseService *GetService();

private:
    void EnableService();
    void DisableService();
};
}  // namespace bluetooth
}  // namespace OHOS
#endif  // PBAP_PSE_SERVICE_H
