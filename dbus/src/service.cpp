#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <functional>

#include "interface.hpp"
#include "conf_worker.cpp"

// does /com/syst.../AppName -> AppName
std::string extract_app_name(const std::string &obj_name)
{
    constexpr char delimiter = '/';

    size_t last_dot = obj_name.find_last_of(delimiter);
    if (last_dot == std::string::npos || last_dot == obj_name.length() - 1)
    {
        // either no dots or ends with a dot
        throw sdbus::Error("Object name error",
                           "Invalid object name " + obj_name);
    }

    return obj_name.substr(last_dot + 1);
}

// returns the function:
// void ChangeConfiguration(std::string key, sdbus::Variant value)
std::function<void(std::string, sdbus::Variant)>
ChangeConfWrapper(std::unique_ptr<sdbus::IObject> &object)
{

    const std::string app_path = object->getObjectPath();
    const std::string app_name = extract_app_name(app_path);

    // this derefferencing is needed to prevent pointer from beeing cleaned up
    // and we can't catch it by value (copying) it since it is a unique_ptr
    sdbus::IObject *object_ptr = object.get();

    return [app_name, object_ptr](std::string key, sdbus::Variant value)
    {
        if (debug_mode)
            std::cout << "Call ChangeConfiguration of " << key << "\n";

        std::map<std::string, sdbus::Variant> config;
        conf_worker::load_conf(config, app_name);

        bool field_exists_ = true;

        if (config.find(key) == config.end())
        {
            std::cout << "Warning: creating new field " << key << std::endl;
            field_exists_ = false;
        }

        if (INT_IS_UINT)
        {
            // let's treat int as uint if possible

            try
            {
                int32_t int_val = value.get<int32_t>();
                if (int_val >= 0)
                {
                    // convert to uint32_t
                    value = sdbus::Variant(static_cast<uint32_t>(int_val));
                    if (debug_mode)
                        std::cout
                            << "Converted int32 to uint32 for key: " << key
                            << "\n";
                }
                else
                {
                    throw sdbus::Error("Invalid field " + key,
                                       "Timeout cannot be negative");
                }
            }
            catch (const sdbus::Error &)
            {
            }
        }

        // if type in an existing
        if (field_exists_ &&
            config[key].peekValueType() != value.peekValueType())
        {
            std::string err_msg = "Expected " + config[key].peekValueType() +
                                  " but got " + value.peekValueType() +
                                  " for key " + key + "\n";

            if (debug_mode)
                std::cerr << err_msg;

            throw sdbus::Error("Type mismatch", err_msg);
        }

        config[key] = value;
        conf_worker::save_conf(config, app_name);

        // send a signal to client with updated config
        try
        {
            object_ptr->emitSignal(SIGNAL_NAMES::changed)
                .onInterface(INTERFACE_NAME)
                .withArguments(config);
        }
        catch (const sdbus::Error &e)
        {
            throw sdbus::Error("Singlal emittion failed", e.what());
        }

        if (debug_mode)
            std::cout << "Signal was emitted\n";
    };
}

// returns the function:
// std::map<std::string, sdbus::Variant> GetConfiguration()
std::function<std::map<std::string, sdbus::Variant>()>
GetConfWrapper(std::unique_ptr<sdbus::IObject> &object)
{

    const std::string app_path = object->getObjectPath();
    const std::string app_name = extract_app_name(app_path);

    // Object is actually not needed here (only name)
    // but I pass it to keep the same function signature
    sdbus::IObject *object_ptr = object.get();

    return [app_name, object_ptr]()
    {
        if (debug_mode)
            std::cout << "Call GetConfiguration\n";

        std::map<std::string, sdbus::Variant> config;
        conf_worker::load_conf(config, app_name);
        return config;
    };
}

void do_registration(std::unique_ptr<sdbus::IObject> &object)
{
    auto app_path = object->getObjectPath();
    if (debug_mode)
        std::cout << "Registering " << app_path << "...\n";

    // registering the method ChangeConfiguration
    object->registerMethod(METHOD_NAMES::Change)
        .onInterface(INTERFACE_NAME)
        .implementedAs(ChangeConfWrapper(object));

    // registering the method GetConfiguration
    object->registerMethod(METHOD_NAMES::Get)
        .onInterface(INTERFACE_NAME)
        .implementedAs(GetConfWrapper(object));

    // registering the signal configurationChanged
    object->registerSignal(SIGNAL_NAMES::changed)
        .onInterface(INTERFACE_NAME)
        .withParameters<std::map<std::string, sdbus::Variant>>();

    object->finishRegistration();

    if (debug_mode)
        std::cout << "Registration was comleted\n\n";
}
