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

#include <map>
#include <vector>

#include "bluetooth_ble_central_manager_stub.h"
#include "bluetooth_log.h"
#include "ipc_types.h"
#include "parcel_bt_uuid.h"
#include "bluetooth_bt_uuid.h"

namespace OHOS {
namespace Bluetooth {
const int32_t BLE_CENTRAL_MANAGER_STUB_READ_DATA_SIZE_MAX_LEN = 0x100;
const std::map<uint32_t, std::function<ErrCode(BluetoothBleCentralManagerStub *, MessageParcel &, MessageParcel &)>>
    BluetoothBleCentralManagerStub::interfaces_ = {
        {IBluetoothBleCentralManager::Code::BLE_REGISTER_BLE_CENTRAL_MANAGER_CALLBACK,
            std::bind(&BluetoothBleCentralManagerStub::RegisterBleCentralManagerCallbackInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_DE_REGISTER_BLE_CENTRAL_MANAGER_CALLBACK,
            std::bind(&BluetoothBleCentralManagerStub::DeregisterBleCentralManagerCallbackInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_START_SCAN,
            std::bind(&BluetoothBleCentralManagerStub::StartScanInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_START_SCAN_WITH_SETTINGS,
            std::bind(&BluetoothBleCentralManagerStub::StartScanWithSettingsInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_CONFIG_SCAN_FILTER,
            std::bind(&BluetoothBleCentralManagerStub::ConfigScanFilterInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_REMOVE_SCAN_FILTER,
            std::bind(&BluetoothBleCentralManagerStub::RemoveScanFilterInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_STOP_SCAN,
            std::bind(&BluetoothBleCentralManagerStub::StopScanInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_PROXY_UID,
            std::bind(&BluetoothBleCentralManagerStub::ProxyUidInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_RESET_ALL_PROXY,
            std::bind(&BluetoothBleCentralManagerStub::ResetAllProxyInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_SET_BURST_PARAM,
            std::bind(&BluetoothBleCentralManagerStub::SetBurstParamInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_SET_SCAN_REPORT_CHANNEL_TO_SH,
            std::bind(&BluetoothBleCentralManagerStub::SetScanReportChannelToSensorHubInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_START_SCAN_IN_SH_SYNC,
            std::bind(&BluetoothBleCentralManagerStub::StartScanInShSuncInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_STOP_SCAN_IN_SH_SYNC,
            std::bind(&BluetoothBleCentralManagerStub::StopScanInShSuncInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_SEND_PARAMS_TO_SH,
            std::bind(&BluetoothBleCentralManagerStub::SendParamsToSensorHubInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_IS_SUPPORT_SENSOR_ADVERTISER_FILTER,
            std::bind(&BluetoothBleCentralManagerStub::IsSupportSensorAdvertiseFilterInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_SET_SH_ADV_FILTER_PARAM,
            std::bind(&BluetoothBleCentralManagerStub::SetAdvFilterParamInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {IBluetoothBleCentralManager::Code::BLE_REMOVE_SH_ADV_FILTER,
            std::bind(&BluetoothBleCentralManagerStub::RemoveAdvFilterInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
};

BluetoothBleCentralManagerStub::BluetoothBleCentralManagerStub()
{}

BluetoothBleCentralManagerStub::~BluetoothBleCentralManagerStub()
{}

int BluetoothBleCentralManagerStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    if (BluetoothBleCentralManagerStub::GetDescriptor() != data.ReadInterfaceToken()) {
        HILOGW("[OnRemoteRequest] fail: invalid interface token!");
        return OBJECT_NULL;
    }

    auto it = interfaces_.find(code);
    if (it == interfaces_.end()) {
        HILOGW("[OnRemoteRequest] fail: unknown code!");
        return IRemoteStub<IBluetoothBleCentralManager>::OnRemoteRequest(code, data, reply, option);
    }

    auto fun = it->second;
    if (fun == nullptr) {
        HILOGW("[OnRemoteRequest] fail: not find function!");
        return IRemoteStub<IBluetoothBleCentralManager>::OnRemoteRequest(code, data, reply, option);
    }

    ErrCode result = fun(this, data, reply);
    if (SUCCEEDED(result)) {
        return NO_ERROR;
    }

    HILOGW("[OnRemoteRequest] fail: Failed to call interface %{public}u, err:%{public}d", code, result);
    return result;
}

ErrCode BluetoothBleCentralManagerStub::RegisterBleCentralManagerCallbackInner(
    MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothBleCentralManagerCallback> callBack =
        OHOS::iface_cast<IBluetoothBleCentralManagerCallback>(remote);
    int32_t scannerId = 0;
    RegisterBleCentralManagerCallback(scannerId, callBack);
    if (!reply.WriteInt32(scannerId)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::DeregisterBleCentralManagerCallbackInner(
    MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothBleCentralManagerCallback> callBack =
        OHOS::iface_cast<IBluetoothBleCentralManagerCallback>(remote);
    DeregisterBleCentralManagerCallback(scannerId, callBack);
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StartScanInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    int ret = StartScan(scannerId);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StartScanWithSettingsInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    std::shared_ptr<BluetoothBleScanSettings> settings(data.ReadParcelable<BluetoothBleScanSettings>());
    if (settings == nullptr) {
        HILOGW("[StartScanWithSettingsInner] fail: read settings failed");
        return TRANSACTION_ERR;
    }

    int ret = StartScan(scannerId, *settings);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StopScanInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    int ret = StopScan(scannerId);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}
ErrCode BluetoothBleCentralManagerStub::ConfigScanFilterInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<BluetoothBleScanFilter> filters {};
    int32_t scannerId = data.ReadInt32();
    int32_t itemsSize = 0;
    if (!data.ReadInt32(itemsSize) || itemsSize > BLE_CENTRAL_MANAGER_STUB_READ_DATA_SIZE_MAX_LEN) {
        HILOGE("read Parcelable size failed.");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < itemsSize; i++) {
        std::shared_ptr<BluetoothBleScanFilter> res(data.ReadParcelable<BluetoothBleScanFilter>());
        if (res == nullptr) {
            HILOGE("null pointer");
            return ERR_INVALID_VALUE;
        }
        BluetoothBleScanFilter item = *(res);
        filters.push_back(item);
    }

    int result = ConfigScanFilter(scannerId, filters);
    bool resultRet = reply.WriteInt32(result);
    bool idRet = reply.WriteInt32(scannerId);
    if (!(resultRet && idRet)) {
        HILOGE("BluetoothBleCentralManagerStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::RemoveScanFilterInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();

    RemoveScanFilter(scannerId);
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::ProxyUidInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t uid = data.ReadInt32();
    bool isProxy = data.ReadBool();

    bool ret = ProxyUid(uid, isProxy);
    if (!reply.WriteBool(ret)) {
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::ResetAllProxyInner(MessageParcel &data, MessageParcel &reply)
{
    bool ret = ResetAllProxy();
    if (!reply.WriteBool(ret)) {
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SetBurstParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SetScanReportChannelToSensorHubInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StartScanInShSuncInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StopScanInShSuncInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SendParamsToSensorHubInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::IsSupportSensorAdvertiseFilterInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SetAdvFilterParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::RemoveAdvFilterInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS
