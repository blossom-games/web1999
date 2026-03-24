#include <graphx.h>

#include "colors.h"
#include "gfx/gfx.h"
#include "util.hpp"
#include "windows.hpp"

HotSingles::HotSingles(int x, uint8_t y)
    : Window(x, y, 180, 126, "Singles near you!") {}

void HotSingles::draw(bool active) const {
    Window::draw(active);
    const auto contentArea = getContentArea().inset(2);

    // A broken image (hypothetically something very NSFW)
    gfx_SetColor(C_BORDER_TOP);
    gfx_Rectangle(contentArea.x, contentArea.y, 96, 64);
    gfx_TransparentSprite(broken_image,
                          contentArea.x + (96 / 2) - (broken_image_width / 2),
                          contentArea.y + (64 / 2) - (broken_image_height / 2));

    // "HOT SINGLES" wordart
    const unsigned wordArtWidth = 158;
    const unsigned wordArtLeft = contentArea.x + (contentArea.width - wordArtWidth) / 2;
    const unsigned wordArtTop = contentArea.y + 66;
    const unsigned wordArtHeight = hotsingles_right_height;
    gfx_RLETSprite(hotsingles_left, wordArtLeft, wordArtTop);
    gfx_RLETSprite(hotsingles_right, wordArtLeft + hotsingles_left_width + 10,
                   wordArtTop);

    gfx_SetTextFGColor(C_BLACK);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("Waiting to meet today!", contentArea.x + 10,
                      wordArtTop + wordArtHeight + 2);

    gfx_SetTextFGColor(C_GREEN);
    gfx_PrintStringXY("CHAT", contentArea.x + 118, contentArea.y + 22);
    gfx_PrintStringXY("NOW!", contentArea.x + 120, contentArea.y + 30);
}

DragonballFanpage::DragonballFanpage(int x, uint8_t y)
    : Window(x, y, 190, 180, "Dragonball Oasis") {}

void DragonballFanpage::draw(bool active) const {
    Window::draw(active);

    auto contentArea = getContentArea();

    gfx_RLETSprite(chibi_goku_80px_web, contentArea.x, contentArea.y + 3);
    gfx_RLETSprite(chibi_vegeta_80px_web, contentArea.x + 110, contentArea.y + 74);
    gfx_SetColor(C_WEB_COLORS + 33);
    gfx_FillRectangle(contentArea.x + 88, contentArea.y + 16, 84, 48);
    gfx_SetColor(C_WHITE);
    gfx_HorizLine(contentArea.x + 88, contentArea.y + 16 + 2, 84);
    gfx_HorizLine(contentArea.x + 88, contentArea.y + 16 + 48 - 3, 84);
    gfx_SetTextFGColor(C_WHITE);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("DRAGONBALL", contentArea.x + 91, contentArea.y + 30);
    gfx_PrintStringXY("OASIS", contentArea.x + 110, contentArea.y + 42);

    gfx_SetTextFGColor(C_BLACK);
    gfx_PrintStringXY("Welcome to my", contentArea.x + 10, contentArea.y + 95);
    gfx_PrintStringXY("fan page!", contentArea.x + 24, contentArea.y + 117);
    const char *c = "DRAGONBALL Z";
    uint8_t color = C_RED;
    gfx_SetTextXY(contentArea.x + 10, contentArea.y + 106);
    while (*c != 0) {
        gfx_SetTextFGColor(color++);
        gfx_PrintChar(*c);
        c += 1;
    }

    // This site is part of a webring!
    gfx_SetColor(C_SILVER);
    gfx_Rectangle(contentArea.x + 8, contentArea.y + 134, 89, 21);
    gfx_SetColor(C_OLIVE);
    gfx_FillRectangle(contentArea.x + 9, contentArea.y + 135, 87, 10);
    gfx_SetTextFGColor(C_WHITE);
    gfx_PrintStringXY("DBZ webring", contentArea.x + 13, contentArea.y + 136);
    gfx_SetTextFGColor(C_TEAL);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("<Prev   Next>", contentArea.x + 11, contentArea.y + 146);
}

