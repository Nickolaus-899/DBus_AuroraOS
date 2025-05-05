#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <filesystem>
#include <sdbus-c++/sdbus-c++.h>
#include <map>
#include <variant>

using json = nlohmann::json;
using Config = std::map<std::string, sdbus::Variant>;

inline const std::string CONF_DIR =
    std::string(getenv("HOME")) + "/com.system.configurationManager/";

namespace conf_worker
{

void save_conf(Config &conf, const std::string &conf_path)
{
    auto path = CONF_DIR + conf_path + ".json";

    if (!std::filesystem::exists(path))
    {
        std::cerr << "File " << path << " doesn't exist (save)\n";
        exit(EXIT_FAILURE);
    }

    json data;

    for (auto &[key, value] : conf)
    {
        try
        {
            // uint
            try
            {
                uint32_t int_value = value.get<uint32_t>();
                data[key] = int_value;
                continue;
            }
            catch (const sdbus::Error &)
            {
            }

            // string
            try
            {
                std::string str_value = value.get<std::string>();
                data[key] = str_value;
                continue;
            }
            catch (const sdbus::Error &)
            {
            }

            std::cerr << "Unsupported type inside conf for key: " << key
                      << "\n";
            exit(EXIT_FAILURE);
        }
        catch (const sdbus::Error &e)
        {
            std::cerr << "D-Bus error: " << e.getMessage() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // write result (do not open earlier to prevent from clearing
    // the file in case of an error)
    std::ofstream out_conf(path);

    if (out_conf.is_open())
    {
        out_conf << data.dump(4); // 4 is number of spaces before an entry
        out_conf.close();
    }
    else
    {
        std::cerr << "Error while oppening the file (save) " << path
                  << std::endl;
        exit(EXIT_FAILURE);
    }
}

void load_conf(Config &conf, const std::string &conf_path)
{
    auto path = CONF_DIR + conf_path + ".json";

    if (!std::filesystem::exists(path))
    {
        std::cerr << "File " << path << " doesn't exist (load)\n";
        exit(EXIT_FAILURE);
    }

    std::ifstream inp_conf(path);
    json j_conf;

    if (inp_conf.is_open())
    {
        // reading to the json object
        inp_conf >> j_conf;
        inp_conf.close();
    }
    else
    {
        std::cerr << "Error while oppening the file (load) " << path
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // extracting info from the json
    for (auto &[key, value] : j_conf.items())
    {
        sdbus::Variant var;
        if (value.is_number_unsigned())
        {
            var = sdbus::Variant(value.get<uint>());
        }
        else if (value.is_string())
        {
            var = sdbus::Variant(value.get<std::string>());
        }
        else
        {
            std::cerr << "Unsupported type inside JSON conf\n";
            exit(EXIT_FAILURE);
        }

        conf[key] = var;
    }
}

} // namespace conf_worker
