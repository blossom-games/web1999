#pragma once

#include "optional.hpp"
#include "window.hpp"
#include "windows.hpp"

class WindowManager final {
private:
    uint8_t n_live;
    /*
     * Window order as index into the instances array. Topmost window comes first.
     */
    uint8_t order[11];
    /*
     * Dynamically allocated window instances, indexed by semi-arbitrary type ID.
     *
     * Dynamic allocation here is okay because the number of allocations is tightly
     * bounded to the size of order. We use dynamic allocation at all to work around
     * compiler issues where large objects (for instance if a WindowManager directly
     * contained an instance of each Window subclass) cause compilation to fail because
     * the compiler generates an out-of-range IY offset when object size exceeds 127
     * bytes.
     */
    Window *instances[11];

public:
    /**
     * Create a window manager containing no windows.
     */
    WindowManager();
    /**
     * Create a window manager, giving it ownership of the provided window.
     */
    WindowManager(Window *window);
    ~WindowManager();

    uint8_t size() const;
    uint8_t capacity() const;
    Window *operator[](uint8_t i);
    const Window *operator[](uint8_t i) const;
    Window *top() const;

    Window *createWindow();
    void raise(Window *w);
    void close(Window *w);
};
