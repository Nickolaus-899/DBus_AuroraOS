#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <functional>


constexpr const char* INTERFACE_NAME = "com.system.configurationManager.Application.Configuration";
constexpr const char* SERVICE_NAME = "com.system.configurationManager";

constexpr bool debug_mode = true;
constexpr bool INT_IS_UINT = true;


// void ChangeConfiguration(std::string key, sdbus::Variant value);
std::function<void(std::string, sdbus::Variant)> ChangeConfWrapper(const std::string& app_path);

// std::map<std::string, sdbus::Variant> GetConfiguration();
std::function<std::map<std::string, sdbus::Variant>()> GetConfWrapper(const std::string& app_path);

struct METHOD_NAMES {
    static constexpr const char* Change = "ChangeConfiguration";
    static constexpr const char* Get    = "GetConfiguration";
};

struct SIGNAL_NAMES {
    static constexpr const char* changed = "configurationChanged";
};


inline auto create_object_name = [](const std::string& app_name)->std::string {
    const std::string APP_BASE_NAME = "/com/system/configurationManager/Application/";

    return APP_BASE_NAME + app_name;
};
