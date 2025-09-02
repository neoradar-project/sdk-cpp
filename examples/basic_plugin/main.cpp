#include <NeoRadarSDK/SDK.h>
#include <iostream>

class ExamplePlugin : public PluginSDK::BasePlugin {
public:
    void Initialize(const PluginSDK::PluginMetadata& metadata, 
                   PluginSDK::CoreAPI* coreAPI, 
                   PluginSDK::ClientInformation info) override {
        m_coreAPI = coreAPI;
        
        m_coreAPI->logger().info("ExamplePlugin initialized");
        m_coreAPI->logger().info("Client: " + info.clientName + " v" + info.clientVersion);
    }
    
    void Shutdown() override {
        if (m_coreAPI) {
            m_coreAPI->logger().info("ExamplePlugin shutting down");
        }
    }
    
    PluginSDK::PluginMetadata GetMetadata() const override {
        return {
            .name = "ExamplePlugin",
            .version = "1.0.0",
            .author = "Example Author"
        };
    }
    
    void OnAircraftConnected(const PluginSDK::Aircraft::AircraftConnectedEvent* event) override {
        if (m_coreAPI) {
            m_coreAPI->logger().info("Aircraft connected: " + event->callsign);
        }
    }
    
    void OnPrivateMessageReceived(const PluginSDK::Chat::PrivateMessageReceivedEvent* event) override {
        if (m_coreAPI) {
            m_coreAPI->logger().info("Private message from " + event->sentFrom + ": " + event->message);
        }
    }

private:
    PluginSDK::CoreAPI* m_coreAPI = nullptr;
};

extern "C" {
PLUGIN_API PluginSDK::BasePlugin* CreatePluginInstance() {
    return new ExamplePlugin();
}
}