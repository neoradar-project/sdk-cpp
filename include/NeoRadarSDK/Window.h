#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace PluginSDK::Window {

using WindowHandle = uint64_t;
using ElementHandle = uint64_t;
constexpr WindowHandle InvalidWindow = 0;

enum class ElementType {
    Panel = 0,
    Label = 1,
    Button = 2,
    TextInput = 3,
    Checkbox = 4,
    Divider = 5,
    Spacer = 6,
    Group = 7
};

struct ElementStyle {
    std::optional<std::array<uint8_t, 3>> backgroundColor;
    std::optional<std::array<uint8_t, 3>> textColor;
    std::optional<int> fontSize;
    std::optional<std::string> fontWeight;
    std::optional<int> width;
    std::optional<int> height;
    std::optional<int> padding;
    std::optional<int> spacing;
};

struct WindowElement {
    std::string id;
    ElementType type = ElementType::Label;
    std::string text;
    std::string data;
    ElementStyle style;
    std::vector<WindowElement> children;
};

struct WindowDefinition {
    std::string title;
    int width = 300;
    int height = 200;
    std::string icon; // Lucide icon name
    std::vector<WindowElement> elements;
};

struct WindowEvent {
    WindowHandle window;
    std::string elementId;
    std::string eventType; // "click", "change", "close"
    std::string value;
};

/**
 * @interface WindowInterface
 * @brief Declarative window management for plugins.
 *
 * Windows are materialised as native OS windows by the C# host.
 * Plugins describe the UI tree; the host renders it with theme-aware
 * Avalonia controls. Events (button clicks, text changes, close)
 * flow back via BasePlugin::OnWindowEvent.
 */
class WindowInterface {
public:
    virtual ~WindowInterface() = default;
    virtual WindowHandle CreateWindow(const WindowDefinition& def) = 0;
    virtual bool UpdateElement(WindowHandle window, const std::string& elementId,
                               const std::string& text) = 0;
    virtual bool CloseWindow(WindowHandle window) = 0;
};

class WindowAPI {
public:
    virtual ~WindowAPI() = default;
    virtual WindowInterface* getInterface() = 0;
};

} // namespace PluginSDK::Window
