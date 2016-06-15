#include "LircDriver.h"

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

// Belonging to driver.c, exported, but not in driver.h. Sigh...
extern struct driver drv;

#if 0
//struct driver drv = {
//	/*.name		*/ NULL,
//	/*.device		*/ NULL,
//	/*.features	*/ 0,
//	/*.send_mode	*/ 0,
//	/*.rec_mode	*/ 0,
//	/*.code_length	*/ 0,
//	/*.init_func	*/ NULL,
//	/*.deinit_func	*/ NULL,
//	/*.open_func	*/ NULL,
//	/*.close_func	*/ NULL,
//	/*.send_func	*/ NULL,
//	/*.rec_func	*/ NULL,
//	/*.decode_func	*/ NULL,
//	/*.drvctl_func	*/ NULL,
//	/*.readdata	*/ NULL,
//		NULL,
//		NULL,
//	/*.api_version	*/ 0,
//	/*.driver_version */ NULL,
//	/*.info		*/ NULL,
//	/*.device_hint    */ 0
//};

struct driver drv = {
// Old-style implicit API version 1:

	/**
	 * Name of the device (string). Set by open_func() before init(),
	 * possibly using the hard-coded driver default value.
	 */
	//const char* device;
	NULL,

	/** Set by the driver after init(). */
	//int		fd;
	0,

	/** Code for the features of the present device, valid after init(). */
	//__u32		features;
	0UL,

	/**
	 * Possible values are: LIRC_MODE_RAW, LIRC_MODE_PULSE, LIRC_MODE_MODE2,
	 * LIRC_MODE_LIRCCODE. These can be combined using bitwise or.
	 */
	//__u32		send_mode;
	0UL,

	/**
	 * Possible values are: LIRC_MODE_RAW, LIRC_MODE_PULSE, LIRC_MODE_MODE2,
	 * LIRC_MODE_LIRCCODE. These can be combined using bitwise or.
	 */
	//__u32		rec_mode;
	0UL,

	/** Length in bits of the code. */
	//const __u32	code_length;
	0UL,

	 /**
	 *  Function called to do basic driver setup.
	 *  @param device String describing what device driver should
	 *      communicate with. Often (but not always) a /dev/... path.
	 *  @return 0 if everything is fine, else positive error code.
	 */
	//int (*const open_func) (const char* device);
	NULL,

	/**
	 * Function called for initializing the driver and the hardware.
	 * Zero return value indicates failure, all other return values success.
	 */
	//int (*const init_func)(void);
	NULL,

	/**
	 * Function called when transmitting/receiving stops. Zero return value
	 *  indicates failure, all other return values success.
	 */
	//int (*const deinit_func) (void);
	NULL,

	/**
	 * Send data to the remote.
	 * @param remote The remote used to send.
	 * @param code Code(s) to send, a single code or the head of a
	 *             list of codes.
	 */
	//int (*const send_func)(struct ir_remote* remote,
	//		       struct ir_ncode* code);
	NULL,

	/**
	 * Receive data from remote. Might close device on error conditions.
	 * @param The remote to read from.
	 * @return Formatted, statically allocated string with decoded
	 *         data: "remote-name code-name code repetitions"
	 */
	//char* (*const rec_func)(struct ir_remote* remotes);
	NULL,

	/**
	 * TODO
	 */
	//int (*const decode_func)(struct ir_remote* remote,
	//			 struct decode_ctx_t* ctx);
	NULL,

	/**
	 * Generic driver control function with semantics as defined by driver
	 * Returns 0 on success, else a positive error code.
	 */
	//int (*const drvctl_func)(unsigned int cmd, void* arg);
	NULL,

	/**
	 * Get length of next pulse/space from hardware.
	 * @param timeout Max time to wait (us).
	 * @return Length of pulse in lower 24 bits (us). PULSE_BIT
	 * is set to reflect if this is a pulse or space. 0
	 * indicates errors.
	 */
	//lirc_t (*const readdata)(lirc_t timeout);
	NULL,

	/**
	 * Driver name, as listed by -H help and used as argument to i
	 * --driver.
	 */
	//const char*	name;
	NULL,

	/**
	 * The resolution in microseconds of the recorded durations when
	 * reading signals.
	 */
	//unsigned int	resolution;
	0U,

/* API version 2 addons: */

	//const int	api_version;            /**< API version (from version 2+).*/
	0,
	//const char*	driver_version;         /**< Driver version (free text). */
	0,
	//const char*	info;                   /**< Free text driver info. */
	NULL,

