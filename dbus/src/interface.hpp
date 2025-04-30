#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <string>


constexpr const char* INTERFACE_NAME = "com.system.configurationManager.Application.Configuration";

class ConfInterface: public sdbus::AdaptorInterfaces<sdbus::ObjectManager_adaptor> {
private:
    std::string m_name;
public:
    ConfInterface(sdbus::IConnection& connection, const std::string& objectPath)
    : AdaptorInterfaces(connection, std::move(objectPath)), m_name(::INTERFACE_NAME) {
        registerAdaptor();
    }
    ~ConfInterface() {
        unregisterAdaptor();
    }

    virtual void ChangeConfiguration(const std::string& key, const sdbus::Variant& value) = 0;
    virtual std::map<std::string, sdbus::Variant> GetConfiguration() = 0;

    void emitConfigurationChanged(const std::map<std::string, sdbus::Variant>& newConfig) {
        emitSignal("configurationChanged")
            .onInterface(this->m_name)
            .withArguments(newConfig);
    }
};