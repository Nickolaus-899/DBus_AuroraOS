#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <memory>

#include "connect.hpp"
#include "printer.cpp"


using Config = std::map<std::string, sdbus::Variant>;


void await_signals(std::unique_ptr<sdbus::IProxy>& proxy, std::shared_ptr<sch_printer::Printer>& printer) {
    proxy->uponSignal(SIGNAL_NAMES::changed)
        .onInterface(INTERFACE_NAME)
        .call([printer](const Config& conf){
            if (debug_mode) std::cout << "Received a signal\n";

            uint32_t timeout;
            std::string phrase;

            try {
                timeout = conf.at(FIELD_NAMES::timeout).get<uint32_t>();
                phrase = conf.at(FIELD_NAMES::phrase).get<std::string>();
            } catch (sdbus::Error& e) {
                std::cerr << "Cannot extract field from the conf: " << e.what() << "\n";
                exit(EXIT_FAILURE);
            }

            printer->update(timeout, phrase);
        });

    // uponSignal as registration, finishRegistration() is necessary
    proxy->finishRegistration();
}

void read_conf(std::unique_ptr<sdbus::IProxy>& proxy, std::shared_ptr<sch_printer::Printer>& printer) {
    // item to store the response
    Config conf;
    
    try {
        // get cur configuration
        proxy->callMethod(METHOD_NAMES::Get)
            .onInterface(INTERFACE_NAME)
            .withArguments()
            .storeResultsTo(conf);

        if (debug_mode) std::cout << "Received conf\n";
    } catch(const sdbus::Error& e) {
        std::cerr << "Error while calling a method to server: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    uint32_t timeout;
    std::string phrase;

    try {
        timeout = conf.at(FIELD_NAMES::timeout).get<uint32_t>();
        phrase = conf.at(FIELD_NAMES::phrase).get<std::string>();
    } catch (sdbus::Error& e) {
        std::cerr << "Cannot extract field from the conf: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    if (debug_mode) std::cout << "Configuration was extracted and read: " << phrase << "\n";

    printer->update(timeout, phrase);
}