	//int (*const close_func)(void);          /**< Hard closing, returns 0 on OK.*/
	NULL,

/* API version 3 addons: */
	/**
	 *  device_hint is a mean for config tools to autodetect devices.
	 *    - /dev/tty*     User selects a tty.
	 *    - /dev/usb/\*   Denotes serial, USB-connectd port.
	 *    - /dev/event\*  A devinput device
	 *    - /dev/foo\*    A wildcard listing possible devices, general
	 *                    fallback.
	 *    - /bin/sh ...   Shell command listing possible devices.
	 *    - drvctl        Driver supports DRVCTL_GET_DEVICES drvctl.
	 *    - None          No device is silently configured.
	 *    - Auto          No device configured, but a message is
	 *                    displayed.
	 */
	//const char* const  device_hint;
	NULL
};


const struct driver* const curr_driver = &drv;
#endif

const char* LircDriver::plugindir = "/usr/local/lib/lirc/plugins";

LircDriver* LircDriver::instance = NULL;

void LircDriver::setPlugindir(const char* newDriverPath) {
    char* buf = strdup(newDriverPath);
    plugindir = buf;
}

void LircDriver::setupLog(const char* fileName, loglevel_t logLevel, bool append) {
    if (strcmp("syslog", fileName) != 0 && !append) {
        int fd = ::open(fileName, O_WRONLY | O_TRUNC);
        if (fd != -1)
            ::close(fd);
    }

    lirc_log_set_file(fileName);
    (void) lirc_log_open("LircDriver", false, logLevel);
}

void LircDriver::closeLog() {
    logprintf(LIRC_INFO, "Closing log");
    (void) lirc_log_close();
}

//LircDriver* LircDriver::newLircDriver(const char *name) {
//    if (instance != NULL)
//        return NULL;
//
//    instance = new LircDriver(name);
//    return instance;
//}

void LircDriver::setInstance(LircDriver *newDriver) {
    instance = newDriver;
}

LircDriver* LircDriver::getInstance() {
    return instance;
}

void LircDriver::deleteInstance() {
    delete instance;
    instance = NULL;
}

//LircDriver::LircDriver() {
//}

//LircDriver::LircDriver(const LircDriver& orig) {
//}

LircDriver::LircDriver(const char* name_, const char* path)
: name(name_),path(path),deviceName(NULL) {
    initialize();
}

LircDriver::LircDriver(const char* name)
: name(name),deviceName(NULL) {
    std::cerr << plugindir << std::endl;
    char newPath[1000];
    strncpy(newPath, plugindir, 100);
    strncat(newPath, "/", 1);
    strncat(newPath, name, 100);
    strncat(newPath, ".so", 3);
    path = newPath;
    initialize();
}

void LircDriver::initialize() {
    plugin = NULL;
    lircDriver = NULL;
    valid = loadPlugin()
            && findDriver();
}

LircDriver::~LircDriver() {
    logprintf(LIRC_INFO, "Closing driver \"%s\".", name);
    (void) close();
    if (plugin != NULL)
        (void) dlclose(plugin);
}

bool LircDriver::close() {
    if (lircDriver == NULL)
        return true;

    return lircDriver->deinit_func() == 0 // just to be safe
        && lircDriver->close_func() == 0;
}

bool LircDriver::loadPlugin() {
    (void) dlerror();
    if (plugin != NULL)
        dlclose(plugin);
    plugin = dlopen(path, RTLD_NOW);
    if (plugin == NULL) {
        //logprintf(LIRC_ERROR, dlerror());
        std::cerr << dlerror() << std::endl;
        return false; // throw ...
    }
    return true;
}

bool LircDriver::findDriver() {
    struct driver **drivers = (struct driver**) dlsym(plugin, "hardwares");
    if (drivers == (struct driver**) NULL) {
        //logprintf(LIRC_WARNING, "No hardwares entrypoint found in %s", path);
        std::cerr << "No hardwares entrypoint found in " << path << std::endl;
        return false; // throw ...
    }

    for (; *drivers; drivers++) {
        if ((*drivers)->name == NULL) {
            //logprintf(LIRC_WARNING, "Driver without a name in %s", path);
            continue;
        }
        if (strcasecmp((*drivers)->name, name) == 0) {
            lircDriver = *drivers;
            memcpy(&drv, lircDriver, sizeof(struct driver));
            return true;
        }
    }
    // If got here, requested driver was not found
    std::cerr << "Did not find " << name << " in " << path << std::endl;
    //throw ...
    return false;
}

