/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include "bluetooth_pan_stub.h"
#include "bluetooth_log.h"

namespace OHOS {
namespace Bluetooth {
const uint32_t PAN_DEVICES_STATES_MAX_NUMS = 0XFF;
BluetoothPanStub::BluetoothPanStub()
{
    HILOGD("start.");
    memberFuncMap_[static_cast<uint32_t>(COMMAND_DISCONNECT)] =
        &BluetoothPanStub::DisconnectInner;
    memberFuncMap_[static_cast<uint32_t>(COMMAND_GET_DEVICE_STATE)] =
        &BluetoothPanStub::GetDeviceStateInner;
    memberFuncMap_[static_cast<uint32_t>(COMMAND_GET_DEVICES_BY_STATES)] =
        &BluetoothPanStub::GetDevicesByStatesInner;
    memberFuncMap_[static_cast<uint32_t>(COMMAND_REGISTER_OBSERVER)] =
        &BluetoothPanStub::RegisterObserverInner;
    memberFuncMap_[static_cast<uint32_t>(COMMAND_DEREGISTER_OBSERVER)] =
        &BluetoothPanStub::DeregisterObserverInner;
    memberFuncMap_[static_cast<uint32_t>(COMMAND_SET_TETHERING)] =
        &BluetoothPanStub::SetTetheringInner;
    memberFuncMap_[static_cast<uint32_t>(COMMAND_IS_TETHERING_ON)] =
        &BluetoothPanStub::IsTetheringOnInner;
    HILOGD("ends.");
}

BluetoothPanStub::~BluetoothPanStub()
{
    HILOGD("start.");
    memberFuncMap_.clear();
}

int BluetoothPanStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGI("BluetoothPanStub::OnRemoteRequest, cmd = %{public}d, flags= %{public}d", code, option.GetFlags());
    if (BluetoothPanStub::GetDescriptor() != data.ReadInterfaceToken()) {
        HILOGE("local descriptor is not equal to remote");
        return IPC_INVOKER_TRANSLATE_ERR;
    }
    auto itFunc = memberFuncMap_.find(code);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data, reply);
        }
    }
    HILOGW("BluetoothHfpHfStub::OnRemoteRequest, default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

int32_t BluetoothPanStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t errCode = Disconnect(*device);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t state;
    int32_t errCode = GetDeviceState(*device, state);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write state
    if (!reply.WriteInt32(state)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int32_t> states = {};
    int32_t stateSize = data.ReadInt32();
    if (static_cast<uint32_t>(stateSize) > PAN_DEVICES_STATES_MAX_NUMS) {
        return ERR_INVALID_STATE;
    }
    for (int i = 0; i < stateSize; i++) {
        int32_t state = data.ReadInt32();
        states.push_back(state);
    }
    std::vector<BluetoothRawAddress> rawAdds;
    int32_t errCode = GetDevicesByStates(states, rawAdds);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write size
    if (!reply.WriteInt32(rawAdds.size())) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    // write devices
    for (auto rawAdd : rawAdds) {
        if (!reply.WriteParcelable(&rawAdd)) {
            return BT_ERR_IPC_TRANS_FAILED;
        }
    }
    return NO_ERROR;
}

ErrCode BluetoothPanStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothPanStub::RegisterObserverInner");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothPanObserver> observer = OHOS::iface_cast<IBluetoothPanObserver>(remote);
    RegisterObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothPanStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothPanStub::DeregisterObserverInner");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothPanObserver> observer = OHOS::iface_cast<IBluetoothPanObserver>(remote);
    DeregisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothPanStub::SetTetheringInner(MessageParcel &data, MessageParcel &reply)
{
    const bool value = data.ReadBool();
    int32_t errCode = SetTethering(value);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::IsTetheringOnInner(MessageParcel &data, MessageParcel &reply)
{
    bool result = false;
    int32_t errCode = IsTetheringOn(result);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write result
    if (!reply.WriteInt32(result)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }

    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS