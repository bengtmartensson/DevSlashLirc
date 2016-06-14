#pragma once

#include <media/lirc.h>
#include <iostream>

#include "InfraredTypes.h"

/** This is the class version of lirc_t, defined in lirc.h. */
class LircT {
private:
    lirc_t data;
    /** Longest possible duration */
    static const lirc_t MAXDURATION = 0xFFFFFFU;

public:

    typedef enum {
        SPACE = LIRC_MODE2_SPACE,
        PULSE = LIRC_MODE2_PULSE,
        FREQUENCY = LIRC_MODE2_FREQUENCY,
        TIMEOUT = LIRC_MODE2_TIMEOUT
    } LircTType;

    LircT(lirc_t data);
    LircT(microseconds_t data, LircTType type);
    LircT(const LircT& orig);
    virtual ~LircT();

    microseconds_t getDuration() const {
        return (microseconds_t) (data & LIRC_VALUE_MASK);
    }

    bool isSpace() const {
        return LIRC_IS_SPACE(data);
    }

    bool isPulse() const {
        return LIRC_IS_PULSE(data);
    }

    bool isFrequency() const {
        return LIRC_IS_FREQUENCY(data);
    }

    bool isTimeout() const {
        return LIRC_IS_TIMEOUT(data);
    }

    bool isTerminal() const {
        return isTimeout() || getDuration() == MAXDURATION;
    }

    void dump(std::ostream& stream = std::cout) const;

    // Static versions
    static microseconds_t getDuration(lirc_t t) {
        return (microseconds_t) (t & LIRC_VALUE_MASK);
    }

    static bool isSpace(lirc_t t) {
        return LIRC_IS_SPACE(t);
    }

    static bool isPulse(lirc_t t) {
        return LIRC_IS_PULSE(t);
    }

    static bool isFrequency(lirc_t t) {
        return LIRC_IS_FREQUENCY(t);
    }

    static bool isTimeout(lirc_t t) {
        return LIRC_IS_TIMEOUT(t);
    }

    static bool isTerminal(lirc_t t) {
        return isTimeout(t) || getDuration(t) == MAXDURATION;
    }

    static void dump(lirc_t t, std::ostream& stream = std::cout) {
        (new LircT(t))->dump(stream);
    }
};
