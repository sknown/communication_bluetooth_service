#ifndef MENU_H
#define MENU_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "cmdline.h"
#include "linenoise.h"

#define MENU_HISTORY_PATH       "history.txt"
namespace menu {

using FunctionType = std::function<void(const std::shared_ptr<cmdline::parser> &)>;

struct MenuItem {
    std::string name;
    std::shared_ptr<cmdline::parser> parser;
    FunctionType func;

    MenuItem() = default;
    MenuItem(const std::shared_ptr<cmdline::parser>& itemParser, const FunctionType& itemFunc)
        : parser(itemParser), func(itemFunc) {
        if (parser) {
            name = parser->get_name();
        } else {
            name = "Unnamed";
        }
    }
};

class OptionBase {
public:
    virtual ~OptionBase() = default;
    virtual std::string getName() const = 0;
    virtual char getShortName() const = 0;
    virtual std::string getDesc() const = 0;
    virtual bool isNeeded() const = 0;
    virtual void addToParser(std::shared_ptr<cmdline::parser>& parser) const = 0;
};

template <class T>
class OptionValue : public OptionBase {
public:
    OptionValue(const std::string& name, char short_name, const std::string& desc, bool need, const T& def, const cmdline::oneof_reader<T>& reader = cmdline::oneof_reader<T>())
        : name_(name), short_name_(short_name), desc_(desc), need_(need), def_(def), reader_(reader) {}

    std::string getName() const override { return name_; }
    char getShortName() const override { return short_name_; }
    std::string getDesc() const override { return desc_; }
    bool isNeeded() const override { return need_; }

    void addToParser(std::shared_ptr<cmdline::parser>& parser) const override {
        parser->add<T>(name_, short_name_, desc_, need_, def_, reader_);
    }

private:
    std::string name_;
    char short_name_;
    std::string desc_;
    bool need_;
    T def_;
    cmdline::oneof_reader<T> reader_;
};

class Menu {
public:
    std::string name;
    std::string prompt;
    std::string desc;
    std::shared_ptr<cmdline::parser> parser;
    std::map<std::string, MenuItem> items;
    std::map<std::string, Menu> subMenus;

    Menu() = default;
    Menu(std::string name, std::string prompt, std::shared_ptr<cmdline::parser> parser)
        : name(std::move(name)), prompt(std::move(prompt)), parser(std::move(parser)) {}

    void addMenuItem(const MenuItem &item) {
        items[item.name] = item;
    }

    void addSubMenu(const Menu &subMenu) {
        subMenus[subMenu.name] = subMenu;
    }
};

class MenuWrapper {
public:
    static MenuWrapper &getInstance();
    static Menu createMenu(const std::string &name,
        const std::string &prompt, std::shared_ptr<cmdline::parser> &parser);
    static MenuItem CreateMenuItem(const std::string& name, const std::string& description,
        const std::function<void(const std::shared_ptr<cmdline::parser>&)>& handler,
        const std::vector<std::shared_ptr<OptionBase>>& options);
    static std::vector<MenuItem> LoadMenuItemsFromJson(const std::string &jsonFile,
        const std::unordered_map<std::string, FunctionType> &funcMap);
    static bool addMenuItem(Menu &menu, const MenuItem &item);
    static bool addMenuItems(Menu &menu, const std::vector<MenuItem> &items);
    static bool addSubMenu(Menu &menu, const Menu &subMenu);
    static bool runMenu(const Menu &menu);
    static void asyncOutput(const std::string &message);
    static void asyncOutput(const std::string &prompt, const std::string &message);

private:
    static bool backMenu_;
    static std::atomic<int> shared_atomic_variable;
    static const Menu *storedMenu_;
    static struct linenoiseState linenoise_;

    MenuWrapper() = default;
    ~MenuWrapper() = default;
    MenuWrapper(const MenuWrapper&) = delete;
    MenuWrapper& operator=(const MenuWrapper&) = delete;

    static std::string asyncInput(const std::string &prompt);
    static void parseLine(const std::string &input, std::vector<std::string> &args);
    static void menuItemHelp(const std::shared_ptr<cmdline::parser> &parser);
    static void menuItemSwitch(const std::shared_ptr<cmdline::parser> &parser);
    static void menuItemBack(const std::shared_ptr<cmdline::parser> &parser);
    static void menuItemClear(const std::shared_ptr<cmdline::parser> &parser);
};

#define MENU_LOG MenuWrapper::getInstance().asyncOutput

} // namespace menu

#endif // MENU_H