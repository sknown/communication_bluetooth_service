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

#ifndef BLUETOOTH_DIALOG_H
#define BLUETOOTH_DIALOG_H

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include "foundation/ability/ability_runtime/interfaces/kits/native/ability/ability_runtime/ability_connection.h"
#include "foundation/communication/bluetooth_service/services/bluetooth/service/src/util/timer.h"
#include "json/json.h"
#include "json/writer.h"
#include "raw_address.h"

namespace OHOS {
namespace bluetooth {
enum DialogType : uint8_t {
    PBAP_AUTH_DIALOG = 0x00,
    MAP_AUTH_DIALOG,
    AUTO_PLAY_AUTH_DIALOG,
};

struct DialogInfo {
    DialogType type;
    std::string address;
    uint32_t timeOutMs;
    std::shared_ptr<utility::Timer> timer;
};

class BluetoothDialog {
public:
    static bool RequestAuthDialog(DialogInfo& dialog);

    static bool DismissCurAndShowNext();

    static bool DialogConnectExtension(
        const std::string commandStr, const std::string bundleName, const std::string abilityName);

    static bool DialogConnectExtensionAbility(const AAFwk::Want& want, const std::string commandStr,
        const std::string bundleName, const std::string abilityName);

private:
    static std::string BuildStartCommand(const std::string& address);

    static bool DisplayDialog(DialogInfo& dialog);
};

} // namespace bluetooth
} // namespace OHOS
#endif
