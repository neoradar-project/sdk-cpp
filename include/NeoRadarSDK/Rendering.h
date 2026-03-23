#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace PluginSDK::Rendering {

using ObjectHandle = uint64_t;
constexpr ObjectHandle InvalidHandle = 0;

enum class CoordinateSpace { World = 0, Screen = 1 };

struct Color {
    uint8_t r = 255, g = 255, b = 255, a = 255;
};

/// Point in EPSG:3857 metres (World) or screen pixels (Screen).
struct Point2D {
    double x = 0, y = 0;
};

struct ViewportInfo {
    double centerX, centerY; // EPSG:3857
    double zoom;
    double widthPixels, heightPixels;
    double pixelsPerMeter;
};

struct RenderObjectEvent {
    ObjectHandle handle;
    double worldX, worldY;
    double screenX, screenY;
    int button; // 0=left, 1=middle, 2=right
};

// ── Draw parameter structs ───────────────────────────────────────────

struct DrawLineParams {
    std::vector<Point2D> points;
    Color color;
    float width = 1.4f;
    CoordinateSpace space = CoordinateSpace::World;
    float minZoom = 0.0f;
    bool hitTestable = false;
};

struct DrawFillParams {
    std::vector<Point2D> ring;
    Color color;
    CoordinateSpace space = CoordinateSpace::World;
    float minZoom = 0.0f;
    bool hitTestable = false;
};

struct DrawTextParams {
    Point2D position;
    std::string text;
    Color color;
    float fontSize = 12.0f;
    CoordinateSpace space = CoordinateSpace::World;
    float minZoom = 0.0f;
};

struct DrawPointParams {
    std::vector<Point2D> positions;
    Color color;
    float size = 4.0f;
    std::string symbol; // "diamond", "triangle", "square", "circle"
    CoordinateSpace space = CoordinateSpace::World;
    float minZoom = 0.0f;
    bool hitTestable = false;
};

// ── Rendering interface ──────────────────────────────────────────────

/**
 * @interface RenderingInterface
 * @brief Retained-mode drawing API for the radar map surface.
 *
 * All draw calls return a stable handle. Use the handle to mutate
 * visibility, colour, or remove the object. Coordinates are
 * EPSG:3857 metres (World) or screen pixels (Screen).
 *
 * Draw commands are batched internally and flushed to the C#
 * renderer at 60 Hz — individual calls are cheap.
 */
class RenderingInterface {
public:
    virtual ~RenderingInterface() = default;

    virtual ObjectHandle DrawLine(const DrawLineParams& params) = 0;
    virtual ObjectHandle DrawFill(const DrawFillParams& params) = 0;
    virtual ObjectHandle DrawText(const DrawTextParams& params) = 0;
    virtual ObjectHandle DrawPoints(const DrawPointParams& params) = 0;

    virtual bool SetVisible(ObjectHandle handle, bool visible) = 0;
    virtual bool SetColor(ObjectHandle handle, Color color) = 0;
    virtual bool Remove(ObjectHandle handle) = 0;
    virtual void RemoveAll() = 0;

    virtual ViewportInfo GetViewport() const = 0;
    virtual Point2D WorldToScreen(double worldX, double worldY) const = 0;
    virtual Point2D ScreenToWorld(double screenX, double screenY) const = 0;
};

/**
 * @interface RenderingAPI
 * @brief Top-level access point for the rendering subsystem.
 */
class RenderingAPI {
public:
    virtual ~RenderingAPI() = default;
    virtual RenderingInterface* getInterface() = 0;
};

} // namespace PluginSDK::Rendering
