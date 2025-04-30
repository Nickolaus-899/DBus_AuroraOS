#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

void onConcatenated(const std::string& concatenatedString) {
    std::cout << "Received signal with concatenated string " << concatenatedString << std::endl;
}

int main(int argc, char *argv[]) {
    auto connection = sdbus::createSessionBusConnection();
    auto proxy = sdbus::createProxy(*connection, "org.sdbuscpp.concatenator", "/org/sdbuscpp/concatenator");
    proxy->uponSignal("concatenated").onInterface("org.sdbuscpp.Concatenator").call([](const std::string& str){ onConcatenated(str); });

    std::vector<int> numbers = {1, 2, 3};
    std::string separator = ":";

    {
        std::string concatenatedString;
        proxy->callMethod("concatenate")
            .onInterface("org.sdbuscpp.Concatenator")
            .withArguments(numbers, separator)
            .storeResultsTo(concatenatedString);

        std::cout << "before assert: " << concatenatedString << "\n";
        assert(concatenatedString == "1:2:3");
    }

    {
        try
        {
            proxy->callMethod("concatenate").onInterface("org.sdbuscpp.Concatenator").withArguments(std::vector<int>(), separator);
            assert(false);
        }
        catch(const sdbus::Error& e)
        {
            std::cerr << "(it's ok) Got concatenate error " << e.getName() << " with message " << e.getMessage() << std::endl;
        }
    }

    sleep(1);

    return EXIT_SUCCESS;
}