const char* LircDriver::getVersion() const {
    return lircDriver->driver_version;
}

bool LircDriver::open(const char* deviceName) {
    this->deviceName = deviceName;
    logprintf(LIRC_INFO, "Opening driver \"%s\".", name);
    return lircDriver->open_func(this->deviceName) == 0;
}

bool LircDriver::setTransmitterMask(uint32_t transmitterMask) {
    assureReady();
    int result = lircDriver->drvctl_func(LIRC_SET_TRANSMITTER_MASK, &transmitterMask);
    return result == 0;
}

bool LircDriver::isReady() const {
    return lircDriver != NULL && curr_driver/*lircDriver*/->fd != -1;
}

bool LircDriver::init() {
    return lircDriver->init_func == NULL
            || lircDriver->init_func() != 0;
}

bool LircDriver::deinit() {
    return lircDriver->init_func == NULL
    || lircDriver->deinit_func() != 0;
}

bool LircDriver::assureReady() {
    bool result = isReady();
    if (!result)
        init();
    return isReady();
}

//IrSequence* LircDriver::receive() {
//    if (!canRec())
//        return NULL;
//    if (!assureReady())
//        return NULL;
//    unsigned int index = 0;
//    //unsigned int totalDuration = 0;
//    //std::vector<lirc_t>durations;
//    microseconds_t durations[maxReceiveLength];
//    lirc_t first;
//    do {
//        first = lircDriver->readdata(receiveBeginTimeout);
//        if (first == 0U) {
//            logprintf(LIRC_TRACE1, "Empty sequence received");
//            deinit();
//            return new IrSequence();
//        }
//    } while (LircT::isSpace(first));
//    durations[index++] = LircT::getDuration(first);
//    //durations.push_back(first);
//    lirc_t dur;
//    do {
//        dur = lircDriver->readdata(receiveEndingTimeout);
//        if (dur == 0) // timeout
//            dur = receiveEndingTimeout;
//        else
//            durations[index++] = LircT::getDuration(dur);
//        //totalDuration += dur;
//    } while (index < maxReceiveLength // Total signal is too long
//            && LircT::getDuration(dur) < receiveEndingTimeout // last duration was an ending time out
//            );
//    deinit();
//    // de-sillify
//    if (LircT::isTerminal(durations[index-1]))
//        durations[index-1] = receiveEndingTimeout;
//    IrSequence* seq = new IrSequence(durations, index);
//
//    const char* s = seq->toString();
//    logprintf(LIRC_INFO, "Sequence received: %s", s);
//    delete [] s;
//    deinit();
//    return seq;
//}


//bool LircDriver::setSendCarrier(frequency_t frequency) {
//    if (!assureReady())
//        return false;
//    int result = lircDriver->drvctl_func(LIRC_SET_SEND_CARRIER, &frequency);
//    return result == 0;
//}

//bool LircDriver::send(const IrSequence &seq, frequency_t frequency) {
//    if (!canSend())
//        return false;
//    if (!assureReady())
//        return false;
////    if (canSetSendCarrier())
////        setSendCarrier(frequency);
//    const char* s = seq.toString();
//    logprintf(LIRC_TRACE1, "sending sequence %s", s);
//    delete [] s;
//    return send(seq.getDurations(), seq.getLength(), frequency);
//}

//bool LircDriver::send(const microseconds_t durations[], size_t length, frequency_t frequency) {
//    int *data = NULL;
//    struct ir_ncode code;
//    memset(&code, 0, sizeof (struct ir_ncode));
//    code.name = const_cast<char*> ("dummy_code"); //TOA2B (the others are to blame)
//    if (length % 1 != 0) {
//        code.signals = (int*) durations; //TOA2B
//        code.length = length;
//    } else {
//        data = new int[length + 1];
//        for (unsigned i = 0; i < length; i++)
//            data[i] = (int) durations[i];
//        data[length + 1] = 1;
//        code.signals = data;
//        code.length = length + 1;
//    }
//
//    struct ir_remote remote;
//    memset(&remote, 0, sizeof (struct ir_remote));
//    remote.name = "dummy_remote";
//    remote.flags = RAW_CODES;
//    remote.freq = frequency;
//
//    bool result = lircDriver->send_func(&remote, &code);
//    if (data != NULL)
//        delete [] data;
//
//    deinit();
//    return result;
//}
