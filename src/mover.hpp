#pragma once

#include <graphx.h>

/**
 * Tracks screen-space motion from a start to an end point on demand.
 */
class Mover {
private:
    int errx, erry;
    int dx, dy;
    uint8_t progress;

public:
    Mover() = default;
    Mover(gfx_point_t start, gfx_point_t end);

    /*
     * Move at 1.7 fixed-point speed (0x80 = 1.0; do the entire motion in one step)
     * toward the endpoint, returning the motion done in this step and setting *done
     * to true if further calls will return zero motion.
     */
    gfx_point_t advance(uint8_t speed, bool *done);
};
