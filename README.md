# NeoRadarSDK

A C++ SDK for developing NeoRadar plugins.

## Requirements

- C++20 or later
- CMake 3.15 or later
- [NeoRadar CLI (Plugin Packaging)](https://github.com/neoradar-project/cli)

## Usage

### Method 1: Git Submodule (Recommended)

Add this SDK as a git submodule to your project:

```bash
git submodule add https://github.com/neoradar-project/sdk-cpp.git third-party/NeoRadarSDK
git submodule update --init --recursive
```

In your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyPlugin)

# Add the SDK
add_subdirectory(third-party/NeoRadarSDK)

# Create your plugin
add_library(MyPlugin SHARED
    src/MyPlugin.cpp
    src/other_files.cpp
)

# Link against the SDK
target_link_libraries(MyPlugin
    PRIVATE
        NeoRadarSDK::NeoRadarSDK
)

# === Plugin Packaging ===
# Include the packaging module (provided by the SDK)
include(PluginPackager)

# One line = cross-platform packaging!
neoradar_package_plugin()
```

### Method 2: Installed SDK (find_package)

If you've installed the SDK system-wide:

```cmake
find_package(NeoRadarSDK REQUIRED)

add_library(MyPlugin SHARED src/MyPlugin.cpp)
target_link_libraries(MyPlugin PRIVATE NeoRadarSDK::NeoRadarSDK)

# Packaging tools are automatically available
neoradar_package_plugin()
```

## Basic Plugin Structure

```cpp
#include <NeoRadarSDK/SDK.h>

class MyPlugin : public PluginSDK::BasePlugin {
public:
    void Initialize(const PluginSDK::PluginMetadata& metadata,
                   PluginSDK::CoreAPI* coreAPI,
                   PluginSDK::ClientInformation info) override {
        // Initialize your plugin
    }

    void Shutdown() override {
        // Clean up resources
    }

    PluginSDK::PluginMetadata GetMetadata() const override {
        return {
            .name = "MyPlugin",
            .version = "1.0.0",
            .author = "Your Name"
        };
    }

    // Override event handlers as needed
    void OnAircraftConnected(const PluginSDK::Aircraft::AircraftConnectedEvent* event) override {
        // Handle aircraft connection
    }
};

// Required export
extern "C" {
PLUGIN_API PluginSDK::BasePlugin* CreatePluginInstance() {
    return new MyPlugin();
}
}
```
