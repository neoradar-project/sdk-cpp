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

## Plugin Packaging

The SDK includes integrated **cross-platform plugin packaging tools** that automatically:

- Detect your OS and architecture
- Create standardized binary names (`os-arch.extension`)
- Generate `.plugin` archives with `neoradar-cli`

### Packaging Options

```cmake
# Option 1: One-line setup (recommended)
neoradar_package_plugin()

# Option 2: Custom output directory
neoradar_package_plugin(OUTPUT_DIR "dist")

# Option 3: Skip automatic archive creation
neoradar_package_plugin(SKIP_ARCHIVES)

# Option 4: Manual control (advanced)
setup_complete_plugin_packaging(${PROJECT_NAME})
setup_plugin_packaging(${PROJECT_NAME} "custom_dir")
```

### Output Structure

After building, you get standardized cross-platform binaries:

```
release/
└── YourPlugin/
    ├── windows-x64.dll     # Windows 64-bit
    ├── macos-arm64.dylib   # macOS Apple Silicon
    ├── linux-x64.so       # Linux 64-bit
    └── YourPlugin.plugin   # Ready-to-distribute archive
```

### Supported Platforms

| OS      | Architecture | Output Example      |
| ------- | ------------ | ------------------- |
| Windows | x64/x86      | `windows-x64.dll`   |
| macOS   | x64/arm64    | `macos-arm64.dylib` |
| Linux   | x64/arm64    | `linux-x64.so`      |

- **Aircraft API**: Access aircraft data and position updates
- **Airport API**: Airport configurations and status
- **Chat API**: Text messaging and command registration
- **Controller API**: Controller information and events
- **Flightplan API**: Flight plan data and routing
- **Logger API**: Structured logging
- **Squawk API**: Transponder code management
- **Tag API**: Radar tag customization

See the individual header files in `include/NeoRadarSDK/` for detailed API documentation.

## Version Compatibility

The SDK exports version information that can be checked at runtime to ensure compatibility between plugins and the host application.