Demotivator::Demotivator(int x, uint8_t y) : Window(x, y, 210, 180, "Despair") {}

uint8_t Demotivator::getContentAreaFillColor() const {
    return C_BLACK;
}

void Demotivator::draw(bool active) const {
    Window::draw(active);

    // This particular design ("Mistakes") was on sale on Despair's web site at least as
    // early as April 1999. It's been modified a bit to work better on a small, low-res
    // screen.
    auto contentArea = getContentArea();
    gfx_Sprite(mistakes_illus, contentArea.x + 19, contentArea.y + 10);
    gfx_SetColor(C_TEAL);
    gfx_FillRectangle(contentArea.x + 66, contentArea.y + 120, 66, 11);
    gfx_HorizLine(contentArea.x + 66, contentArea.y + 133, 66);
    gfx_SetTextFGColor(C_BLACK);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("MISTAKES", contentArea.x + 68, contentArea.y + 123);
    gfx_SetTextFGColor(C_WHITE);
    gfx_PrintStringXY("Your purpose may only be", contentArea.x + 15,
                      contentArea.y + 137);
    gfx_PrintStringXY("to act as a warning to others", contentArea.x + 2,
                      contentArea.y + 147);
}

// LimeWire didn't arrive until early 2000, but as a vague feeling malware doesn't seem
// to have been nearly as common on Napster as it was on Napster's immediate successors
// (including LimeWire).
//
// "Crawling" wasn't released until October of 2000, so it's likely to have been widely
// available on LimeWire especially since it was a top-40 single in many countries.
NapsterVirus::NapsterVirus(int x, uint8_t y) : Window(x, y, 170, 68, "LimeWire") {}

void NapsterVirus::draw(bool active) const {
    Window::draw(active);
    auto contentArea = getContentArea();

    gfx_SetTextFGColor(C_BLACK);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("Linkin Park - Crawling", contentArea.x + 2, contentArea.y + 2);
    gfx_SetColor(C_BLACK);
    gfx_HorizLine(contentArea.x, contentArea.y + 11, contentArea.width);
    gfx_PrintStringXY("Size: 3,191 KB", contentArea.x + 2, contentArea.y + 14);
    gfx_PrintStringXY("Type: exe", contentArea.x + 2, contentArea.y + 24);
    gfx_SetTextFGColor(C_BLUE);
    gfx_PrintStringXY("Download", contentArea.x + 52, contentArea.y + 35);
    gfx_SetColor(C_BLUE);
    gfx_HorizLine(contentArea.x + 52, contentArea.y + 43, 60);
}

HomestarRunner::HomestarRunner(int x, uint8_t y)
    : Window(x, y, 170, 150, "Strong Bad Sings") {}

uint8_t HomestarRunner::getContentAreaFillColor() const {
    return UINT8_MAX;
}

void HomestarRunner::draw(bool active) const {
    Window::draw(active);

    const auto contentArea = getContentArea();
    gfx_SetColor(C_WEB_COLORS + 71); // ffcc33
    gfx_FillRectangle(contentArea.x, contentArea.y, contentArea.width, 25);
    gfx_SetColor(C_WEB_COLORS + 70); // ff9933
    gfx_FillRectangle(contentArea.x, contentArea.y + 25, contentArea.width, 15);
    gfx_SetColor(C_WEB_COLORS + 21);  // cc6600
    gfx_FillRectangle(contentArea.x, contentArea.y + 40, contentArea.width, 17);
    gfx_SetColor(C_WEB_COLORS + 20);  // 993300
    gfx_FillRectangle(contentArea.x, contentArea.y + 57, contentArea.width, 33);
    gfx_SetColor(C_BLACK);
    gfx_FillRectangle(contentArea.x, contentArea.y + 90, contentArea.width, 40);
    gfx_SetColor(C_WEB_COLORS + 64);  // cc9933
    gfx_FillCircle(contentArea.x + 119, contentArea.y + 73, 12);

    gfx_RLETSprite(strongbad, contentArea.x, contentArea.y + 10);
    // Some titles from http://www.hrwiki.org/wiki/Strong_Bad_Sings,
    // or just "STRONG BAD SINGS" and the price
    //
    gfx_SetTextFGColor(C_RED);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("STRONG BAD", contentArea.x + 80, contentArea.y + 4);
    gfx_PrintStringXY("SINGS", contentArea.x + 96, contentArea.y + 14);

    gfx_SetTextFGColor(C_GREEN);
    gfx_PrintStringXY("$99.99?", contentArea.x + 96, contentArea.y + 32);
    gfx_SetColor(C_GREEN);
    gfx_HorizLine(contentArea.x + 96, contentArea.y + 35, 50);
    gfx_SetTextFGColor(C_LIME);
    gfx_PrintStringXY("$193.75!!!", contentArea.x + 90, contentArea.y + 44);

    gfx_SetTextFGColor(C_WHITE);
    gfx_PrintStringXY("1-800-", contentArea.x + 86, contentArea.y + 108);
    gfx_PrintStringXY("555-SBSINGS", contentArea.x + 64, contentArea.y + 118);
}

