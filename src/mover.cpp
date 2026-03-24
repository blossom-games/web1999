#include "mover.hpp"

/*
 * The Mover does fixed-point movement with error accumulation, similar to
 * Bresenham's line algorithm. Non-integer values have 7 fractional bits
 * and progress ticks from 0 to 1 over that range.
 *
 * dx and dy are the fixed-point change of x and y per tick of 1/128 progress,
 * which get added to the current x and y coordinates. The bits that get
 * truncated in removing the fractional part for that addition get stored
 * in errx and erry, which contribute 1 more unit of movement when they exceed
 * one.
 */

Mover::Mover(gfx_point_t start, gfx_point_t end) : errx(0), erry(0), progress(0) {
    dx = end.x - start.x;
    dy = end.y - start.y;
}

static int signum(int x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    } else {
        return 0;
    }
}

gfx_point_t Mover::advance(uint8_t speed, bool *done) {
    if (speed + progress > 0x80) {
        speed = 0x80 - progress;
    }
    gfx_point_t out;

    out.x = (dx * speed) / 0x80;
    errx += abs(dx * speed) & 0x7f;
    if (errx >= 0x80) {
        out.x += signum(dx);
        errx -= 0x80;
    }

    out.y = (dy * speed) / 0x80;
    erry += abs(dy * speed) & 0x7f;
    if (erry >= 0x80) {
        out.y += signum(dy);
        erry -= 0x80;
    }

    progress += speed;
    *done = progress >= 0x80;
    if (*done) {
        // Some speeds may be able to leave some extra error behind.
        out.x += errx > 0;
        out.y += erry > 0;
    }

    return out;
}
