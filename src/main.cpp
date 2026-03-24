#include <debug.h>
#include <graphx.h>
#include <limits.h>
#include <string.h>
#include <sys/timers.h>
#include <ti/getcsc.h>
#include <time.h>

#include "colors.h"
#include "credits.hpp"
#include "cursor.hpp"
#include "gfx/gfx.h"
#include "optional.hpp"
#include "window.hpp"
#include "window_manager.hpp"

enum ControlMode {
    IDLE,
    // Cursor is moving to the close target
    CLOSE_MOVE,
    // Cursor is moving to the titlebar drag target
    RELOCATE_MOVE,
    // Cursor moving to titlebar in order to activate target
    RAISE_MOVE,
    RELOCATE_DRAG,
};

struct ctx {
    Cursor cursor;
    WindowManager wm;

    /** The window that the cursor wants to interact with. */
    Window *target;
    /* Exactly how the cursor wants to interact with the target. */
    enum ControlMode mode;
};

/** Redraw all windows. */
static void fullRedraw(struct ctx &ctx) {
    gfx_ZeroScreen();
    // Draw windows from the bottom up, excluding the topmost
    auto i = ctx.wm.size();
    while (i-- > 0) {
        ctx.wm[i]->draw(false);
    }
    // Topmost window must be drawn with focus
    auto top = ctx.wm.top();
    if (top != nullptr) {
        top->draw(true);
    }
}

/**
 * Partial redraw for after a window is placed on top, either because it's
 * created or gains focus.
 *
 * This redraws only the old and new topmost windows, because others are
 * guaranteed to be unchanged.
 */
static void repaintAfterRaise(struct ctx &ctx) {
    auto top = ctx.wm.top();
    if (top == nullptr) {
        dbg_printf("%s: expected at least one window to be live!\n", __func__);
        return;
    }

    if (ctx.wm.size() > 1) {
        ctx.wm[1]->draw(false);
    }
    top->draw(true);
}

