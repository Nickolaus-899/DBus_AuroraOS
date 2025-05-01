#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <filesystem>
#include <string>

#include "service.cpp"


int main() {
    auto connection = sdbus::createSessionBusConnection();
    connection->requestName(SERVICE_NAME);

    // reading config files
    if (!std::filesystem::exists(CONF_DIR)) {
        std::cerr << "Config directory " << CONF_DIR << " doesn't exist\n";
        exit(EXIT_FAILURE);
    }

    for (const auto& entry : std::filesystem::directory_iterator(CONF_DIR)) {
        if (entry.path().extension() == ".json") {
            std::string app_name = entry.path().stem();
            std::string obj_name = create_object_name(app_name);

            try {
                auto object = sdbus::createObject(*connection, obj_name);
                do_registration(object);
            } catch (sdbus::Error& e) {
                std::cout << "Error while creating an object " << obj_name << ": " << e.what();
                std::cout << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }
    }

    // starting to accept requests
    std::cout << "Running service...\n";
    connection->enterEventLoop();
}
