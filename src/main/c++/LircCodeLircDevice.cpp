#include "LircCodeLircDevice.h"

#include <sys/ioctl.h>

static const int IOCTL_OK = 0;

LircCodeLircDevice::LircCodeLircDevice(const char *path) : LircDevice(path) {
}

LircCodeLircDevice::LircCodeLircDevice(const LircDevice& orig) : LircDevice(orig) {
}

bool LircCodeLircDevice::open() {
    if (recordingMode != LIRC_MODE_LIRCCODE) {
        std::cerr << "This is not a lirccode device!" << std::endl;
        valid = false;
        return false;
    }

    int status = ::ioctl(fileDescriptor, LIRC_GET_LENGTH, &codeLength);
    if (status != IOCTL_OK) {
        std::cerr << "Error: " << status << std::endl;
        return false;
    }
    return true;
}

void LircCodeLircDevice::report(std::ostream& stream) const {
    if (!reportValidity(stream))
        return;

    stream << "recording mode: lirccode" << std::endl;

    if (codeLength != INVALID)
        stream << "codeLength = " << codeLength << std::endl;
    else if (longReport)
        stream << "codeLength: not available" << std::endl;

    LircDevice::report(stream);
}

// TODO
uint32_t LircCodeLircDevice::readCode() {
    std::cerr << "Not implemented yet" << std::endl;
    return 0U;
}

// TODO
bool LircCodeLircDevice::writeCode(uint32_t code) {
    (void) code;
    std::cerr << "Not implemented yet" << std::endl;
    return false;
}