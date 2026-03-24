/**
 * main_shim supports building multiple targets by calling the correct
 * "real" main function for a given application based on build-time
 * configuration.
 *
 * The actual main functions live in other main_* files, with this shim
 * handling the system setup and teardown to put everything in the expected
 * modes.
 */
#include <fileioc.h>
#include <graphx.h>
#include <string.h>
#include <sys/rtc.h>

#include "gfx/gfx.h"

void scr_main(void);
void testvis_main(void);

int main(void) {
    srand(rtc_Time());

    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTextConfig(gfx_text_clip);
    ti_SetGCBehavior(gfx_End, gfx_Begin);

    memcpy(gfx_palette, global_palette, sizeof(global_palette));

#ifdef TESTVIS
    testvis_main();
#else
    scr_main();
#endif

    gfx_End();
    return 0;
}
