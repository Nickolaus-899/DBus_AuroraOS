#include <iostream>
#include <string>
#include <sdbus-c++/sdbus-c++.h>
#include <memory>

#include "proxy.cpp"


constexpr const char* APP_NAME = "confManagerApplication1";

int main() {
    // to connect to the service
    auto connection = sdbus::createSessionBusConnection();
    auto proxy = sdbus::createProxy(*connection, SERVICE_NAME, create_object_name(APP_NAME));

    // to print message
    auto printer = std::make_shared<sch_printer::Printer>();

    await_signals(proxy, printer);
    read_conf(proxy, printer);

    std::cout << "\nRunning client...\n";
    std::cout << "Service name to address: " << SERVICE_NAME << "\n";

    std::thread t(&sch_printer::Printer::schedule, printer);

    connection->enterEventLoop();
    t.join();
    
    return EXIT_SUCCESS;
}   