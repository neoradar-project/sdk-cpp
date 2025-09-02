# PluginPackager.cmake - NeoRadarSDK Plugin Packaging Utilities
# Reusable CMake module for cross-platform plugin packaging and distribution.

function(setup_plugin_packaging TARGET_NAME)
    set(OUTPUT_DIR "release")
    if(ARGC GREATER 1)
        set(OUTPUT_DIR "${ARGV1}")
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(OS_NAME "windows")
        set(LIB_EXTENSION "dll")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(OS_NAME "macos")
        set(LIB_EXTENSION "dylib")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OS_NAME "linux")
        set(LIB_EXTENSION "so")
    else()
        set(OS_NAME "unknown")
        set(LIB_EXTENSION "so")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(ARCH_NAME "x64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686")
        set(ARCH_NAME "x86")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(ARCH_NAME "arm64")
    else()
        set(ARCH_NAME "${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set(TARGET_FILENAME "${OS_NAME}-${ARCH_NAME}.${LIB_EXTENSION}")

    message(STATUS "NeoRadarSDK Plugin Packaging - OS: ${OS_NAME}, Architecture: ${ARCH_NAME}")
    message(STATUS "NeoRadarSDK Plugin Packaging - Target filename: ${TARGET_FILENAME}")

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR}/${TARGET_NAME}"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${TARGET_NAME}>" "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR}/${TARGET_NAME}/${TARGET_FILENAME}"
        COMMENT "Copying ${TARGET_NAME} to ${OUTPUT_DIR}/${TARGET_NAME}/${TARGET_FILENAME}"
    )
endfunction()

function(setup_neoradar_packaging TARGET_NAME)
    set(OUTPUT_DIR "release")
    if(ARGC GREATER 1)
        set(OUTPUT_DIR "${ARGV1}")
    endif()

    find_program(NEORADAR_CLI neoradar-cli)
    if(NOT NEORADAR_CLI)
        message(WARNING "neoradar-cli not found. Plugin archives will not be created automatically.")
        message(STATUS "To install neoradar-cli: npm install -g @neoradar/cli")
        return()
    endif()

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${NEORADAR_CLI} create-plugin-archives "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR}/" -o "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR}/" --no-confirmation
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Creating plugin archives with neoradar-cli"
    )
endfunction()

function(setup_complete_plugin_packaging TARGET_NAME)
    set(OUTPUT_DIR "release")
    if(ARGC GREATER 1)
        set(OUTPUT_DIR "${ARGV1}")
    endif()

    setup_plugin_packaging(${TARGET_NAME} ${OUTPUT_DIR})
    setup_neoradar_packaging(${TARGET_NAME} ${OUTPUT_DIR})
endfunction()

function(neoradar_package_plugin)
    set(options SKIP_ARCHIVES)
    set(oneValueArgs OUTPUT_DIR)
    set(multiValueArgs "")
    
    cmake_parse_arguments(NPP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if(NOT NPP_OUTPUT_DIR)
        set(NPP_OUTPUT_DIR "release")
    endif()
    
    set(TARGET_NAME ${PROJECT_NAME})
    
    if(NPP_SKIP_ARCHIVES)
        setup_plugin_packaging(${TARGET_NAME} ${NPP_OUTPUT_DIR})
        message(STATUS "NeoRadarSDK: Plugin packaging configured for ${TARGET_NAME} (archives skipped)")
    else()
        setup_complete_plugin_packaging(${TARGET_NAME} ${NPP_OUTPUT_DIR})
        message(STATUS "NeoRadarSDK: Complete plugin packaging configured for ${TARGET_NAME}")
    endif()
endfunction()