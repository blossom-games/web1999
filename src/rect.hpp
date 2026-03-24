#pragma once

/**
 * A screen-space rectangle with X and Y coordinates of the top-left corner
 * and a width and height. Left, right and bottom edges may extend off the
 * screen's edge, but not the top.
 */
class Rect {
public:
    int x;
    int y;
    uint16_t width;
    uint8_t height;

    Rect() = default;
    Rect(int x, int y, uint16_t width, uint8_t height);

    /**
     * Return true if any part of this rectangle is onscreen.
     */
    bool isOnScreen() const;
    bool overlapsWith(const Rect &other) const;

    gfx_point_t getNearestPoint(gfx_point_t ref) const;

    void shift(gfx_point_t shift);

    /**
     * Return a new rectangle set inside this one by distance pixels.
     */
    Rect inset(uint8_t distance) const;

    /**
     * Return a new rectangle set outside this one by distance pixels.
     */
    Rect outset(uint8_t distance) const;

    /**
     * Draw this rectangle using win9x-style shading.
     *
     * @param topColor Palette index for the top and left edges
     * @param botColor Palette index for the bottom and right edges
     */
    void drawShaded(uint8_t topColor, uint8_t botColor) const;
};
