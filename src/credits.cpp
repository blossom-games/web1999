#include <fileioc.h>
#include <ti/vars.h>

#include "colors.h"
#include "credits.hpp"

static const char appVarName[] = "WWW99CrF";
static const char appVarHeader[] = {'I', 'E', 4, 'l', 'y', 'f'};

Credits::Credits(int x, uint8_t y) : Window(x, y, 220, 140, "Credits") {}

void Credits::draw(bool active) const {
    Window::draw(active);
    const auto c = getContentArea();

    // Clipped text can't be scaled, but this window is guaranteed not be placed
    // even partially offscreen so we can disable clipping in order to get larger
    // text.
    gfx_SetTextConfig(gfx_text_noclip);
    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(C_MAROON);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("WEB1999", c.x + 47, c.y + 3);
    gfx_SetTextConfig(gfx_text_clip);

    gfx_SetTextFGColor(C_BLACK);
    gfx_PrintStringXY("by Peter Marheine", c.x + 40, c.y + 21);
    gfx_PrintStringXY("<peter@taricorp.net>", c.x + 56, c.y + 30);
    gfx_SetColor(C_GRAY);
    gfx_HorizLine(c.x, c.y + 40, c.width);

    gfx_SetTextFGColor(C_TEAL);
    gfx_SetColor(C_TEAL);
    gfx_PrintStringXY("gitlab.com/taricorp/web1999", c.x + 5, c.y + 43);
    gfx_HorizLine(c.x + 5, c.y + 51, 198);
    gfx_SetTextFGColor(C_GRAY);
    gfx_PrintStringXY("or visit ", c.x + 36, c.y + 54);
    gfx_SetTextFGColor(C_TEAL);
    gfx_PrintString("www.taricorp.net");
    gfx_HorizLine(c.x + 91, c.y + 62, 112);

    gfx_SetColor(C_BLACK);
    gfx_SetTextFGColor(C_YELLOW);
    gfx_FillRectangle(c.x + 8, c.y + 68, c.width - 16, 27);
    gfx_PrintStringXY("Press any key to continue", c.x + 16, c.y + 73);
    gfx_SetTextFGColor(C_SILVER);
    gfx_PrintStringXY("Auto-start in ", c.x + 19, c.y + 83);
    int secondsX = gfx_GetTextX();
    int secondsY = gfx_GetTextY();
    gfx_PrintString("00 seconds");

    gfx_SetTextFGColor(C_DARKGRAY);
    gfx_PrintStringXY("Delete AppVar ", c.x + 14, c.y + 100);
    gfx_PrintString(appVarName);
    gfx_PrintString(" to");
    gfx_PrintStringXY("show this message again later.", c.x + 2, c.y + 109);

    // Inform the caller of the countdown location by leaving the cursor there.
    gfx_SetTextXY(secondsX, secondsY);
}

bool Credits::isSeen() {
    int archived;
    // Don't validate the header string, because if the appvar exists but
    // we didn't create it there is no recourse. Just assume we created it.
    return os_GetAppVarData(appVarName, &archived) != nullptr;
}

void Credits::markSeen() {
    if (isSeen()) {
        return;
    }

    // If not seen, the var doesn't exist and we should create it.
    uint8_t handle = ti_Open(appVarName, "w");
    if (handle == 0) {
        // Failed to create; give up.
        return;
    }

    // Write a header that identifies this as a WEB1999 state var, rather than
    // data associated with some other program.
    if (ti_Write(appVarHeader, sizeof(appVarHeader), 1, handle) != 1) {
        // Failed to write for some reason; delete the incomplete var.
        ti_Close(handle);
        ti_Delete(appVarName);
    }

    // Archive the var, since it's just a persistent flag.
    ti_SetArchiveStatus(true, handle);
    ti_Close(handle);
}
