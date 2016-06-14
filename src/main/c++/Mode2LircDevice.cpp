#include "Mode2LircDevice.h"
#include "LircT.h"

#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

static const int IOCTL_OK = 0;
static const int IOCTL_ERROR = -1;
static const unsigned MAXSIZE = 1000;

Mode2LircDevice::Mode2LircDevice(const char *path) : LircDevice(path) {
}

//Mode2LircDevice::Mode2LircDevice(const LircDevice& orig) : LircDevice(orig) {
//}

bool Mode2LircDevice::open() {
    LircDevice::open();
    if (recordingMode != LIRC_MODE_MODE2) {
        std::cerr << "This is not a mode2 device!" << std::endl;
        valid = false;
        return false;
    }


    if (canGetRecResolution()) {
        uint32_t res;
        int status = ::ioctl(fileDescriptor, LIRC_GET_REC_MODE, &res);
        if (status != IOCTL_OK) {
            std::cerr << "Error: " << status << std::endl;
            return false;
        }
        resolution = (microseconds_t) res;
    } else
        resolution = (microseconds_t) INVALID;
    return true;
}

void Mode2LircDevice::report(std::ostream& stream) const {
    stream << "Filename \"" << fileName << "\"" << std::endl;
    if (!reportValidity())
        return;

    stream << "recording mode: mode2" << std::endl;
    if (resolution != INVALID)
        stream << "resolution = " << resolution << std::endl;
    else if (longReport)
        stream << "resolution: not available" << std::endl;

    LircDevice::report(stream);

    LircDevice::report("canSetSendCarrier", canSetSendCarrier(), stream);
    //LircDevice::report("canSetSendDutyCycle", canSetSendDutyCycle(), stream);
    LircDevice::report("canGetRecResolution", canGetRecResolution(), stream);
}

lirc_t Mode2LircDevice::read() {
    if (!canRecMode2()) {
        return 0;
    }

    lirc_t data;
    ssize_t actualLength = ::read(fileDescriptor, &data, sizeof(lirc_t));
    return actualLength == sizeof(lirc_t) ? data : 0;
}

IrSequence* Mode2LircDevice::receive() {
    microseconds_t data[MAXSIZE];
    unsigned index = 0;
    while (index < MAXSIZE) {
        lirc_t t = read();
        if (t == 0)
            return NULL;

        LircT lircT(t);
        if (index > 0 && lircT.isTerminal(t)) {
            data[index++] = replacementEndingSilence;
            break;
        }

        if (index > 0 || lircT.isPulse()) // ignore leading spaces
            data[index++] = lircT.getDuration(t);
    }
    size_t length = index;
    microseconds_t* newData = new microseconds_t[length];
    for (unsigned j = 0; j < length; j++)
        newData[j] = data[j];
    return new IrSequence(newData, length);
}

void Mode2LircDevice::mode2dump(std::ostream& stream) {
    while (true) {
        lirc_t t = read();
        if (t == 0) {
            std::cerr << "0 received" << std::endl;
            return;
        }
        LircT::dump(t, stream);
        stream << std::endl;
    }
}

int Mode2LircDevice::write(const uint32_t *data, size_t length) {
    if (!canSendPulse())
        return ENOTSUP;

    return ::write(fileDescriptor, data, length);
}

bool Mode2LircDevice::sendOdd(const lirc_t *data, unsigned length) {
    if (!canSendPulse())
        return false;

    size_t toSend = length * sizeof(lirc_t);
    size_t sent = 0;

    while (sent < toSend) {
        int n = ::write(fileDescriptor, data + sent/sizeof(lirc_t), toSend - sent);
        if (n < 0)
            return false;
        sent += n;
    }
    return true;
}

bool Mode2LircDevice::send(const lirc_t *data, unsigned length) {
    if (length % 2 != 0)
        return sendOdd(data, length);

    lirc_t newData[length + 1];
    memcpy(newData, data, length*sizeof(lirc_t));
    newData[length] = 1;
    return sendOdd(newData, length+1);
}

bool Mode2LircDevice::send(const IrSequence& irSequence) {
    return send((lirc_t*)irSequence.getDurations(), irSequence.getLength());
}

bool Mode2LircDevice::send(const lirc_t *data, unsigned length, frequency_t frequency) {
    if (canSetSendCarrier()) {
        int status = setSendCarrier(frequency);
        if (status == IOCTL_ERROR) {
            std::cerr << "Could not set sending frequency" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Device does not support setting sending frequency" << std::endl;
        return false;
    }
    return send(data, length);
}

//bool Mode2LircDevice::send(const lirc_t *data, unsigned length, frequency_t frequency,
//        dutycycle_t dutyCycle) {
//    if (canSetSendDutyCycle()) {
//        int status = setSendDutyCycle(dutyCycle);
//        if (status == IOCTL_ERROR) {
//            std::cerr << "Could not set sending duty cycle" << std::endl;
//            return false;
//        }
//    } else {
//        std::cerr << "Device does not support setting duty cycle" << std::endl;
//        return false;
//    }
//    return send(data, length, frequency);
//}
