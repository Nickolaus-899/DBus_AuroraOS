#include <iostream>
#include <string>
#include <chrono>
#include <thread>

namespace sch_printer
{

class Printer {
private:
    uint32_t timeout;
    std::string phrase;

    bool m_repeate = true;
public:
    explicit Printer(uint32_t t, std::string& p): timeout(t), phrase(p) {};
    explicit Printer(): timeout(1), phrase("Default phrase") {}
    ~Printer() = default;
    void print();
    void schedule();
    void update(uint32_t t, std::string& p);

    void stop();
};

void Printer::print() {
    std::cout << this->phrase << "\n";
}

void Printer::schedule() {
    // action
    this->print();

    // sleep
    std::this_thread::sleep_for(std::chrono::seconds(this->timeout));

    // repeate
    if(this->m_repeate) this->schedule();
}  

void Printer::update(uint32_t t, std::string& p) {
    this->timeout = t;
    this->phrase = p;
}


void Printer::stop() {
    this->m_repeate = false;
}

} // namespace sch_printer




