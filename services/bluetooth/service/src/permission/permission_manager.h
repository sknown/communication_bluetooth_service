/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef PERMISSION_MANAGER_H
#define PERMISSION_MANAGER_H

#include <string>

#include "tokenid_kit.h"

namespace OHOS {
namespace bluetooth {
class PermissionManager {
public:
    static std::string GetCallingName();

    static std::string GetCallingName(const uint32_t& tokenId);

    static bool IsSystemHap();

    static bool IsSystemHap(const uint64_t& fullTokenId);
};
} // namespace bluetooth
} // namespace OHOS
#endif
