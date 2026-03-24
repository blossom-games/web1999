#include <assert.h>
#include <debug.h>
#include <graphx.h>
#include <stdint.h>

#include "rect.hpp"

Rect::Rect(int x, int y, uint16_t width, uint8_t height)
    : x(x),
      y(y),
      width(width),
      height(height) {}

void Rect::shift(gfx_point_t shift) {
    x += shift.x;
    y += shift.y;
}

bool Rect::isOnScreen() const {
    return gfx_CheckRectangleHotspot(x, y, width, height, 0, 0, GFX_LCD_WIDTH,
                                     GFX_LCD_HEIGHT);
}

bool Rect::overlapsWith(const Rect &other) const {
    return gfx_CheckRectangleHotspot(x, y, width, height, other.x, other.y, other.width,
                                     other.height);
}

gfx_point_t Rect::getNearestPoint(gfx_point_t ref) const {
    gfx_point_t out;

    if (ref.x < x) {
        out.x = x;
    } else if (ref.x > x + width) {
        out.x = x + width;
    } else {
        out.x = ref.x;
    }

    if (ref.y < y) {
        out.y = y;
    } else if (ref.y > y + height) {
        out.y = y + height;
    } else {
        out.y = ref.y;
    }

    return out;
}

Rect Rect::inset(uint8_t distance) const {
    return Rect(this->x + distance, this->y + distance, this->width - (distance << 1),
                this->height - (distance << 1));
}

Rect Rect::outset(uint8_t distance) const {
    return Rect(this->x - distance, this->y - distance, this->width + (distance << 1),
                this->height + (distance << 1));
}

void Rect::drawShaded(uint8_t topColor, uint8_t botColor) const {
    gfx_SetColor(topColor);
    gfx_HorizLine(this->x, this->y, this->width);
    gfx_VertLine(this->x, this->y, this->height);
    gfx_SetColor(botColor);
    gfx_HorizLine(this->x, this->y + this->height - 1, this->width);
    gfx_VertLine(this->x + this->width - 1, this->y, this->height);
}