GotMilk::GotMilk(int x, uint8_t y) : Window(x, y, 112, 126, "Got milk?") {}

uint8_t GotMilk::getContentAreaFillColor() const {
    return C_BLUE;
}

void GotMilk::draw(bool active) const {
    Window::draw(active);

    const auto contentArea = getContentArea();
    gfx_RLETSprite(gotmilk, contentArea.x + 40, contentArea.y + 4);

    gfx_SetTextFGColor(C_WHITE);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("GOT", contentArea.x + 4, contentArea.y + 50);
    gfx_PrintStringXY("MILK?", contentArea.x + 4, contentArea.y + 62);
}

MillionthVisitor::MillionthVisitor(int x, uint8_t y)
    : Window(x, y, 120, 98, "YOU WON!") {}

void MillionthVisitor::draw(bool active) const {
    Window::draw(active);
    const auto c = getContentArea();

    gfx_SetTextBGColor(255);
    gfx_SetTextFGColor(C_MAROON);
    gfx_PrintStringXY("You are visitor", c.x + 3, c.y + 2);
    gfx_SetTextFGColor(C_RED);
    gfx_PrintStringXY("#1,000,000!", c.x + 17, c.y + 12);
    gfx_RLETSprite(sportscar, c.x + 12, c.y + 24);
    gfx_SetColor(C_GREEN);
    gfx_FillRectangle(c.x + 4, c.y + 60, 100, 15);
    gfx_SetTextFGColor(C_WHITE);
    gfx_PrintStringXY("CLAIM PRIZE", c.x + 13, c.y + 64);

}

AolMailer::AolMailer(int x, uint8_t y) : Window(x, y, 169, 90, "America Online") {}

void AolMailer::draw(bool active) const {
    Window::draw(active);
    const auto c = getContentArea();

    gfx_RLETSprite(aol_logo, c.x + 2, c.y + 2);
    gfx_SetTextBGColor(255);
    gfx_SetTextFGColor(C_BLACK);
    gfx_PrintStringXY("540 hours ", c.x + 50, c.y + 6);
    gfx_PrintString("free!");
    gfx_SetColor(C_BLACK);
    gfx_HorizLine(c.x + 121, c.y + 14, 33);

    gfx_SetTextFGColor(C_PURPLE);
    gfx_PrintStringXY("New", c.x + 91, c.y + 18);
    gfx_PrintStringXY("version 6.0!", c.x + 64, c.y + 27);

    gfx_SetTextFGColor(C_MAROON);
    gfx_PrintStringXY("Blazing", c.x + 88, c.y + 38);
    gfx_PrintStringXY("56k speeds!", c.x + 74, c.y + 47);

    gfx_SetTextFGColor(C_BLUE);
    gfx_SetColor(C_BLUE);
    gfx_PrintStringXY("Join today!", c.x + 36, c.y + 58);
    gfx_HorizLine(c.x + 36, c.y + 67, 73);
}

