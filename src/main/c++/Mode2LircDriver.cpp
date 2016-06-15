#include "Mode2LircDriver.h"

Mode2LircDriver* Mode2LircDriver::newInstance(const char* name) {
    if (LircDriver::getInstance() != NULL)
        return NULL;

    Mode2LircDriver* instance = new Mode2LircDriver(name);
    setInstance(instance->isValid() ? instance : NULL);
    return getInstance();
}

Mode2LircDriver* Mode2LircDriver::newInstance(const char* name, const char* filename) {
    if (LircDriver::getInstance() != NULL)
        return NULL;

    Mode2LircDriver* instance = new Mode2LircDriver(name, filename);
    setInstance(instance);
    return instance;
}

Mode2LircDriver* Mode2LircDriver::getInstance() {
    LircDriver* instance = LircDriver::getInstance();
    return instance == NULL ? NULL
           : instance->isMode2() ? (Mode2LircDriver*) instance : NULL;
}

Mode2LircDriver::Mode2LircDriver(const char* name, const char* path) : LircDriver(name, path) {
}

Mode2LircDriver::Mode2LircDriver(const char* name) : LircDriver(name) {
}

Mode2LircDriver::~Mode2LircDriver() {
}

IrSequence* Mode2LircDriver::receive() {
    if (!canRec())
        return NULL;
    if (!assureReady())
        return NULL;
    unsigned int index = 0;
    //unsigned int totalDuration = 0;
    //std::vector<lirc_t>durations;
    microseconds_t durations[maxReceiveLength];
    lirc_t first;
    do {
        first = lircDriver->readdata(receiveBeginTimeout);
        if (first == 0U) {
            logprintf(LIRC_TRACE1, "Empty sequence received");
            deinit();
            return new IrSequence();
        }
    } while (LircT::isSpace(first));
    durations[index++] = LircT::getDuration(first);
    //durations.push_back(first);
    lirc_t dur;
    do {
        dur = lircDriver->readdata(receiveEndingTimeout);
        if (dur == 0) // timeout
            dur = receiveEndingTimeout;
        else
            durations[index++] = LircT::getDuration(dur);
        //totalDuration += dur;
    } while (index < maxReceiveLength // Total signal is too long
	    && ! LircT::isTerminal(dur)
            && LircT::getDuration(dur) < receiveEndingTimeout // last duration was an ending time out
            );
    deinit();
    // de-sillify
    if (LircT::isTerminal(durations[index-1]))
        durations[index-1] = receiveEndingTimeout;
    IrSequence* seq = new IrSequence(durations, index);

    const char* s = seq->toString();
    logprintf(LIRC_INFO, "Sequence received: %s", s);
    delete [] s;
    deinit();
    return seq;
}


//bool Mode2LircDriver::setSendCarrier(frequency_t frequency) {
//    if (!assureReady())
//        return false;
//    int result = lircDriver->drvctl_func(LIRC_SET_SEND_CARRIER, &frequency);
//    return result == 0;
//}

bool Mode2LircDriver::send(const IrSequence &seq, frequency_t frequency) {
    const char* s = seq.toString();
    logprintf(LIRC_TRACE1, "sending sequence %s", s);
    delete [] s;
    return send(seq.getDurations(), seq.getLength(), frequency);
}

bool Mode2LircDriver::send(const microseconds_t durations[], size_t length, frequency_t frequency) {
   if (!(canSend() && assureReady()))
        return false;
//    if (canSetSendCarrier())
//        setSendCarrier(frequency);
    int *data = NULL;
    struct ir_ncode code;
    memset(&code, 0, sizeof (struct ir_ncode));
    code.name = const_cast<char*> ("dummy_code"); //TOA2B (the others are to blame)
    if (length % 1 != 0) {
        code.signals = (int*) durations; //TOA2B
        code.length = length;
    } else {
        data = new int[length + 1];
        for (unsigned i = 0; i < length; i++)
            data[i] = (int) durations[i];
        data[length] = 1;
        code.signals = data;
        code.length = length + 1;
    }

    struct ir_remote remote;
    memset(&remote, 0, sizeof (struct ir_remote));
    remote.name = "dummy_remote";
    remote.flags = RAW_CODES;
    remote.freq = frequency;

    bool result = lircDriver->send_func(&remote, &code);
    if (data != NULL)
        delete [] data;

    deinit();
    return result;
}
