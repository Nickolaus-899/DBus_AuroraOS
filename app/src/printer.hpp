#include <iostream>


class Printer {
private:
    uint timeout;
    std::string phrase;
public:
    explicit Printer(uint t, std::string& p): timeout(t), phrase(p) {};
    void print();
    void schedule();
};

std::pair<uint, std::string> read_conf();


constexpr const char CONF_PATH[] = "/home/nickolaus-sdr/com.system.configurationManager/confManagerApplication1.json";
constexpr bool debug_mode = false;
