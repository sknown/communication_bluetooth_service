#include <iomanip>
#include <iostream>
#include <sys/select.h>
#include <fstream>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

#include "menu.h"
#include "cmdline.h"
#include "linenoise.h"

using json = nlohmann::json;

namespace menu {

bool MenuWrapper::backMenu_;
const Menu *MenuWrapper::storedMenu_;
struct linenoiseState MenuWrapper::linenoise_;
std::atomic<int> MenuWrapper::shared_atomic_variable;

MenuWrapper &MenuWrapper::getInstance()
{
    static MenuWrapper instance;
    return instance;
}

Menu MenuWrapper::createMenu(const std::string &name,
        const std::string &prompt, std::shared_ptr<cmdline::parser> &parser)
{
    Menu menu(name, prompt, parser);

    // 创建默认菜单项--menu
    std::shared_ptr<cmdline::parser> parseMenu = std::make_shared<cmdline::parser>();
    parseMenu->set_program("menu", "Select submenu");
    parseMenu->add<std::string>("name", 'n', "menu name", true, "");
    addMenuItem(menu, {parseMenu, menuItemSwitch});

    // 创建默认菜单项--help
    std::shared_ptr<cmdline::parser> parseHelp = std::make_shared<cmdline::parser>();
    parseHelp->set_program("help", "Display help about this program");
    parseHelp->add("all", 'a', "Show all details");
    addMenuItem(menu, {parseHelp, menuItemHelp});

    // 创建默认菜单项--back
    std::shared_ptr<cmdline::parser> parseBack = std::make_shared<cmdline::parser>();
    parseBack->set_program("back", "Exit or return to the previous menu");
    addMenuItem(menu, {parseBack, menuItemBack});

    // 创建默认菜单项--clear
    std::shared_ptr<cmdline::parser> parseClear = std::make_shared<cmdline::parser>();
    parseClear->set_program("clear", "Clear Screen");
    addMenuItem(menu, {parseClear, menuItemClear});

    return menu;
}

MenuItem MenuWrapper::CreateMenuItem(const std::string& name, const std::string& description,
        const std::function<void(const std::shared_ptr<cmdline::parser>&)>& handler,
        const std::vector<std::shared_ptr<OptionBase>>& options)
{
    std::shared_ptr<cmdline::parser> parser = std::make_shared<cmdline::parser>();
    parser->set_program(name, description);

    for (const auto& option : options) {
        option->addToParser(parser);
    }

    MenuItem item = {parser, handler};
    return item;
}

std::vector<MenuItem> MenuWrapper::LoadMenuItemsFromJson(const std::string &jsonFile,
    const std::unordered_map<std::string, FunctionType> &funcMap)
{
    std::ifstream jsonFileStream(jsonFile);
    if (!jsonFileStream.is_open()) {
        std::cerr << "Error opening JSON file: " << jsonFile << std::endl;
        return {};
    }

    try {
        json root = json::parse(jsonFileStream);
        jsonFileStream.close();

        std::vector<MenuItem> menuItems;
        for (const auto &menuItem : root) {
            std::string name = menuItem["name"];
            std::string description = menuItem["description"];
            std::string callback = menuItem["callback"];

            std::vector<std::shared_ptr<OptionBase>> options;
            if (menuItem.find("options") != menuItem.end()) {
                const auto &optionsArray = menuItem["options"];
                for (const auto &option : optionsArray) {
                    std::string type = option["type"];

                    if (type == "int") {
                        int defaultValue = option.value("defaultValue", 0);
                        std::vector<int> allowedValues = option.value("allowedValues", std::vector<int>());
                        std::shared_ptr<OptionValue<int>> optionValue;
                        cmdline::oneof_reader<int> reader;
                        for (auto value : allowedValues) {
                            reader.add(value);
                        }
                        optionValue = std::make_shared<OptionValue<int>>(
                            option["name"].get<std::string>(),
                            option["shortcut"].get<std::string>()[0],
                            option["description"].get<std::string>(),
                            option["required"].get<bool>(),
                            defaultValue,
                            reader
                        );
                        options.push_back(optionValue);
                    } else if (type == "string") {
                        std::string defaultValue = option.value("defaultValue", "");
                        std::vector<std::string> allowedValues = option.value("allowedValues", std::vector<std::string>());
                        std::shared_ptr<OptionValue<std::string>> optionValue;
                        cmdline::oneof_reader<std::string> reader;
                        for (auto value : allowedValues) {
                            reader.add(value);
                        }
                        optionValue = std::make_shared<OptionValue<std::string>>(
                            option["name"].get<std::string>(),
                            option["shortcut"].get<std::string>()[0],
                            option["description"].get<std::string>(),
                            option["required"].get<bool>(),
                            defaultValue,
                            reader
                        );
                        options.push_back(optionValue);
                    }
                }
            }

            auto func = funcMap.find(callback);
            if (func != funcMap.end()) {
                menuItems.emplace_back(
                    CreateMenuItem(
                        name,
                        description,
                        std::bind(func->second, std::placeholders::_1),
                        options
                    )
                );
            }
        }

        return menuItems;
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
        return {};
    }
}

bool MenuWrapper::addMenuItem(Menu &menu, const MenuItem &item)
{
    if (menu.items.find(item.name) != menu.items.end()) {
        return false; // Item already exists
    }
    menu.addMenuItem(item);
    return true;
}

bool MenuWrapper::addMenuItems(Menu &menu, const std::vector<MenuItem> &items)
{
    bool result = true;

    for (const auto &item : items) {
        result = addMenuItem(menu, item);
        if (result == false) {
            return false;
        }
    }

    return true;
}

bool MenuWrapper::addSubMenu(Menu &menu, const Menu &subMenu)
{
    if (menu.subMenus.find(subMenu.name) != menu.subMenus.end()) {
        return false; // SubMenu already exists
    }
    menu.addSubMenu(subMenu);
    return true;
}

bool MenuWrapper::runMenu(const Menu &menu)
{
    bool parseResult = false;
    std::vector<std::string> args;

    storedMenu_ = &menu;

    menuItemHelp(nullptr);

    while (true) {
        args.clear();

        std::string line = asyncInput(menu.prompt);
        parseLine(line, args);
        if (args.empty()) {
            continue;
        }

        auto it = menu.items.find(args[0]);
        if (it == menu.items.end()) {
            std::cerr << "Invalid option" << std::endl;
            continue;
        }

        it->second.parser->resetAll();
        parseResult = it->second.parser->parse(args);

        if (it->second.parser->exist("help")) {
            std::cout << it->second.parser->usage() << std::endl;
            continue;
        }

        if (parseResult == false) {
            std::cerr << it->second.parser->error() << std::endl;
            continue;
        }

        it->second.func(it->second.parser);

        storedMenu_ = &menu;

        if (backMenu_) {
            backMenu_ = false;
            break;
        }
    }

    return true;
}

void MenuWrapper::asyncOutput(const std::string &message)
{
    if (shared_atomic_variable == 1) {
        linenoiseHide(&linenoise_);
        std::cout << message << std::endl;
        linenoiseShow(&linenoise_);
    } else {
        std::cout << message << std::endl;
    }
}

void MenuWrapper::asyncOutput(const std::string &prompt, const std::string &message)
{
    if (shared_atomic_variable == 1) {
        linenoiseHide(&linenoise_);
        std::cout << "[" << prompt << "] " << message << std::endl;
        linenoiseShow(&linenoise_);
    } else {
        std::cout << "[" << prompt << "] " << message << std::endl;
    }
}

std::string MenuWrapper::asyncInput(const std::string &prompt)
{
    char buf[1024];
    char *c_line = NULL;

    linenoiseEditStart(&linenoise_, -1, -1, buf, sizeof(buf), prompt.c_str());
    shared_atomic_variable = 1;

    while(1) {
        int retval;
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(linenoise_.ifd, &readfds);

        retval = select(linenoise_.ifd+1, &readfds, NULL, NULL, NULL);
        if (retval == -1) {
            perror("select()");
            exit(1);
        } else if (retval) {
            c_line = linenoiseEditFeed(&linenoise_);
            /* A NULL return means: line editing is continuing.
            * Otherwise the user hit enter or stopped editing
            * (CTRL+C/D). */
            if (c_line != linenoiseEditMore) break;
        }
    }
    shared_atomic_variable = 0;
    linenoiseEditStop(&linenoise_);
    if (c_line == NULL) exit(0); /* Ctrl+D/C. */

    linenoiseHistoryAdd(c_line); /* Add to the history. */
    linenoiseHistorySave(MENU_HISTORY_PATH); /* Save the history on disk. */

    std::string line(c_line);
    linenoiseFree(c_line);

    return line;
}

void MenuWrapper::parseLine(const std::string &input, std::vector<std::string> &args)
{
    size_t start = 0, end = 0;
    while ((end = input.find(' ', start)) != std::string::npos) {
        args.push_back(input.substr(start, end - start));
        start = end + 1;
    }
    if (start < input.length()) {
        args.push_back(input.substr(start));
    }
}

void MenuWrapper::menuItemHelp(const std::shared_ptr<cmdline::parser> &parser)
{
    if (parser != nullptr && parser->exist("all")) {
        for (const auto& subMenu : storedMenu_->subMenus) {
            std::cout << subMenu.second.parser->usage() << std::endl;
        }
        for (const auto& item : storedMenu_->items) {
            std::cout << item.second.parser->usage() << std::endl;
        }
    } else {
        for (const auto& subMenu : storedMenu_->subMenus) {
            std::cout << subMenu.second.parser->about("* ");
        }
        for (const auto& item : storedMenu_->items) {
            std::cout << item.second.parser->about();
        }
    }
}

void MenuWrapper::menuItemSwitch(const std::shared_ptr<cmdline::parser> &parser)
{
    if (!parser->exist("name")) {
        std::cerr << parser->error() << std::endl;
        return ;
    }

    auto it = storedMenu_->subMenus.find(parser->get<std::string>("name"));
    if (it == storedMenu_->subMenus.end()) {
        std::cerr << "The submenu name is invalid option." << std::endl;
        return;
    }

    runMenu(it->second);
}

void MenuWrapper::menuItemBack(const std::shared_ptr<cmdline::parser> &parser)
{
    backMenu_ = true;
}

void MenuWrapper::menuItemClear(const std::shared_ptr<cmdline::parser> &parser)
{
    linenoiseClearScreen();
}

} // namespace menu