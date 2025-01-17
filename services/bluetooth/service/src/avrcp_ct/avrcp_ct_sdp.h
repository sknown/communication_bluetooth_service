/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef AVRCP_CT_SDP_H
#define AVRCP_CT_SDP_H

#include "avrcp_ct_internal.h"
#include "sdp.h"
#define AVRC_SDP_ATTRIBUTE_NUM 3

namespace OHOS {
namespace bluetooth {
// The attribute id of the supported features.
const uint16_t AVRC_CT_ATTRIBUTE_ID_SUPPORTED_FEATURES = 0x0311;

// All of the features that can registered into the SDP.
const uint16_t AVRC_CT_SDP_ALL_SUPPORTED_FEATURES =
    (AVRC_CT_FEATURE_CATEGORY_1 | AVRC_CT_FEATURE_CATEGORY_2 | AVRC_CT_FEATURE_CATEGORY_3 | AVRC_CT_FEATURE_CATEGORY_4 |
        AVRC_CT_FEATURE_BROWSING | AVRC_CT_FEATURE_COVER_ART_IMAGE_PROPERTIES | AVRC_CT_FEATURE_COVER_ART_IMAGE |
        AVRC_CT_FEATURE_COVER_ART_THUMBNAIL);

/**
 * @brief This class provides a set of methods for registering/unregistering service record into the SDP and
 * finding the service record from the SDP.
 * @see Audio/Video Remote Control 1.6.2 Section 8 Service discovery interoperability requirements ->
 *      Table 8.2: Service record for CT.
 */
class AvrcCtSdpManager {
public:
    /**
     * @brief A constructor used to create an <b>AvrcCtSdpManager</b> instance.
     */
    explicit AvrcCtSdpManager(uint32_t features);

    /**
     * @brief A destructor used to delete the <b>AvrcCtSdpManager</b> instance.
     */
    ~AvrcCtSdpManager();

    /**
     * @brief Registers the service record into the SDP.
     *
     * @return The result of the method execution.
     * @retval BT_SUCCESS   Execute success.
     * @retval RET_BAD_STATUS Execute failure.
     */
    int RegisterService(void);

    /**
     * @brief Unregisters the service record from the SDP.
     */
    int UnregisterService(void) const;

    /**
     * @brief Finds the service record from the SDP.
     *
     * @param[in] rawAddr  The address of the bluetooth device.
     * @param[in] callback The callback function that receives the search result.
     * @return The result of the method execution.
     * @retval BT_SUCCESS   Execute success.
     * @retval RET_BAD_STATUS Execute failure.
     */
    static int FindTgService(const RawAddress &rawAddr,
        void (*callback)(const BtAddr *btAddr, const SdpService *serviceArray, uint16_t serviceNum, void *context));

private:
    uint32_t sdpHandle_;  // The handle got from the SDP.
    uint32_t features_;   // The features supported by the AVRCP CT service.

    /**
     * @brief Checks the category1 feature is supported or not.
     *
     * @return The result of the method execution.
     * @retval true  The category1 feature is supported..
     * @retval false The category1 feature is unsupported..
     */
    bool IsSupportedCategory1(void) const
    {
        return ((features_ & AVRC_CT_FEATURE_CATEGORY_1) == AVRC_CT_FEATURE_CATEGORY_1);
    }

    /**
     * @brief Checks the category2 feature is supported or not.
     *
     * @return The result of the method execution.
     * @retval true  The category2 feature is supported..
     * @retval false The category2 feature is unsupported..
     */
    bool IsSupportedCategory2(void) const
    {
        return ((features_ & AVRC_CT_FEATURE_CATEGORY_2) == AVRC_CT_FEATURE_CATEGORY_2);
    }

    int AddProtocolDescriptorList();
};
}  // namespace bluetooth
}  // namespace OHOS

#endif  // !AVRCP_CT_SDP_H
