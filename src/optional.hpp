#pragma once

#include <assert.h>

template<class T>
class optional {
private:
    bool present;
    T value;

public:
    optional() : present(false) {}
    optional(T value) : present(true), value(value) {}

    operator bool() const {
        return present;
    }

    T &operator*() {
        assert(present);
        return value;
    }

    const T &operator*() const {
        assert(present);
        return value;
    }

    T *operator->() {
        assert(present);
        return &value;
    }

    const T *operator->() const {
        assert(present);
        return &value;
    }
};