static int distance_NonLinear(gfx_point_t a, gfx_point_t b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

/**
 * Return a pointer to the window that the cursor should interact with,
 * setting targetIsCloseable to indicate whether its close target is occluded.
 * (If it is, the window will need to be moved or raised before it can be closed.)
 */
static Window *getNewTarget(struct ctx &ctx, bool *targetIsCloseable) {
    auto &wm = ctx.wm;
    auto &cursor = ctx.cursor;

    Window *nearest = nullptr;
    int nearestMetric = INT_MAX;

    for (auto candidateIdx = 0; candidateIdx < wm.size(); candidateIdx++) {
        auto candidate = wm[candidateIdx];
        dbg_printf("Inspect target candidate %p\n", candidate);
        auto closeTarget = candidate->getCloseTarget();
        bool closeOccluded = false;
        auto moveTarget = candidate->getMoveTarget();
        bool moveOccluded = false;

        auto occluderIdx = candidateIdx;
        while (occluderIdx-- > 0) {
            // Occlusion is simply based on having any overlap, because partial overlap
            // involves much more complicated geometry.
            auto occBounds = wm[occluderIdx]->getBounds();
            closeOccluded |= occBounds.overlapsWith(closeTarget);
            moveOccluded |= occBounds.overlapsWith(moveTarget);
        }
        // If both targets are occluded, this window is not interactable.
        if (closeOccluded && moveOccluded) {
            dbg_printf("Window %p is fully occluded, will not "
                       "interact.\n",
                       &*candidate);
            continue;
        }

        auto candidateDest = closeTarget.getNearestPoint(cursor.getLocation());
        auto candidateDist = distance_NonLinear(cursor.getLocation(), candidateDest);
        if (candidateDist < nearestMetric) {
            nearest = &*candidate;
            nearestMetric = candidateDist;
            *targetIsCloseable = !closeOccluded;
        }
    }

    return nearest;
}

void scr_main(void) {
    // Display the credits and pause for a keypress before continuing.
    Window *credits = nullptr;
    if (!Credits::isSeen()) {
        dbg_printf("Showing credits window for first run\n");
        credits = new Credits(160, 120);

        gfx_ZeroScreen();
        credits->draw(true);

        int timeXCoord = gfx_GetTextX();
        int timeYCoord = gfx_GetTextY();
        // Overwrite existing text
        gfx_SetTextFGColor(C_SILVER);
        gfx_SetTextBGColor(C_BLACK);

        // Show this window for up to 15 seconds, accounting for contest rules
        // that programs should operate without any input.
        const unsigned timeoutSeconds = 15;
        const clock_t startTime = clock();
        const clock_t endTime = startTime + (CLOCKS_PER_SEC * timeoutSeconds);
        clock_t now;
        while ((now = clock()) < endTime) {
            // Count down the seconds, taking the ceiling of fractional seconds
            // so we finish at the transition from 1 to 0.
            unsigned remainingSeconds = (endTime - now + (CLOCKS_PER_SEC - 1)) / CLOCKS_PER_SEC;
            gfx_SetTextXY(timeXCoord, timeYCoord);
            gfx_PrintInt(remainingSeconds, 2);

            gfx_BlitBuffer();
            if (os_GetCSC() != 0) {
                break;
            }
            msleep(33);
        }
    } else {
        dbg_printf("Credits already seen; starting immediately\n");
    }

    static struct ctx ctx = {
            .cursor = Cursor(),
            // Hand the credits window (if displayed) off to the WM so it
            // will be closed.
            .wm = WindowManager(credits),

            .target = nullptr,
            .mode = IDLE,
    };

    static unsigned int ticksUntilNewWindow = 0;
    static bool showShutdownScreen = false;
    static bool needFullRedraw = true;
    static void (*partialRedrawFunc)(struct ctx &) = nullptr;

    while (os_GetCSC() == 0 && !(showShutdownScreen = boot_CheckOnPressed())) {
        partialRedrawFunc = nullptr;
        auto &wm = ctx.wm;
        auto &cursor = ctx.cursor;
        auto &mode = ctx.mode;

        if (ticksUntilNewWindow == 0) {
            Window *created = wm.createWindow();
            if (created != nullptr) {
                // Creating a new window behaves just like raising one that was
                // completely covered, so we don't need to do a full redraw.
                partialRedrawFunc = repaintAfterRaise;
                // Cancel any active action and decide what to do again,
                // since a new window appeared.
                cursor.cancelMove();
                mode = IDLE;
            }

            // The range here affects how quickly windows spawn; cursor moves
            // always take less than 128 ticks to complete, but often much
            // less. In general, we want windows to spawn slightly more often
            // than they are closed so there's usually at least one window
            // on screen.
            ticksUntilNewWindow = randInt(12, 96);
            dbg_printf("Next window in %d ticks\n", ticksUntilNewWindow);
        } else {
            ticksUntilNewWindow -= 1;

            switch (mode) {
                case IDLE:
                    if (wm.size() == 0) {
                        // Do nothing if no windows are open
                        break;
                    } else {
                        // There's at least one window available to interact with.
                        // Choose which one we want to do something with.
                        bool targetCanClose;
                        auto target = getNewTarget(ctx, &targetCanClose);
                        assert(target != nullptr);
                        ctx.target = target;
                        dbg_printf("Selected target window %p\n", ctx.target);

                        // What interaction should we do?
                        auto closeTarget = target->getCloseTarget();
                        auto moveTarget = target->getMoveTarget();
                        if (closeTarget.isOnScreen()) {
                            if (!targetCanClose) {
                                // Move to titlebar to raise the window
                                dbg_printf("Move cursor to raise window %p\n", target);
                                cursor.moveTo(moveTarget.getNearestPoint(
                                        cursor.getLocation()));
                                mode = RAISE_MOVE;
                            } else {
                                // Move to close target and close the window
                                dbg_printf("Move cursor to close target\n");
                                cursor.moveTo(closeTarget.getNearestPoint(
                                        cursor.getLocation()));
                                mode = CLOSE_MOVE;
                            }
                        } else {
                            // Move to titlebar and prepare to move the window
                            dbg_printf("Prepare to move window\n");
                            cursor.moveTo(
                                    moveTarget.getNearestPoint(cursor.getLocation()));
                            mode = RELOCATE_MOVE;
                        }
                    }
                    break;

                case CLOSE_MOVE:
                    if (!cursor.isBusy()) {
                        // TODO: raise the target window and take a moment to "click"?
                        mode = IDLE;
                        dbg_printf("Closing window\n");
                        wm.close(ctx.target);
                        ctx.target = nullptr;
                        // Full redraw to ensure that whatever was under this window
                        // gets repainted.
                        needFullRedraw = true;
                    }
                    break;

                case RAISE_MOVE:
                    if (!cursor.isBusy()) {
                        wm.raise(ctx.target);
                        // TODO: can this simply redraw the target instead of everything?
                        // No, but if we also repaint the old top as inactive then it's okay.
                        //fullRedraw(ctx);
                        partialRedrawFunc = repaintAfterRaise;
                        mode = IDLE;
                    }
                    break;

                case RELOCATE_MOVE:
                    if (!cursor.isBusy()) {
                        if (ctx.target != wm.top()) {
                            wm.raise(ctx.target);
                            partialRedrawFunc = repaintAfterRaise;
                        }

                        // Insetting by one ensures the target will actually be visible.
                        auto ct = ctx.target->getCloseTarget().inset(1);
                        gfx_point_t shift{};

                        if (ct.x >= GFX_LCD_WIDTH) {
                            shift.x = GFX_LCD_WIDTH - ct.x;
                        }
                        if (ct.y >= GFX_LCD_HEIGHT) {
                            shift.y = GFX_LCD_HEIGHT - ct.y;
                        }

                        dbg_printf("Moving window to expose close target\n");
                        cursor.moveBy(shift);
                        mode = RELOCATE_DRAG;
                    }
                    break;

                case RELOCATE_DRAG:
                    if (!cursor.isBusy()) {
                        mode = IDLE;
                    } else {
                        auto moved = cursor.tick();
                        ctx.target->shift(moved);
                        needFullRedraw = true;
                    }
                    break;
            }
        }

        // Redraw windows as desired
        if (needFullRedraw) {
            fullRedraw(ctx);
            needFullRedraw = false;
        } else {
            // For partial redraw, grab the last frame and erase the cursor
            // then do the update.
            gfx_BlitScreen();
            cursor.unDraw();
            if (partialRedrawFunc != nullptr) {
                partialRedrawFunc(ctx);
            }
        }
        // Move the cursor along on its business. When in RELOCATE_DRAG mode
        // we've already run the tick so must not repeat it.
        if (cursor.isBusy() && mode != RELOCATE_DRAG) {
            cursor.tick();
        }
        // Always draw the cursor for simplicity, even if it didn't move.
        cursor.draw();

        gfx_SwapDraw();
    }

    if (showShutdownScreen) {
        gfx_FillScreen(C_BLACK);
        gfx_SetTextFGColor(C_WEB_COLORS + 70);
        gfx_SetTextBGColor(255);
        gfx_PrintStringXY("It's now safe to turn off", 75, 110);
        gfx_PrintStringXY("your computer.", 110, 120);

        gfx_BlitBuffer();
        // TODO: CheckOnPressed latches on, so we need to exit only a regular
        // keypress if this was triggered by ON. Can we clear the latch?
        while (!os_GetCSC()) {
            msleep(33);
        }
    }

    // Mark credits as seen only on exit, so if this triggers garbage
    // collection it occurs while exiting instead of when starting.
    Credits::markSeen();
}
