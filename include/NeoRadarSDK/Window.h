#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace PluginSDK::Window {

using WindowHandle = uint64_t;
using ElementHandle = uint64_t;
constexpr WindowHandle InvalidWindow = 0;

/// Element types for declarative plugin windows.
enum class ElementType {
    Panel = 0,        ///< Container (vertical/horizontal StackPanel)
    Label = 1,        ///< Single-line text
    Button = 2,       ///< Clickable button (fires "click" event)
    TextInput = 3,    ///< Text entry (fires "change"/"submit" events)
    Checkbox = 4,     ///< Boolean toggle with label
    Divider = 5,      ///< Horizontal separator line
    Spacer = 6,       ///< Fixed-size empty gap
    Group = 7,        ///< Container with header label

    Heading = 10,     ///< Section heading (14pt, SemiBold by default)
    Paragraph = 11,   ///< Multi-line wrapped text
    Select = 12,      ///< Dropdown/ComboBox (fires "change" event)
    Toggle = 13,      ///< On/off switch
    Radio = 14,       ///< Radio button (exclusive within parent Group)
    Slider = 15,      ///< Range slider (fires "change" event)
    NumberInput = 16, ///< Numeric entry with spin buttons
    ProgressBar = 17, ///< Progress indicator
    Badge = 18,       ///< Small colored tag/label
    Tabs = 19,        ///< Tab container (children must be TabPage)
    TabPage = 20,     ///< Single tab page (child of Tabs)
    ScrollArea = 21,  ///< Scrollable container
    Grid = 22,        ///< CSS-grid-like 2D layout
    Image = 23,       ///< Static image display
};

/// Font weight values (maps to Avalonia FontWeight).
enum class FontWeight : uint8_t {
    Default = 0,
    Thin = 1,
    Light = 2,
    Regular = 3,
    Medium = 4,
    SemiBold = 5,
    Bold = 6,
    ExtraBold = 7
};

/// Horizontal alignment.
enum class HAlign : uint8_t {
    Stretch = 0, Left = 1, Center = 2, Right = 3
};

/// Vertical alignment.
enum class VAlign : uint8_t {
    Stretch = 0, Top = 1, Center = 2, Bottom = 3
};

/// Button visual variant.
enum class ButtonVariant : uint8_t {
    Default = 0,  ///< Neutral (Toolbar.ItemBg / FG1)
    Success = 1,  ///< Green (Save/Confirm)
    Danger = 2,   ///< Red (Delete/Disconnect)
    Accent = 3,   ///< Blue (Primary action)
    Ghost = 4     ///< Transparent background
};

/// Style properties for a window element. All fields are optional;
/// zero/empty = use the control's default.
struct ElementStyle {
    // Colors (RGB, 0,0,0 = use default theme color)
    std::optional<std::array<uint8_t, 3>> backgroundColor;
    std::optional<uint8_t> backgroundAlpha;
    std::optional<std::array<uint8_t, 3>> textColor;

    // Typography
    std::optional<int> fontSize;
    FontWeight fontWeight = FontWeight::Default;

    // Dimensions
    std::optional<int> width;
    std::optional<int> height;
    std::optional<int> minWidth;
    std::optional<int> minHeight;
    std::optional<int> maxWidth;
    std::optional<int> maxHeight;

    // Spacing
    std::optional<int> padding;
    std::optional<int> paddingH;
    std::optional<int> paddingV;
    std::optional<int> margin;
    std::optional<int> marginH;
    std::optional<int> marginV;
    std::optional<int> spacing;

    // Border
    std::optional<std::array<uint8_t, 3>> borderColor;
    std::optional<uint8_t> borderWidth;
    std::optional<uint8_t> borderRadius;
    std::optional<uint8_t> opacity; ///< 0 = fully opaque, 1-255 = opacity/255

    // Theme resource keys (override RGB if set)
    std::string themeBg;   ///< e.g. "Brush.Surface.Deepest"
    std::string themeFg;   ///< e.g. "Brush.FG1"

    // Button
    ButtonVariant variant = ButtonVariant::Default;
};

/// A single UI element in the declarative window tree.
struct WindowElement {
    std::string id;
    ElementType type = ElementType::Label;
    std::string text;
    std::string data;     ///< Freeform plugin data (placeholder for TextInput, value for Radio, etc.)
    ElementStyle style;
    std::vector<WindowElement> children;

    // Layout
    uint8_t direction = 0; ///< 0=vertical, 1=horizontal
    HAlign hAlign = HAlign::Stretch;
    VAlign vAlign = VAlign::Stretch;
    bool wrap = false;

    // Grid placement (for children of Grid containers)
    int col = 0, row = 0;
    int colSpan = 0, rowSpan = 0;

    // Grid definitions (for Grid containers)
    std::string columns;   ///< "1fr,auto,200" or "3" (repeat)
    std::string rows;

    // State
    bool enabled = true;
    bool visible = true;
    bool isChecked = false;
    bool isPassword = false;

    // Select/Dropdown
    std::string options;    ///< Pipe-delimited: "Option A|Option B|Option C"
    int selectedIndex = -1;

    // Slider/Number/Progress
    double minValue = 0;
    double maxValue = 0;    ///< 0 = use default (100 for slider)
    double step = 0;        ///< 0 = use default (1)
    double value = 0;
    double progress = 0;    ///< 0.0-1.0 for determinate, <0 for indeterminate

    // Tooltip
    std::string tooltip;
};

/// Window definition passed to OpenWindow.
struct WindowDefinition {
    std::string title;
    int width = 300;
    int height = 200;
    std::string icon;          ///< Lucide icon name (e.g. "Puzzle", "Settings")
    bool showInList = false;   ///< If true, window appears in the Lists dropdown so users can reopen it
    std::vector<WindowElement> elements;
};

/// Event fired back to the plugin when the user interacts with a window element.
struct WindowEvent {
    WindowHandle window;
    std::string elementId;
    std::string eventType; ///< "click", "change", "submit", "close"
    std::string value;
};

/**
 * @interface WindowInterface
 * @brief Declarative window management for plugins.
 *
 * Windows are materialised as native OS windows (ToolWindow) by the C# host.
 * Plugins describe the UI tree; the host renders it with theme-aware Avalonia
 * controls. Events (button clicks, text changes, close) flow back via
 * BasePlugin::OnWindowEvent.
 */
class WindowInterface {
public:
    virtual ~WindowInterface() = default;

    /// Create a new window from a declarative definition. Returns a handle.
    virtual WindowHandle OpenWindow(const WindowDefinition& def) = 0;

    /// Update an element's text/value by ID.
    virtual bool UpdateElement(WindowHandle window, const std::string& elementId,
                               const std::string& text) = 0;

    /// Close and destroy a window.
    virtual bool CloseWindow(WindowHandle window) = 0;
};

class WindowAPI {
public:
    virtual ~WindowAPI() = default;
    virtual WindowInterface* getInterface() = 0;
};

} // namespace PluginSDK::Window
