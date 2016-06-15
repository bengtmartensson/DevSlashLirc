#pragma once

#include "IrSequence.h"
#include "LircT.h"
#include "LircDriver.h"
#include <stdint.h>
#include <lirc/driver.h>
#include <lirc/lirc_log.h>

class Mode2LircDriver : public LircDriver {
public:

    static Mode2LircDriver* newInstance(const char *name = "default");
    static Mode2LircDriver* newInstance(const char *name, const char *filename);
    static Mode2LircDriver* getInstance();
    static void deleteInstance() { LircDriver::deleteInstance(); }

private:
    static const microseconds_t receiveBeginTimeout = 10000000;
    static const microseconds_t receiveEndingTimeout =  100000;
    static const unsigned maxReceiveLength = 1000;

    Mode2LircDriver(const char* name);
    Mode2LircDriver(const char* name, const char* path);
    virtual ~Mode2LircDriver();

public:
    bool isMode2() const { return true; }
    IrSequence* receive();
    bool send(const microseconds_t durations[], size_t length, frequency_t frequency);
    bool send(const IrSequence &seq, frequency_t frequency);

    // questionable; IrRemote contains the frequency.
    //bool setSendCarrier(frequency_t frequency);
    //bool canSetSendCarrier() const { return canDo(LIRC_CAN_SET_SEND_CARRIER); }
    bool canGetRecResolution() const { return canDo(LIRC_CAN_GET_REC_RESOLUTION); }
    microseconds_t getRecResolution() const { return curr_driver->resolution; }
};
