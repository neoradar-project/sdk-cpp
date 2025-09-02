cmake_minimum_required(VERSION 3.15)
project(ExamplePlugin)


add_library(ExamplePlugin SHARED
    main.cpp
)

target_link_libraries(ExamplePlugin 
    PRIVATE 
        NeoRadarSDK::NeoRadarSDK
)

target_compile_features(ExamplePlugin PRIVATE cxx_std_17)

include(PluginPackaging)

neoradar_package_plugin()
