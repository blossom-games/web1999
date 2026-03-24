#include <debug.h>
#include <sys/timers.h>
#include <ti/getcsc.h>

#include "colors.h"
#include "credits.hpp"
#include "window.hpp"
#include "windows.hpp"

static void drawRect(const Rect &r, uint8_t color) {
    gfx_SetColor(color);
    gfx_FillRectangle(r.x, r.y, r.width, r.height);
}

static Window PLAIN_WINDOW(160, 120, 280, 200, "Large test window");

static void drawPlainWindow() {
    auto &w = PLAIN_WINDOW;

    w.draw(true);
}

static void drawPlainWindowAreas() {
    auto &w = PLAIN_WINDOW;

    w.draw(false);
    drawRect(w.getContentArea(), C_GREEN);
    drawRect(w.getCloseTarget(), C_TEAL);
    drawRect(w.getMoveTarget(), C_FUCHSIA);
}

static void drawOffscreenWindow() {
    static Window w(0, 120, 200, 120, "Partially offscreen");

    w.draw(true);
}

#define DRAWFUNC(t)                                                                    \
    static void draw##t() {                                                            \
        static t w(160, 120);                                                          \
        w.draw(true);                                                                  \
    }

DRAWFUNC(HotSingles)
DRAWFUNC(DragonballFanpage)
DRAWFUNC(Demotivator)
DRAWFUNC(NapsterVirus)
DRAWFUNC(HomestarRunner)
DRAWFUNC(GotMilk)
DRAWFUNC(AolMailer)
DRAWFUNC(MillionthVisitor)
DRAWFUNC(PersonalityTest)
DRAWFUNC(AllYourBase)
DRAWFUNC(Ticalc)
DRAWFUNC(Credits)
DRAWFUNC(EthanTang)
#define DRAWFUNC_P(t)                                                                  \
    { #t, draw##t }

static const struct {
    const char *name;
    void (*drawFunc)();
} MODES[] = {
        {"Plain window", drawPlainWindow},
        {"Plain window areas", drawPlainWindowAreas},
        {"Offscreen left", drawOffscreenWindow},
        DRAWFUNC_P(HotSingles),
        DRAWFUNC_P(DragonballFanpage),
        DRAWFUNC_P(Demotivator),
        DRAWFUNC_P(NapsterVirus),
        DRAWFUNC_P(HomestarRunner),
        DRAWFUNC_P(GotMilk),
        DRAWFUNC_P(AolMailer),
        DRAWFUNC_P(MillionthVisitor),
        DRAWFUNC_P(PersonalityTest),
        DRAWFUNC_P(AllYourBase),
        DRAWFUNC_P(Ticalc),
        DRAWFUNC_P(Credits),
        DRAWFUNC_P(EthanTang),
};

static const unsigned N_MODES = sizeof(MODES) / sizeof(*MODES);

void testvis_main(void) {

    unsigned modeIdx = 0;
    while (1) {
        gfx_ZeroScreen();
        dbg_printf("MODE %d/%d: %s\n", modeIdx, N_MODES - 1, MODES[modeIdx].name);
        MODES[modeIdx].drawFunc();
        gfx_BlitBuffer();

        int key;
        while ((key = os_GetCSC()) == 0) {
            msleep(30);
        }
        if (key == sk_Left) {
            if (modeIdx > 0) {
                modeIdx -= 1;
            } else {
                modeIdx = N_MODES - 1;
            }
        } else if (key == sk_Right) {
            modeIdx += 1;
            if (modeIdx >= N_MODES) {
                modeIdx = 0;
            }
        } else {
            break;
        }
    }
}
