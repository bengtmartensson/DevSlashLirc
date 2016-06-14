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

public:
    static const frequency_t defaultFrequency = 38000;
    static const microseconds_t replacementEndingSilence = 100000;

    Mode2LircDevice(const char *path = defaultFilename);
    //Mode2LircDevice(const LircDevice& orig);
    //virtual ~Mode2LircDevice();

    virtual bool open();

    bool isMode2() const { return true; }
    bool isLircCode() const { return false; }

    void report(std::ostream& stream = std::cout) const;

    microseconds_t getRecResolution() const { return resolution; }

    bool canSetSendCarrier() const { return canDo(LIRC_CAN_SET_SEND_CARRIER); }
    bool canGetRecResolution() const { return canDo(LIRC_CAN_GET_REC_RESOLUTION); }

    /**
     * Reads exactly one duration. Blocks.
     * @return duration as lirc_t.
     */
    lirc_t read();

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
     * @param length, required to be odd.
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

    bool send(const lirc_t *data, unsigned int length, frequency_t frequency);

    /** Set  the  modulation  frequency.   The argument is the frequency (Hz).*/
    int setSendCarrier(int frequency = defaultFrequency) {
        return ioctl(LIRC_SET_SEND_CARRIER, frequency);
    }
};
