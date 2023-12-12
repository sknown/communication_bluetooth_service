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
#include "bluetooth_log.h"
#include "ipc_types.h"
#include "bluetooth_pbap_pse_stub.h"

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
BluetoothPbapPseStub::BluetoothPbapPseStub()
{
    HILOGD("start.");
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_GET_DEVICE_STATE)] =
        &BluetoothPbapPseStub::GetDeviceStateInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_GET_DEVICES_BY_STATES)] =
        &BluetoothPbapPseStub::GetDevicesByStatesInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_DISCONNECT)] =
        &BluetoothPbapPseStub::DisconnectInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_SET_CONNECTION_STRATEGY)] =
        &BluetoothPbapPseStub::SetConnectionStrategyInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_GET_CONNECTION_STRATEGY)] =
        &BluetoothPbapPseStub::GetConnectionStrategyInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_REGISTER_OBSERVER)] =
        &BluetoothPbapPseStub::RegisterObserverInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_DEREGISTER_OBSERVER)] =
        &BluetoothPbapPseStub::DeregisterObserverInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_SET_SHARE_TYPE)] =
        &BluetoothPbapPseStub::SetShareTypeInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_GET_SHARE_TYPE)] =
        &BluetoothPbapPseStub::GetShareTypeInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_SET_ACCESS_AUTHORIZATION)] =
        &BluetoothPbapPseStub::SetPhoneBookAccessAuthorizationInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothPbapPseInterfaceCode::PBAP_PSE_GET_ACCESS_AUTHORIZATION)] =
        &BluetoothPbapPseStub::GetPhoneBookAccessAuthorizationInner;
}

BluetoothPbapPseStub::~BluetoothPbapPseStub()
{
    HILOGD("start.");
    memberFuncMap_.clear();
}

int32_t BluetoothPbapPseStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGD("cmd = %{public}d, flags= %{public}d", code, option.GetFlags());
    if (BluetoothPbapPseStub::GetDescriptor() != data.ReadInterfaceToken()) {
        HILOGE("local descriptor is not equal to remote");
        return ERR_INVALID_STATE;
    }
    auto itFunc = memberFuncMap_.find(code);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data, reply);
        }
    }
    HILOGW("default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

int32_t BluetoothPbapPseStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");

    int32_t state;
    int32_t ret = GetDeviceState(*device, state);

    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    CHECK_AND_RETURN_LOG_RET(ret == BT_NO_ERROR, BT_ERR_INTERNAL_ERROR, "internal error");
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(state), BT_ERR_INTERNAL_ERROR, "reply write state failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int32_t> states;
    CHECK_AND_RETURN_LOG_RET(data.ReadInt32Vector(&states), BT_ERR_INTERNAL_ERROR, "Read states failed");

    std::vector<BluetoothRawAddress> rawAdds;
    int32_t ret = GetDevicesByStates(states, rawAdds);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(rawAdds.size()), BT_ERR_INTERNAL_ERROR, "reply write devices failed");

    for (auto rawAdd : rawAdds) {
        if (!reply.WriteParcelable(&rawAdd)) {
            HILOGE("write WriteString failed");
            return BT_ERR_INTERNAL_ERROR;
        }
    }
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");

    int32_t ret = Disconnect(*device);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::SetConnectionStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");
    int32_t strategy = data.ReadInt32();

    int32_t ret = SetConnectionStrategy(*device, strategy);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::GetConnectionStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");

    int32_t strategy = 0;
    int32_t ret = GetConnectionStrategy(*device, strategy);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    CHECK_AND_RETURN_LOG_RET(ret == BT_NO_ERROR, BT_ERR_INTERNAL_ERROR, "internal error");
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(strategy), BT_ERR_INTERNAL_ERROR, "reply write strategy failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothPbapPseObserver> observer = OHOS::iface_cast<IBluetoothPbapPseObserver>(remote);
    RegisterObserver(observer);
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothPbapPseObserver> observer = OHOS::iface_cast<IBluetoothPbapPseObserver>(remote);
    DeregisterObserver(observer);
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::SetShareTypeInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");
    int32_t shareType = data.ReadInt32();

    int32_t ret = SetShareType(*device, shareType);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::GetShareTypeInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");

    int32_t shareType = 0;
    int32_t ret = GetShareType(*device, shareType);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    CHECK_AND_RETURN_LOG_RET(ret == BT_NO_ERROR, BT_ERR_INTERNAL_ERROR, "internal error");
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(shareType), BT_ERR_INTERNAL_ERROR, "reply write shareType failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::SetPhoneBookAccessAuthorizationInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");
    int32_t accessAuthorization = data.ReadInt32();

    int32_t ret = SetPhoneBookAccessAuthorization(*device, accessAuthorization);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    return BT_NO_ERROR;
}

int32_t BluetoothPbapPseStub::GetPhoneBookAccessAuthorizationInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device != nullptr, BT_ERR_INTERNAL_ERROR, "Read BluetoothRawAddress failed");

    int32_t accessAuthorization = 0;
    int32_t ret = GetPhoneBookAccessAuthorization(*device, accessAuthorization);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "reply write ret failed");
    CHECK_AND_RETURN_LOG_RET(ret == BT_NO_ERROR, BT_ERR_INTERNAL_ERROR, "internal error");
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(accessAuthorization), BT_ERR_INTERNAL_ERROR,
        "reply write accessAuthorization failed");
    return BT_NO_ERROR;
}

}  // namespace Bluetooth
}  // namespace OHOS