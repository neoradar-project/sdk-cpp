cmake_minimum_required(VERSION 3.15)

project(NeoRadarSDK 
    VERSION 1.0.1
    DESCRIPTION "NeoRadar Plugin SDK"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NEORADAR_SDK_BUILD_EXAMPLES "Build examples" OFF)
option(NEORADAR_SDK_INSTALL "Install NeoRadarSDK" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(PluginPackager)

add_library(NeoRadarSDK STATIC
    src/SDK.cpp
)

add_library(NeoRadarSDK::NeoRadarSDK ALIAS NeoRadarSDK)

target_include_directories(NeoRadarSDK
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(NeoRadarSDK 
    PUBLIC 
        cxx_std_17
)

if(WIN32)
    target_compile_definitions(NeoRadarSDK PUBLIC WIN32_LEAN_AND_MEAN)
endif()

if(APPLE)
    set_target_properties(NeoRadarSDK PROPERTIES
        INTERFACE_LINK_OPTIONS "-Wl,-u,_GetPluginSDKVersionMajor;-Wl,-u,_GetPluginSDKVersionMinor;-Wl,-u,_GetPluginSDKVersionPatch"
    )
elseif(UNIX)
    set_target_properties(NeoRadarSDK PROPERTIES
        INTERFACE_LINK_OPTIONS "-Wl,--undefined=GetPluginSDKVersionMajor;-Wl,--undefined=GetPluginSDKVersionMinor;-Wl,--undefined=GetPluginSDKVersionPatch"
    )
elseif(WIN32)
    set_target_properties(NeoRadarSDK PROPERTIES
        INTERFACE_LINK_OPTIONS "/INCLUDE:GetPluginSDKVersionMajor;/INCLUDE:GetPluginSDKVersionMinor;/INCLUDE:GetPluginSDKVersionPatch"
    )
endif()

target_compile_definitions(NeoRadarSDK PRIVATE
    PLUGIN_SDK_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PLUGIN_SDK_VERSION_MINOR=${PROJECT_VERSION_MINOR} 
    PLUGIN_SDK_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

if(NEORADAR_SDK_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h"
    )

    install(FILES cmake/PluginPackager.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NeoRadarSDK
    )

    install(TARGETS NeoRadarSDK
        EXPORT NeoRadarSDKTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(EXPORT NeoRadarSDKTargets
        FILE NeoRadarSDKTargets.cmake
        NAMESPACE NeoRadarSDK::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NeoRadarSDK
    )

    configure_package_config_file(
        cmake/NeoRadarSDKConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/NeoRadarSDKConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NeoRadarSDK
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/NeoRadarSDKConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NeoRadarSDKConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/NeoRadarSDKConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NeoRadarSDK
    )
endif()

if(NEORADAR_SDK_BUILD_EXAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
    include(PluginPackager)
    add_subdirectory(examples)
endif()

export(EXPORT NeoRadarSDKTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/NeoRadarSDKTargets.cmake
    NAMESPACE NeoRadarSDK::
)