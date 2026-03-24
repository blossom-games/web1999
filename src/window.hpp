#pragma once

#include <graphx.h>
#include <stdint.h>

#include "rect.hpp"

class Window {
private:
    Rect location;
    const char *title;
    bool active;

    static const uint8_t TITLEBAR_HEIGHT = 9;

protected:
    virtual uint8_t getContentAreaFillColor() const;

public:
    Window() = default;
    Window(int x, uint8_t y);
    /*
     * Window instances are meant to be POD, but delete on a superclass pointer for an
     * instance of a subclass is undefined behavior even for POD types unless there is
     * a virtual destructor.
     */
    virtual ~Window() = default;
    /**
     * Create a window with the given width and height in pixels, approximately
     * centered at the screen coordinates (x, y).
     */
    Window(int x, uint8_t y, uint16_t width, uint16_t height, const char *title);

    /**
     * Display the window chrome.
     */
    virtual void draw(bool active) const;

    Rect getBounds() const;

    /**
     * Return the rectangle bounding the window's content area.
     */
    Rect getContentArea() const;

    /**
     * Return the bounding rectangle for clicking the close button.
     */
    Rect getCloseTarget() const;

    /**
     * Return the bounding rectangle for dragging the titlebar to move the window.
     */
    Rect getMoveTarget() const;

    void setActive();
    void setInactive();

    void shift(gfx_point_t shift);
};
