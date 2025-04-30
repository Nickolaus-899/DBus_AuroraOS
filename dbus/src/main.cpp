#include <iostream>
#include <string>
#include <sdbus-c++/sdbus-c++.h>
#include <fstream>
#include <vector>

#include "conf_worker.cpp"


int main(int argc, char *argv[]) {
    auto connection = sdbus::createSessionBusConnection();
    auto concatenator = sdbus::createObject(*connection, "/org/sdbuscpp/concatenator");

    auto concatenate = [&concatenator](const std::vector<int> numbers, const std::string& separator){
        // Return error if there are no numbers in the collection
        if (numbers.empty())
            throw sdbus::Error("org.sdbuscpp.Concatenator.Error", "No numbers provided");

        std::string result;
        for (auto number : numbers) {
            result += (result.empty() ? std::string() : separator) + std::to_string(number);
        }

        // Emit 'concatenated' signal
        concatenator->emitSignal("concatenated").onInterface("org.sdbuscpp.Concatenator").withArguments(result);

        return result;
    };

    concatenator->registerMethod("concatenate").onInterface("org.sdbuscpp.Concatenator").implementedAs(concatenate);
    concatenator->registerSignal("concatenated").onInterface("org.sdbuscpp.Concatenator").withParameters<std::string>();

    concatenator->finishRegistration();

    connection->requestName("org.sdbuscpp.concatenator");


    std::cout << "Running service...\n";

    connection->enterEventLoop();

    // return EXIT_SUCCESS;
}