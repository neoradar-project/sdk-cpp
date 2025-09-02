#include "NeoRadarSDK/SDK.h"

extern "C" {
    PLUGIN_API int GetPluginSDKVersionMajor() { 
        return PLUGIN_SDK_VERSION_MAJOR; 
    }
    
    PLUGIN_API int GetPluginSDKVersionMinor() { 
        return PLUGIN_SDK_VERSION_MINOR; 
    }
    
    PLUGIN_API int GetPluginSDKVersionPatch() { 
        return PLUGIN_SDK_VERSION_PATCH; 
    }
}