#pragma once

#include "window.hpp"

class Credits : public Window {

public:
    Credits(int x, uint8_t y);

    virtual void draw(bool active) const override;

    static bool isSeen();
    static void markSeen();
};
