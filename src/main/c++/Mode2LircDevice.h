#pragma once

#include "LircDevice.h"
#include "IrSequence.h"

/**
 * This class is a LIRC_MODE_MODE2 device as per lirc(4).
 */
class Mode2LircDevice : public LircDevice {
private:
    microseconds_t resolution;

    bool sendOdd(const lirc_t *data, unsigned length);

protected:
    milliseconds_t endTimeout;
    size_t captureSize;

public:
    static const frequency_t defaultFrequency = 38000U;
    static const microseconds_t replacementEndingSilence = 100000U;
    static const milliseconds_t defaultEndTimeout = 200U;
    static const size_t defaultCaptureSize = 250;

    //Mode2LircDevice(const char *path = defaultFilename);
    Mode2LircDevice(const char *path = defaultFilename = defaultFilename,
            milliseconds_t beginTimeout = defaultBeginTimeout,
            size_t captureSize = defaultCaptureSize,
            milliseconds_t endTimeout = defaultEndTimeout);
    //Mode2LircDevice(const LircDevice& orig);
    //virtual ~Mode2LircDevice();

    virtual bool open();

    bool isMode2() const { return true; }
    bool isLircCode() const { return false; }

    void report(std::ostream& stream = std::cout) const;

    microseconds_t getRecResolution() const { return resolution; }

    bool canSetSendCarrier() const { return canDo(LIRC_CAN_SET_SEND_CARRIER); }
    bool canGetRecResolution() const { return canDo(LIRC_CAN_GET_REC_RESOLUTION); }

    milliseconds_t getEndTimeout() const {
        return endTimeout;
    }

    void setEndTimeout(int timeout) {
        endTimeout = timeout;
        setRecTimeout(1000U * endTimeout); // do not care to check status
    }

    size_t getCaptureSize() const {
        return captureSize;
    }

    void setCaptureSize(size_t captureSize_) {
        captureSize = captureSize_;
    }

    /**
     * Reads exactly one duration. Blocks.
     * @return duration as lirc_t.
     */
    lirc_t read(int timeout);

    /**
     * Reads and dumps readings in mode2 format.
     * @param stream
     */
    void mode2dump(std::ostream& stream = std::cout);

    /**
     * Reads a sequence. The mechanism for the ending may be device dependent,
     * but in general returns a sane IrSequence.
     * @return received IrSequence
     */
    IrSequence *receive();

    /**
     * A low-level interface to the write(2) call on the file descriptor.
     *
     * @param data data to be written
     * @param length of data
     * @return number or characters actually written.
     */
    int write(const uint32_t *data, size_t length);

    /**
     * Mid-level sending routine. Sends the data.
     * @param data
     * @param length; required to be odd.
     * @return success of operation
     */
    bool send(const lirc_t *data, unsigned length);

    /**
     * High-level sending routine. Sends the irSequence with the requested
     * frequency and duty cycle.
     * @param irSequence
     * @return success of operation
     */
    bool send(const IrSequence& irSequence);

    /**
     * High-level sending routine. Sends the irSequence with the requested
     * frequency.
     * @param irSequence
     * @param frequency
     * @return success of operation
     */
    bool send(const IrSequence& irSequence, frequency_t frequency) {
        return send((lirc_t *) irSequence.getDurations(), irSequence.getLength(), frequency);
    }

    /**
     * High-level sending routine. Sends the irSequence with the requested
     * frequency.
     * @param data
     * @param length
     * @param frequency
     * @return success of operation
     */
    bool send(const lirc_t *data, unsigned length, frequency_t frequency);

    /** Set  the  modulation  frequency.   The argument is the frequency (Hz).*/
    int setSendCarrier(int frequency = defaultFrequency) {
        return ioctl(LIRC_SET_SEND_CARRIER, frequency);
    }
};
