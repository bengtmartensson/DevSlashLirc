#pragma once

#include "IrSequence.h"
#include "LircT.h"
#include <stdint.h>
#include <lirc/driver.h>
#include <lirc/lirc_log.h>

class LircDriver {
public:
    static void setPlugindir(const char* newDriverPath);
    static void setupLog(const char* fileName = "/tmp/lircdriver.log", loglevel_t logLevel = LIRC_TRACE1, bool append = false);
    static void closeLog();

protected:
    static LircDriver* getInstance();
    static void setInstance(LircDriver *newDriver);
    static void deleteInstance();

    struct driver* lircDriver;
    bool assureReady();
    uint32_t getFeatures() const    { return curr_driver->features; }
    bool canDo(uint32_t mask) const { return (getFeatures() & mask) != 0; }
    LircDriver(const char* name);
    LircDriver(const char* name, const char* path);
    virtual ~LircDriver();

private:
    const char* name;
    const char* path;
    const char* deviceName;
    static LircDriver* instance;
    void* plugin;
    bool valid;
    static const char *plugindir;

    bool loadPlugin();
    bool findDriver();
    void initialize();

public:
    virtual bool isMode2() const = 0;

    bool isLircCode() const { return ! isMode2(); }

    bool isValid() const { return valid; }

    const char* getVersion() const;

    // The syntax of the driver open differs pretty much from every other
    // use of that verb...
    bool open(const char* deviceName = "auto");

    bool init();

    bool deinit();

    bool close();

    bool setTransmitterMask(uint32_t transmitterMask);

    bool isReady() const;

    bool canSend() const { return canDo(LIRC_CAN_SEND_MASK); }
    bool canSetTransmitterMask() const { return canDo(LIRC_CAN_SET_TRANSMITTER_MASK); }
    bool canRec() const { return canDo(LIRC_CAN_REC_MASK); }

    // These are sort-of useless and/or redundant, but let's leave them in
    bool canSendPulse() const { return canDo(LIRC_CAN_SEND_PULSE); }
    bool canSendLircCode() const { return canDo(LIRC_CAN_SEND_LIRCCODE); }
    bool canRecMode2() const { return canDo(LIRC_CAN_REC_MODE2); }
    bool canRecLircCode() const { return canDo(LIRC_CAN_REC_LIRCCODE); }
};
