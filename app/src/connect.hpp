#include <string>
#include <map>

constexpr const char *INTERFACE_NAME =
    "com.system.configurationManager.Application.Configuration";
constexpr const char *SERVICE_NAME = "com.system.configurationManager";

// print extra info like "signal received" or not
constexpr bool debug_mode = true;

struct METHOD_NAMES
{
    static constexpr const char *Change = "ChangeConfiguration";
    static constexpr const char *Get = "GetConfiguration";
};

struct SIGNAL_NAMES
{
    static constexpr const char *changed = "configurationChanged";
};

struct FIELD_NAMES
{
    static constexpr const char *timeout = "Timeout";
    static constexpr const char *phrase = "TimeoutPhrase";
};

inline auto create_object_name = [](const std::string &app_name) -> std::string
{
    const std::string APP_BASE_NAME =
        "/com/system/configurationManager/Application/";

    return APP_BASE_NAME + app_name;
};
