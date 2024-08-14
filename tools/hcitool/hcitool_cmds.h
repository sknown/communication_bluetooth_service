#ifndef HCITOOL_CMDS_H
#define HCITOOL_CMDS_H

namespace OHOS {
namespace BluetoothHciTool {

#define DEFINE_FUNC_MAP(name) { #name, name }

const std::unordered_map<std::string, menu::FunctionType> &HciToolCmdsGetFuncMap(void);

} // namespace BluetoothHciTool
} // namespace OHOS

#endif // HCITOOL_CMDS_H
