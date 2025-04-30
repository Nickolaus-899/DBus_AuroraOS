#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>
#include <thread>

#include "printer.hpp"

using json = nlohmann::json;

void Printer::print() {
    std::cout << this->phrase << "\n";
}

void Printer::schedule() {
    this->print();
    std::this_thread::sleep_for(std::chrono::seconds(this->timeout));

    auto conf = read_conf();

    this->timeout = conf.first;
    this->phrase = conf.second;

    this->schedule();
}

std::pair<uint, std::string> read_conf() {
    std::ifstream inp_conf(CONF_PATH);
    json conf;

    if (inp_conf.is_open()) {
        inp_conf >> conf;
        inp_conf.close();
    } else {
        std::cerr << "Error while oppening the file " << CONF_PATH << std::endl;
        exit(EXIT_FAILURE);
    }

    if (debug_mode) {
        std::cout << "Conf file was read successfully\n";
    }

    uint t = conf["Timeout"];
    std::string p = conf["TimeoutPhrase"];

    return std::pair(t, p);
}


int main() {
    auto conf = read_conf();
    auto printer = Printer(conf.first, conf.second);

    std::thread t(&Printer::schedule, &printer);
    t.join();
    
    return EXIT_SUCCESS;
}