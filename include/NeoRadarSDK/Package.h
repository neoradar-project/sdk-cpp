#pragma once
#include <filesystem>

namespace PluginSDK::Package {

class PackageAPI {
public:
    virtual ~PackageAPI() = default;

    /**
     * @brief Get active package file path
     * @return Filesystem Path of the active package
     */
    virtual std::filesystem::path getPackagePath() = 0;
};

} // namespace PluginSDK::Aircraft
