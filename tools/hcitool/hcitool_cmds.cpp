#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "menu.h"
#include "cmdline.h"
#include "hcitool_cmds.h"

#include "btm.h"
#include "gap_if.h"

using namespace menu;

namespace OHOS {
namespace BluetoothHciTool {

static std::vector<MenuItem> g_menuItems;

void HciCmdExample(const std::shared_ptr<cmdline::parser> &parser)
{
    if (parser->exist("OptionTypeInt")) {
        int value = parser->get<int>("OptionTypeInt");
        std::cout << "The value of OptionTypeInt is: " << value << std::endl;
    } else {
        std::cout << "OptionTypeInt option does not exist" << std::endl;
    }

    if (parser->exist("OptionTypeString")) {
        std::string value = parser->get<std::string>("OptionTypeString");
        std::cout << "The value of OptionTypeString is: " << value << std::endl;
    } else {
        std::cout << "OptionTypeString option does not exist" << std::endl;
    }
}

void HciCmdInitialize(const std::shared_ptr<cmdline::parser> &parser)
{
    int result = 0;

    result = BTM_Initialize();
    if (result != BT_SUCCESS) {
        MENU_LOG("ERR", "Hci Controller Initialize Failed, Errorcode: " + std::to_string(result));
        return ;
    }

    MENU_LOG("Hci Controller Initialize successful");
}

void HciCmdClose(const std::shared_ptr<cmdline::parser> &parser)
{
    int result = 0;

    result = BTM_Close();
    if (result != BT_SUCCESS) {
        MENU_LOG("ERR", "Hci Controller Close Failed, Errorcode: " + std::to_string(result));
        return ;
    }

    MENU_LOG("Hci Controller Close Successful");
}

void HciCmdEnable(const std::shared_ptr<cmdline::parser> &parser)
{
    int result = 0;
    if (!parser->exist("ControllerType")) {
        MENU_LOG("ERR", "Invalid input Parameters");
        return ;
    }

    int controller= 0;
    std::string ControllerType = parser->get<std::string>("ControllerType");
    if (ControllerType == "LE") {
        controller = LE_CONTROLLER;
    } else {
        controller = BREDR_CONTROLLER;
    }

    result = BTM_Enable(controller);
    if (result != BT_SUCCESS) {
        MENU_LOG("ERR", "Hci Controller Enable Failed, Errorcode: " + std::to_string(result));
        return ;
    }

    MENU_LOG("Hci Controller Enable Successful");
}

void HciCmdDisable(const std::shared_ptr<cmdline::parser> &parser)
{
    int result = 0;
    if (!parser->exist("ControllerType")) {
        MENU_LOG("ERR", "Invalid input Parameters");
        return ;
    }

    int controller= 0;
    std::string ControllerType = parser->get<std::string>("ControllerType");
    if (ControllerType == "LE") {
        controller = LE_CONTROLLER;
    } else {
        controller = BREDR_CONTROLLER;
    }

    result = BTM_Disable(controller);
    if (result != BT_SUCCESS) {
        MENU_LOG("ERR", "Hci Controller Disable Failed, Errorcode: " + std::to_string(result));
        return ;
    }

    MENU_LOG("Hci Controller Disable Successful");
}

void HciCmdGetLocalAddr(const std::shared_ptr<cmdline::parser> &parser)
{
    int result = 0;
    BtAddr btAddr;

    result = GAPIF_GetLocalAddr(&btAddr);
    if (result != BT_SUCCESS) {
        MENU_LOG("ERR", "Hci Controller GetLocalAddr Failed, Errorcode: " + std::to_string(result));
        return ;
    }

    char addrStr[18];
    sprintf(addrStr, "%02X:%02X:%02X:%02X:%02X:%02X",
        btAddr.addr[5], btAddr.addr[4], btAddr.addr[3],
        btAddr.addr[2], btAddr.addr[1], btAddr.addr[0]);

    MENU_LOG("LocalAddr", addrStr);
}

static const std::unordered_map<std::string, FunctionType> g_hciFuncMap = {
    DEFINE_FUNC_MAP(HciCmdExample),
    DEFINE_FUNC_MAP(HciCmdInitialize),
    DEFINE_FUNC_MAP(HciCmdClose),
    DEFINE_FUNC_MAP(HciCmdEnable),
    DEFINE_FUNC_MAP(HciCmdDisable),
    DEFINE_FUNC_MAP(HciCmdDisable),
    DEFINE_FUNC_MAP(HciCmdGetLocalAddr),
};

const std::unordered_map<std::string, FunctionType> &HciToolCmdsGetFuncMap(void)
{
    return g_hciFuncMap;
}

} // namespace BluetoothHciTool
} // namespace OHOS