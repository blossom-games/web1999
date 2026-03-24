#pragma once

#include "window.hpp"

#define _WINTYPE(name)                                                                 \
    class name : public Window {                                                       \
    public:                                                                            \
        name() = default;                                                              \
        name(int x, uint8_t y);                                                        \
                                                                                       \
        virtual void draw(bool active) const override;

#define WINTYPE(name)                                                                  \
    _WINTYPE(name)                                                                     \
    }
#define WINTYPE_FILL(name)                                                             \
    _WINTYPE(name)                                                                     \
    virtual uint8_t getContentAreaFillColor() const override;                          \
    }

WINTYPE(HotSingles);
WINTYPE(DragonballFanpage);
WINTYPE_FILL(Demotivator);
WINTYPE(NapsterVirus);
WINTYPE_FILL(HomestarRunner);
WINTYPE_FILL(GotMilk);
WINTYPE(MillionthVisitor);
WINTYPE(AolMailer);
WINTYPE(PersonalityTest);
WINTYPE_FILL(AllYourBase);
WINTYPE(Ticalc);
WINTYPE(EthanTang);
