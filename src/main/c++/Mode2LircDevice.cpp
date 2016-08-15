#include "Mode2LircDevice.h"
#include "LircT.h"

#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <poll.h>

static const int IOCTL_OK = 0;
static const int IOCTL_ERROR = -1;

Mode2LircDevice::Mode2LircDevice(const char *path, milliseconds_t beginTimeout, size_t captureSize_, milliseconds_t endTimeout_)
    : LircDevice(path, beginTimeout),endTimeout(endTimeout_),captureSize(captureSize_) {
}

bool Mode2LircDevice::open() {
    bool status = LircDevice::open();
    if (!status)
        return false;

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

    setRecTimeout(1000U * endTimeout); // do not care to check status
    setRecTimeoutReports(true); // do not care to check status

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

/**
 * Read a number.
 * @param timeout timeout in milliseconds; 0 for no timeout (wait forever)
 * @return
 */
lirc_t Mode2LircDevice::read(int timeout) {
    if (!canRecMode2()) {
        return 0;
    }

    pollfd pfd = {
        .fd = fileDescriptor,
        .events = POLLIN,
        .revents = 0
    };
    int rc = poll(&pfd, 1, timeout ? timeout : -1);
    if (rc < 0)
        return 0;
    if (rc == 0)
        return LircT::LircTType::TIMEOUT;

    lirc_t data;
    ssize_t actualLength = ::read(fileDescriptor, &data, sizeof(lirc_t));
    return actualLength == sizeof(lirc_t) ? data : 0;
}

IrSequence* Mode2LircDevice::receive() {
    microseconds_t data[captureSize];
    unsigned index = 0;
    while (index < captureSize) {
        lirc_t t = read(index == 0 ? beginTimeout : endTimeout);
        if (t == 0)
            return new IrSequence();

        LircT lircT(t);
        if (lircT.isTerminal(t)) {
            if (index > 0)
                data[index++] = replacementEndingSilence;
            break;
        }

        if (index > 0 || lircT.isPulse()) // ignore leading spaces
            data[index++] = lircT.getDuration(t);
    }
    if (index == 0)
        return new IrSequence();

    microseconds_t* newData = new microseconds_t[index];
    for (unsigned j = 0; j < index; j++)
        newData[j] = data[j];
    return new IrSequence(newData, index);
}

void Mode2LircDevice::mode2dump(std::ostream& stream) {
    while (true) {
        lirc_t t = read(0);
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
