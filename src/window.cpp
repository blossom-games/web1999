#include <assert.h>
#include <debug.h>
#include <graphx.h>
#include <sys/util.h>

#include "colors.h"
#include "gfx/gfx.h"
#include "util.hpp"
#include "window.hpp"

static const int REQUIRED_RIGHT_VISIBLE = window_controls_width + 4;
static const int REQUIRED_LEFT_VISIBLE = REQUIRED_RIGHT_VISIBLE;
static const int REQUIRED_TOP_VISIBLE = 10;

Window::Window(int x, uint8_t y)
    : Window(x, y, randInt(30, 300), randInt(30, 200), "Blank window") {}

Window::Window(int x, uint8_t y, uint16_t width, uint16_t height, const char *title)
    : location(MIN(GFX_LCD_WIDTH - REQUIRED_LEFT_VISIBLE,
                   MAX(-width + REQUIRED_RIGHT_VISIBLE, x - (width / 2))),
               MIN(GFX_LCD_HEIGHT - REQUIRED_TOP_VISIBLE, MAX(0, y - (height / 2))),
               MAX(30, width), MAX(30, height)),
      title(title) {
    dbg_printf("Created new window at (%d, %d) size (%d, %d)\n", location.x, location.y,
               location.width, location.height);
}

uint8_t Window::getContentAreaFillColor() const {
    return C_WHITE;
}

Rect Window::getBounds() const {
    return location;
}

Rect Window::getContentArea() const {
    Rect out = this->location.inset(6);
    out.height -= this->TITLEBAR_HEIGHT;
    out.y += this->TITLEBAR_HEIGHT;

    return out;
}

Rect Window::getCloseTarget() const {
    return Rect(this->location.x + this->location.width - 6 - 5, this->location.y + 6,
                5, 5);
}

Rect Window::getMoveTarget() const {
    // TODO: needs X offset if an icon is added
    return Rect(this->location.x + 5, this->location.y + 5,
                // Right edge of target must exclude the window controls
                this->location.width - 8 - window_controls_width - 2,
                TITLEBAR_HEIGHT - 1);
}

void Window::draw(bool active) const {
    this->location.drawShaded(C_BORDER_TOP, C_BORDER_BOTTOM);
    this->location.inset(1).drawShaded(C_BORDER_INSIDE_TOP, C_BORDER_INSIDE_BOTTOM);

    this->location.inset(2).drawShaded(C_FRAME, C_FRAME);
    this->location.inset(3).drawShaded(C_FRAME, C_FRAME);

    // IE icon in the top left? Can it be made to fit in ~7px square?

    gfx_SetColor(active ? C_TITLEBAR_ACTIVE : C_TITLEBAR_INACTIVE);
    gfx_FillRectangle(this->location.x + 4, this->location.y + 4,
                      this->location.width - 8, TITLEBAR_HEIGHT);

    gfx_SetTextFGColor(active ? C_TITLEBAR_ACTIVE_TEXT : C_TITLEBAR_INACTIVE_TEXT);
    gfx_SetTextBGColor(active ? C_TITLEBAR_ACTIVE : C_TITLEBAR_INACTIVE);
    gfx_PrintStringXY(this->title, this->location.x + 5, this->location.y + 5);

    gfx_TransparentSprite(window_controls,
                          this->location.x + this->location.width - 4 -
                                  window_controls_width,
                          this->location.y + 5);

    auto contentArea = this->getContentArea();
    contentArea.outset(2).drawShaded(C_BORDER_INSIDE_BOTTOM, C_BORDER_INSIDE_TOP);
    contentArea.outset(1).drawShaded(C_BORDER_BOTTOM, C_BORDER_TOP);

    auto contentColor = getContentAreaFillColor();
    if (contentColor != UINT8_MAX) {
        gfx_SetColor(contentColor);
        gfx_FillRectangle(contentArea.x, contentArea.y, contentArea.width,
                          contentArea.height);
    }
}

void Window::shift(gfx_point_t shift) {
    location.shift(shift);
}
