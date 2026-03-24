#include <debug.h>
#include <graphx.h>
#include <stdint.h>

#include "cursor.hpp"
#include "gfx/gfx.h"
#include "util.hpp"

/**
 * Return an integer representing the fraction of total motion to be performed
 * per step in a cursor movement of dx/dy pixels, using the same fixed-point
 * representation as a Mover (128 = 1.0).
 *
 * In general this is inversely proportional to the Euclidean distance to be
 * moved; short moves should complete quickly and long ones more slowly.
 */
static unsigned moveFractionPerTick(unsigned dx, unsigned dy) {
    unsigned d = dx + dy;
    // Arbitrary thresholds, chosen to look okay.
    if (d == 0) {
        return 128;
    } else if (d < 3) {
        return 64;
    } else if (d < 9) {
        return 32;
    } else if (d < 16) {
        return 16;
    } else if (d < 48) {
        return 8;
    } else if (d < 96) {
        return 4;
    } else if (d < 152) {
        return 2;
    } else {
        return 1;
    }
}

Cursor::Cursor()
    : x(GFX_LCD_WIDTH / 2),
      y(GFX_LCD_HEIGHT / 2),
      mover(optional<Mover>()),
      overdraw_saved{OVERDRAW_WIDTH, OVERDRAW_HEIGHT} {}

bool Cursor::isBusy() const {
    return mover;
}

void Cursor::moveTo(uint16_t x, uint8_t y) {
    mover = Mover({this->x, this->y}, {x, y});
    unsigned dx = MAX(this->x, x) - MIN(this->x, x);
    unsigned dy = MAX(this->y, y) - MIN(this->y, y);
    speed = moveFractionPerTick(dx, dy);
    assert(speed > 0 && speed <= 128);
    dbg_printf("Cursor moving from %d,%d to %d,%d with speed %d\n", this->x, this->y, x, y, speed);
}

void Cursor::moveTo(gfx_point_t dest) {
    moveTo(dest.x, dest.y);
}

void Cursor::moveBy(gfx_point_t shift) {
    moveTo(x + shift.x, y + shift.y);
}

void Cursor::cancelMove() {
    mover = optional<Mover>();
}

const gfx_sprite_t *Cursor::getOverdrawSprite() const {
    return (const gfx_sprite_t *) (&this->overdraw_saved);
}

gfx_sprite_t *Cursor::getOverdrawSprite() {
    return (gfx_sprite_t *) (&this->overdraw_saved);
}

void Cursor::draw() {
    gfx_GetSprite(getOverdrawSprite(), x, y);
    gfx_RLETSprite(cursor_pointer, x, y);
}

void Cursor::unDraw() const {
    gfx_Sprite(getOverdrawSprite(), x, y);
}

gfx_point_t Cursor::tick() {
    if (!mover) {
        return {0, 0};
    }

    bool done;
    gfx_point_t shift = mover->advance(speed, &done);
    if (done) {
        dbg_printf("Cursor move completed!\n");
        mover = optional<Mover>();
    }

    x += shift.x;
    y += shift.y;

    return shift;
}

gfx_point_t Cursor::getLocation() const {
    return {x, y};
}
