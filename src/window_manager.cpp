#include <cstdint>
#include <cstring>
#include <debug.h>
#include <sys/util.h>

#include "util.hpp"
#include "window_manager.hpp"

WindowManager::WindowManager() : n_live(0), instances{} {
    static_assert(ARRAY_SIZE(order) == ARRAY_SIZE(instances),
                  "WM order array must accommodate all instances");
}

WindowManager::WindowManager(Window *const window) : WindowManager() {
    if (window == nullptr) {
        return;
    }

    // Randomly select a type to replace with this window. Once the
    // instance is removed then the type will behave as usual again.
    auto tid = randInt(0, capacity() - 1);
    while (instances[tid] != nullptr) {
        tid = (tid + 1) % capacity();
    }

    order[0] = tid;
    instances[tid] = window;
    n_live += 1;
    assert(size() == 1);
}

WindowManager::~WindowManager() {
    for (uint8_t i = 0; i < size(); i++) {
        delete instances[order[i]];
    }
}

uint8_t WindowManager::size() const {
    return n_live;
}

uint8_t WindowManager::capacity() const {
    return ARRAY_SIZE(order);
}

static int genXCoord() {
    // This uses an Irwin-Hall distribution approximating a normal distribution in
    // range 0-512 to generate coordinates with approximate normal distribution
    // between -96 and 416 which is a pretty good range for window X coordinates.
    int sum = 0;

    for (auto i = 0; i < 3; i++) {
        uint32_t x = random();
        sum += x & 0xff;
        sum += (x >> 8) & 0xff;
        sum += (x >> 16) & 0xff;
        sum += (x >> 24) & 0xff;
    }
    return (sum / 6) - 96;
}

Window *WindowManager::createWindow() {
    if (size() == capacity()) {
        return nullptr;
    }

    auto x = randInt(0, capacity() - 1);
    dbg_printf("WM creating window of type %d\n", x);

    int xc = genXCoord();
    uint8_t yc = randInt(0, 220);
    dbg_printf("Randomizing new window to %d,%d\n", xc, yc);

#define CREATECASE(n, t)                                                               \
    case n:                                                                            \
        static_assert(n < ARRAY_SIZE(order), "Windows must have slots in WM order");   \
        if (instances[n] == nullptr) {                                                 \
            instances[n] = new t(xc, yc);                                              \
            tid = n;                                                                   \
            break;                                                                     \
        } else {                                                                       \
            dbg_printf("Fallthrough; type %d already has an instance %p\n", n,         \
                       instances[n]); \
        }

    uint8_t tid;
    switch (x) {
    createWrap:
        CREATECASE(0, HotSingles)
        CREATECASE(1, DragonballFanpage)
        CREATECASE(2, Demotivator)
        CREATECASE(3, NapsterVirus)
        CREATECASE(4, HomestarRunner)
        CREATECASE(5, GotMilk)
        CREATECASE(6, AolMailer)
        CREATECASE(7, MillionthVisitor)
        CREATECASE(8, PersonalityTest)
        CREATECASE(9, AllYourBase)
        CREATECASE(10, Ticalc)
        CREATECASE(11, EthanTang)
        goto createWrap;

        default:
            dbg_printf("Window type count mismatch: type %d unhandled\n", x);
            assert(false);
            return nullptr;
    }

#undef CREATECASE
    dbg_printf("Created window instance type %d: %p\n", tid, instances[tid]);

    // Insert at the top of the stack
    memmove(order + 1, order, sizeof(*order) * size());
    order[0] = tid;
    n_live += 1;

    return instances[tid];
}

void WindowManager::close(Window *w) {
    for (uint8_t i = 0; i < size(); i++) {
        if (w == instances[order[i]]) {
            const uint8_t tid = order[i];
            delete instances[tid];
            instances[tid] = nullptr;

            memmove(&order[i], &order[i + 1], size() - i);
            n_live -= 1;
            return;
        }
    }
    dbg_printf("Ignored attempt to close window %p that doesn't seem to exist\n", w);
}

Window *WindowManager::top() const {
    if (size() == 0) {
        return nullptr;
    }

    assert(instances[order[0]] != nullptr);
    return instances[order[0]];
}

void WindowManager::raise(Window *w) {
    for (uint8_t i = 0; i < size(); i++) {
        const auto ip = order[i];
        if (w == instances[ip]) {
            memmove(order + 1, order, i * sizeof(*order));
            order[0] = ip;
            return;
        }
    }
}

Window *WindowManager::operator[](uint8_t i) {
    assert(i < size());
    assert(instances[order[i]] != nullptr);
    return instances[order[i]];
}

