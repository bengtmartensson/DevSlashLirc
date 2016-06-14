#pragma once

#include <media/lirc.h>
#include "LircDevice.h"

/**
 * This class is a LIRC_MODE_LIRCCODE device as per lirc(4).
 */
class LircCodeLircDevice : public LircDevice {
private:

    uint32_t codeLength;

    void init();

public:

    LircCodeLircDevice(const char *path = defaultFilename);
    LircCodeLircDevice(const LircDevice& orig);
    //virtual ~LircCodeLircDevice();

    bool virtual open();

    /**
     * Always returns false.
     * @return false
     */
    bool isMode2() const { return false; }

    /**
     * Always returns true
     * @return true
     */
    bool isLircCode() const { return true; }

    void report(std::ostream& stream = std::cout) const;

    uint32_t readCode();

    bool writeCode(uint32_t code);
};