PersonalityTest::PersonalityTest(int x, uint8_t y)
    : Window(x, y, 180, 126, "What droid?") {}

void PersonalityTest::draw(bool active) const {
    Window::draw(active);
    const auto c = getContentArea();

    gfx_SetTextBGColor(255);
    gfx_RLETSprite(r2unit, c.x + 2, c.y + 2);
    gfx_SetTextFGColor(C_MAROON);
    gfx_PrintStringXY("What kind of", c.x + 72, c.y + 5);
    gfx_SetTextFGColor(C_BLUE);
    gfx_PrintStringXY("DROID", c.x + 94, c.y + 15);
    gfx_SetTextFGColor(C_MAROON);
    gfx_PrintStringXY("are you?", c.x + 86, c.y + 25);

    gfx_SetColor(C_YELLOW);
    gfx_FillRectangle(c.x + 70, c.y + 46, 90, 34);
    gfx_SetTextFGColor(C_BLACK);
    gfx_PrintStringXY("Free", c.x + 98, c.y + 47);
    gfx_PrintStringXY("personality", c.x + 74, c.y + 58);
    gfx_PrintStringXY("test!", c.x + 96, c.y + 68);

    gfx_SetTextFGColor(C_OLIVE);
    gfx_PrintStringXY("Now including Episode I!", c.x + 4, c.y + 96);
}

AllYourBase::AllYourBase(int x, uint8_t y) : Window(x, y, 252, 180, "ALL YOUR BASE") {}

uint8_t AllYourBase::getContentAreaFillColor() const {
    return 255;
}

void AllYourBase::draw(bool active) const {
    Window::draw(active);
    const auto c = getContentArea();

    gfx_SetColor(C_WEB_COLORS + 34);
    gfx_FillRectangle(c.x, c.y, c.width, 80);
    gfx_SetColor(C_BLACK);
    gfx_FillRectangle(c.x, c.y + 80, c.width, 80);
    gfx_RLETSprite(allyourbase, c.x, c.y);
}

Ticalc::Ticalc(int x, uint8_t y) : Window(x, y, 252, 130, "ticalc.org") {}

void Ticalc::draw(bool active) const {
    Window::draw(active);
    const auto c = getContentArea();
    const auto navWidth = 88;

    gfx_SetColor(C_RED);
    gfx_FillRectangle(c.x, c.y, navWidth, 16);
    gfx_SetTextFGColor(C_YELLOW);
    gfx_SetTextBGColor(255);
    gfx_PrintStringXY("ticalc.org", c.x + 8, c.y + 4);

    gfx_SetColor(C_YELLOW);
    gfx_FillRectangle(c.x, c.y + 16, navWidth, c.height - 16);

    gfx_SetColor(C_BLACK);
    gfx_SetTextFGColor(C_BLACK);
    const char *const nav[] = {
            "Calculators",
            "Programming",
            "Linking",
            "Archives",
            "Features",
            "News & Lists",
            "FAQ",
            "Other Sites",
    };
    for (unsigned i = 0; i < ARRAY_SIZE(nav); i++) {
        gfx_HorizLine(c.x, c.y + 16 + (11 * i), navWidth);
        gfx_PrintStringXY(nav[i], c.x + 2, c.y + 18 + (11 * i));
    }
    gfx_HorizLine(c.x, c.y + 16 + (11 * ARRAY_SIZE(nav)), navWidth);
    gfx_VertLine(c.x + navWidth, c.y, c.height);

    const char *const bodyText[] = {
        "Your Internet",
        "headquarters for",
            "Texas Instruments'",
            "graphing calculators.",
            nullptr,
            "Home of ZShell and",
            "Fargo for the TI-85",
            "and TI-92, plus a",
            "comprehensive",
            "archive of programs",
            "and utilities.",
    };
    for (unsigned i = 0; i < ARRAY_SIZE(bodyText); i++) {
        if (bodyText[i] == nullptr) {
            continue;
        }
        gfx_PrintStringXY(bodyText[i], c.x + navWidth + 3,
                          c.y + 2 + (i * 9));
    }
}

