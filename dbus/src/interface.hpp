#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <functional>


constexpr const char* INTERFACE_NAME = "com.system.configurationManager.Application.Configuration";
constexpr const char* SERVICE_NAME = "com.system.configurationManager";

constexpr bool debug_mode = true;


// void ChangeConfiguration(std::string key, sdbus::Variant value);
std::function<void(std::string, sdbus::Variant)> ChangeConfWrapper(const std::string& app_path);

// std::map<std::string, sdbus::Variant> GetConfiguration();
std::function<std::map<std::string, sdbus::Variant>()> GetConfWrapper(const std::string& app_path);

std::map<const std::string, const std::string> METHODS_NAMES {
    {"change", "ChangeConfiguration"},
    {"get", "GetConfiguration"}
};

std::map<const std::string, const std::string> SIGNALS_NAMES {
    {"changed", "configurationChanged"}
};


inline auto create_object_name = [](const std::string& app_name)->std::string {
    const std::string APP_BASE_NAME = "/com/system/configurationManager/Application/";

    return APP_BASE_NAME + app_name;
};
