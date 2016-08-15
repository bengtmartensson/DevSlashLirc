#include "LircDevice.h"
#include "Mode2LircDevice.h"
#include "LircCodeLircDevice.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>

const char *LircDevice::defaultFilename = "/dev/lirc0";
const char *LircDevice::version = "LircDevice " VERSION;

static const int IOCTL_OK = 0;
static const int IOCTL_ERROR = -1;

LircDevice::LircDevice(const char *path, milliseconds_t beginTimeout_) : valid(false),fileName(path),fileDescriptor(-1),beginTimeout(beginTimeout_) {
}

bool LircDevice::open() {
    fileDescriptor = ::open(fileName, O_RDWR);
    if (fileDescriptor < 0) {
        std::cerr << "Error: Cannot open " << fileName << std::endl;
        return false;
    }

    struct stat s;
    int status = fstat(fileDescriptor, &s);
    if (status == IOCTL_ERROR) {
        std::cerr << "Error: Cannot stat " << fileName << std::endl;
        return false;
    }

    if (!S_ISCHR(s.st_mode)) {
        std::cerr << "Error: " << fileName << " is not a character device" << std::endl;
        return false;
    }

    status = ::ioctl(fileDescriptor, LIRC_GET_REC_MODE, &recordingMode);
    if (status != IOCTL_OK) {
        std::cerr << "Error: cannot get recording mode" << status << std::endl;
        return false;
    }

    if ((recordingMode != LIRC_MODE_MODE2) && (recordingMode != LIRC_MODE_LIRCCODE)) {
        std::cerr << "This cannot happen!" << std::endl;
        return false;
    }

    status = ::ioctl(fileDescriptor, LIRC_GET_FEATURES, &features);
    if (status != IOCTL_OK) {
        std::cerr << "Error: " << status << std::endl;
        return false;
    }

    numberTransmitters =
            canSetTransmitterMask() ? setTransmitterMask(UINT32_MAX)
            : canSend() ? 1
            : 0;

    valid = true;
    return true;
}

LircDevice::LircDevice(const LircDevice& orig) :
valid(orig.valid),
fileName(orig.fileName),
fileDescriptor(orig.fileDescriptor),
numberTransmitters(orig.numberTransmitters),
recordingMode(orig.recordingMode),
features(orig.features),
beginTimeout(orig.beginTimeout) {
}

LircDevice::~LircDevice() {
    close();
}

bool LircDevice::close() {
    if (fileDescriptor < 0)
        return false;

    int status = ::close(fileDescriptor);
    if (status != IOCTL_OK)
        std::cerr << "Error when closing " << fileName << ": " << status << std::endl;
#ifdef DEBUG
    else
        std::cerr << "Successful close of " << fileName << std::endl;
#endif
    fileDescriptor = -1;
    valid = false;
    return status == IOCTL_OK;
}

int LircDevice::ioctl(uint32_t cmd, uint32_t arg) {
    return ::ioctl(fileDescriptor, cmd, &arg);
}

void LircDevice::report(const char *name, bool value, std::ostream& stream) const {
    if (longReport)
        stream << name << ": " << (value ? "true" : "false") << std::endl;
    else
        if (value)
            stream << name << std::endl;
}

void LircDevice::report(std::ostream& stream) const {
    if (!valid)
        return;

    stream << "Number of transmitters = " << getNumberTransmitters() << std::endl;
    report("canSendPulse", canSendPulse(), stream);
    report("canSendLircCode", canSendLircCode(), stream);
    report("canSetTransmitterMask", canSetTransmitterMask(), stream);
    report("canRecMode2", canRecMode2(), stream);
    report("canRecLircCode", canRecLircCode(), stream);
    report("canSetRecTimeout", canSetRecTimeout(), stream);

#ifdef SUPPORT_NEVER_IMPLEMENTED_OPTIONS
    report("canSendRaw", canSendRaw(), stream);
    report("canSendMode2", canSendMode2(), stream);
    report("canRecRaw", canRecRaw(), stream);
    report("canRecPulse", canRecPulse(), stream);
    report("canSetRecCarrier", canSetRecCarrier(), stream);
    report("canSetRecDutyCycle", canSetRecDutyCycle(), stream);
    report("canSetRecDutyCycleRange", canSetRecDutyCycleRange(), stream);
    report("canSetRecCarrierRange", canSetRecCarrierRange(), stream);
    report("canSetRecFilter", canSetRecFilter(), stream);
    report("canNotifyDecoder", canNotifyDecoder(), stream);
    report("canMeasureCarrier", canMeasureCarrier(), stream);
    report("canUseWidebandReceiver", canUseWidebandReceiver(), stream);
#endif //SUPPORT_NEVER_IMPLEMENTED_OPTIONS
}

bool LircDevice::reportValidity(std::ostream& stream) const {
    stream << (valid ? "VALID" : "INVALID") << std::endl;
    return valid;
}
