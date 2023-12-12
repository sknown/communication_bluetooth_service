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

/**
 * @addtogroup Bluetooth
 * @{
 *
 * @brief Defines pbap pse service interface, including observer and api functions.
 *
 */

#ifndef INTERFACE_PROFILE_PBAP_PSE_H
#define INTERFACE_PROFILE_PBAP_PSE_H

#include "interface_profile.h"

namespace OHOS {
namespace bluetooth {
/**
 * @brief pan observer for framework api
 *
 */
class IPbapPseObserver {
public:
    /**
     * @brief Destroy the IPbapPseObserver object
     *
     */
    virtual ~IPbapPseObserver() = default;

    /**
     * @brief  ConnectionState Changed
     *
     * @param  device bluetooth address
     * @param  state changed status
     */
    virtual void OnConnectionStateChanged(const RawAddress &device, int32_t state) = 0;
};

class IProfilePbapPse : public IProfile {
public:
    /**
     * @brief  register observer
     *
     * @param  observer function pointer
     */
    virtual void RegisterObserver(IPbapPseObserver &observer) = 0;

    /**
     * @brief  deregister observer
     *
     * @param  observer function pointer
     */
    virtual void DeregisterObserver(IPbapPseObserver &observer) = 0;

    /**
     * @brief Get remote devices with the specified states.
     *
     * @param states List of remote device states.
     * @return Returns the list of devices.
     */
    virtual std::vector<RawAddress> GetDevicesByStates(const std::vector<int32_t> &states) = 0;

    /**
     * @brief Get the connection state of the specified remote device.
     *
     * @param device Remote device object.
     * @return Returns the connection state of the remote device.
     */
    virtual int32_t GetDeviceState(const RawAddress &device) = 0;

    /**
     * @brief Set the connection policy of the specified device.
     *
     * @param device Remote device object.
     * @param strategy The connection policy of device.
     * @return Result.
     */
    virtual int32_t SetConnectionStrategy(const RawAddress &device, int32_t strategy) = 0;

    /**
     * @brief Get the connection policy of the specified device.
     *
     * @param device Remote device object.
     * @param[out] strategy The connection policy of device.
     * @return Result.
     */
    virtual int32_t GetConnectionStrategy(const RawAddress &device, int32_t &strategy) = 0;

    /**
     * @brief Set the share type of the specified device.
     *
     * @param device Remote device object.
     * @param shareType The share type of device.
     * @return Result.
     */
    virtual int32_t SetShareType(const RawAddress &device, int32_t shareType) = 0;

    /**
     * @brief Get the share type of the specified device.
     *
     * @param device Remote device object.
     * @param[out] shareType The share type of device.
     * @return Result.
     */
    virtual int32_t GetShareType(const RawAddress &device, int32_t &shareType) = 0;

    /**
     * @brief Set the phonebook access authorization of the specified device.
     *
     * @param device Remote device object.
     * @param accessAuthorization The phonebook access authorization of device.
     * @return Result.
     */
    virtual int32_t SetPhoneBookAccessAuthorization(const RawAddress &device, int32_t accessAuthorization) = 0;

    /**
     * @brief Get the phonebook access authorization of the specified device.
     *
     * @param device Remote device object.
     * @param[out] accessAuthorization The phonebook access authorization of device.
     * @return Result.
     */
    virtual int32_t GetPhoneBookAccessAuthorization(const RawAddress &device, int32_t &accessAuthorization) = 0;
};
}  // namespace bluetooth
}  // namespace OHOS
#endif  // INTERFACE_PROFILE_PBAP_PSE_H