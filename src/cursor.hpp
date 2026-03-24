#pragma once

#include "gfx/gfx.h"
#include "mover.hpp"
#include "optional.hpp"

class Cursor {
private:
    uint16_t x;
    uint8_t y;

    uint8_t speed;
    optional<Mover> mover;

    static const uint8_t OVERDRAW_WIDTH = cursor_pointer_width;
    static const uint8_t OVERDRAW_HEIGHT = cursor_pointer_height;
    uint8_t overdraw_saved[2 + (OVERDRAW_WIDTH * OVERDRAW_HEIGHT)];

    gfx_sprite_t *getOverdrawSprite();
    const gfx_sprite_t *getOverdrawSprite() const;

public:
    Cursor();

    bool isBusy() const;
    gfx_point_t getLocation() const;

    void moveTo(uint16_t x, uint8_t y);
    void moveTo(gfx_point_t destination);
    void moveBy(gfx_point_t shift);
    gfx_point_t tick();
    void cancelMove();

    void draw();
    void unDraw() const;
};
