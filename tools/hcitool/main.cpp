#include <iostream>
#include <string>
#include "cmdline.h"
#include "menu.h"
#include "hcitool_cmds.h"

using namespace menu;
namespace BluetoothTool = OHOS::BluetoothHciTool;

const std::string BLUETOOTH_HCITOOL_PROMPT = "[hcitool]#";
const std::string BLUETOOTH_HCITOOL_MENU_CONFIG_FILE = "/data/bluetooth/etc/hcitool_menus.json";

int main(int argc, char* argv[])
{
    MenuWrapper &menuWrapper = MenuWrapper::getInstance();

    std::shared_ptr<cmdline::parser> mainParser = std::make_shared<cmdline::parser>();
    mainParser->set_program("Main", "This is the main menu");

    auto mainMenu = menuWrapper.createMenu(
        "Main",
        BLUETOOTH_HCITOOL_PROMPT,
        mainParser
    );

    menuWrapper.addMenuItems(
        mainMenu,
        MenuWrapper::LoadMenuItemsFromJson(
            BLUETOOTH_HCITOOL_MENU_CONFIG_FILE,
            BluetoothTool::HciToolCmdsGetFuncMap()
        )
    );

    menuWrapper.runMenu(mainMenu);

    return 0;
}
