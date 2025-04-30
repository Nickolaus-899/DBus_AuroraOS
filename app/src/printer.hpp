#include <iostream>
#include <string>

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


inline const std::string CONF_PATH = std::string(getenv("HOME")) + "/com.system.configurationManager/confManagerApplication1.json";
constexpr bool debug_mode = false;
