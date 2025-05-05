#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <functional>

/*
Here, constexpr is used to share all these entities
not as variables but as constants to decrease memory
usage for allocation

I used wrappers for both methods to create
corresponding lambdas that catch an object
to address the app
*/

constexpr const char *INTERFACE_NAME =
    "com.system.configurationManager.Application.Configuration";
constexpr const char *SERVICE_NAME = "com.system.configurationManager";

// print extra info like "signal was emitted" or not
constexpr bool debug_mode = true;

// treat int32 as uint32 if conversion is possible or not
constexpr bool INT_IS_UINT = true;

// void ChangeConfiguration(std::string key, sdbus::Variant value);
std::function<void(std::string, sdbus::Variant)>
ChangeConfWrapper(std::unique_ptr<sdbus::IObject> &object);

// std::map<std::string, sdbus::Variant> GetConfiguration();
std::function<std::map<std::string, sdbus::Variant>()>
GetConfWrapper(std::unique_ptr<sdbus::IObject> &object);

struct METHOD_NAMES
{
    static constexpr const char *Change = "ChangeConfiguration";
    static constexpr const char *Get = "GetConfiguration";
};

struct SIGNAL_NAMES
{
    static constexpr const char *changed = "configurationChanged";
};

inline auto create_object_name = [](const std::string &app_name) -> std::string
{
    const std::string APP_BASE_NAME =
        "/com/system/configurationManager/Application/";

    return APP_BASE_NAME + app_name;
};
