#pragma once

enum color_index {
    // Not part of other palettes, but used by Windows.
    // Desktop color comes first specifically so we can zero the display
    // buffer to get a blank desktop.
    C_DESKTOP = 0,
    C_DARKGRAY = 1,
    // Windows VGA palette as included in the HTML 3.0 spec
    C_WHITE = 2,
    C_SILVER = 3,
    C_GRAY = 4,
    C_BLACK = 5,
    C_RED = 6,
    C_MAROON = 7,
    C_YELLOW = 8,
    C_OLIVE = 9,
    C_LIME = 10,
    C_GREEN = 11,
    C_AQUA = 12,
    C_TEAL = 13,
    C_BLUE = 14,
    C_NAVY = 15,
    C_FUCHSIA = 16,
    C_PURPLE = 17,
    // Following entries are the 216-color web-safe palette. There is no
    // typical order to these, so refer to the output of scripts/webpalette.py
    // to determine their ordering (or grab swatches from the palette PNG in
    // the gfx directory).
    C_WEB_COLORS = 18,

    // Aliases for colors on particular UI elements
    C_BORDER_TOP = C_SILVER,
    C_BORDER_INSIDE_TOP = C_WHITE,
    C_BORDER_INSIDE_BOTTOM = C_GRAY,
    C_BORDER_BOTTOM = C_DARKGRAY,
    C_TITLEBAR_ACTIVE = C_NAVY,
    C_FRAME = C_SILVER,
    C_TITLEBAR_ACTIVE_TEXT = C_WHITE,
    C_TITLEBAR_INACTIVE = C_DARKGRAY,
    C_TITLEBAR_INACTIVE_TEXT = C_SILVER,
    C_CONTENTAREA = C_TITLEBAR_ACTIVE_TEXT,
};
