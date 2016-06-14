#pragma once

#include <media/lirc.h>
#include <iostream>
#include "InfraredTypes.h"

// In the future, this may be turned into an abstract class having derived classes
// FileLircDevice, FIFOLircDevice, TcpLircDevice etc.
// For now, this is "FileLirdDevice" supporting /dev/lirc as described
// by lirc(4).)

// The lirc(4) contains a number of never implemented options, like
// LIRC_CAN_REC_PULSE and LIRC_CAN_SET_REC_CARRIER_RANGE. I believe that this
// project is more useful if these are NOT supported. To support (a subset) of
// them, define the following symbol.

//#define SUPPORT_NEVER_IMPLEMENTED_OPTIONS

/**
 * This abstract class contains the common functionality of the derived classes.
 * It opens a file system device, per default /dev/lirc0, as described in lirc(4).
 */
class LircDevice {
protected:
    /** Flag indicating how to format reports */
    static const bool longReport = false;

    bool valid;
    const char *fileName;
    int fileDescriptor;
    unsigned numberTransmitters;
    uint32_t recordingMode;  //LIRC_MODE_MODE2 or LIRC_MODE_LIRCCODE
    uint32_t features;

    void report(const char *name, bool value, std::ostream& stream) const;
    bool reportValidity(std::ostream &stream = std::cout) const;

    int ioctl(uint32_t cmd, uint32_t arg);

    bool canDo(uint32_t mask) const { return mask & features; }

    LircDevice(const char *path);
    LircDevice(const LircDevice& orig);
    virtual bool open();
    bool close();

public:
    static const char *version;
    static const char *defaultFilename; // = "/dev/lirc0";
    static const uint32_t INVALID = UINT32_MAX;

    virtual ~LircDevice();

//    /**
//     * This factory method delivers an instance of a subclass,
//     * determined at runtime according to the connected hardware.
//     * @param path File system path or lird device
//     * @return instance
//     */
//    static LircDevice *newLircDevice(const char *path = defaultFilename);

    bool isValid() const { return valid; }

    /**
     * Prints a listing of the properties implemented on the stream supplied.
     * @param stream
     */
    virtual void report(std::ostream& stream = std::cout) const;

    virtual bool isMode2() const = 0;
    virtual bool isLircCode() const = 0;

    bool canSend() const { return canDo(LIRC_CAN_SEND_MASK); }
    bool canSetTransmitterMask() const { return canDo(LIRC_CAN_SET_TRANSMITTER_MASK); }
    bool canRec() const { return canDo(LIRC_CAN_REC_MASK); }

    // These are sort-of redundant, but let's leave them in
    bool canSendPulse() const { return canDo(LIRC_CAN_SEND_PULSE); }
    bool canSendLircCode() const { return canDo(LIRC_CAN_SEND_LIRCCODE); }
    bool canRecMode2() const { return canDo(LIRC_CAN_REC_MODE2); }
    bool canRecLircCode() const { return canDo(LIRC_CAN_REC_LIRCCODE); }


    /**
     * Return the number of transmitters as reported from the device.
     * @return Number transmitters.
     */
    unsigned getNumberTransmitters() const { return numberTransmitters; }

    /** Enable the set of transmitters specified in val, which  contains
    a  bit  mask  where  each enabled transmitter is a 1.  The first
    transmitter is encoded by the least significant bit, and so  on.
    When an invalid bit mask is given, for example a bit is set even
    though the device does not have so many transmitters, this operation
    returns  the  number  of  available transmitters and does
    nothing otherwise.
    */
    int setTransmitterMask(uint32_t mask) {
        return ioctl(LIRC_SET_TRANSMITTER_MASK, mask);
    }

#ifdef SUPPORT_NEVER_IMPLEMENTED_OPTIONS

    bool canSetRecTimeout() const {
        return canDo(LIRC_CAN_SET_REC_TIMEOUT);
    }
    bool canSendRaw() const {
        return canDo(LIRC_CAN_SEND_RAW);
    }

    bool canSendMode2() const {
        return canDo(LIRC_CAN_SEND_MODE2);
    }

    bool canRecRaw() const {
        return canDo(LIRC_CAN_REC_RAW);
    }

    bool canRecPulse() const {
        return canDo(LIRC_CAN_REC_PULSE);
    }

    bool canSetRecCarrier() const {
        return canDo(LIRC_CAN_SET_REC_CARRIER);
    }

    bool canSetRecDutyCycle() const {
        return canDo(LIRC_CAN_SET_REC_DUTY_CYCLE);
    }

    bool canSetRecDutyCycleRange() const {
        return canDo(LIRC_CAN_SET_REC_DUTY_CYCLE_RANGE);
    }

    bool canSetRecCarrierRange() const {
        return canDo(LIRC_CAN_SET_REC_CARRIER_RANGE);
    }

    bool canSetRecFilter() const {
        return canDo(LIRC_CAN_SET_REC_FILTER);
    }

    bool canNotifyDecoder() const {
        return canDo(LIRC_CAN_NOTIFY_DECODE);
    }

    // This would have been nice, but unfortunately,
    // no lirc hardware implements it...
    bool canMeasureCarrier() const {
        return canDo(LIRC_CAN_MEASURE_CARRIER);
    }

    bool canUseWidebandReceiver() const {
        return canDo(LIRC_CAN_USE_WIDEBAND_RECEIVER);
    }

    bool canRecLircCode() const {
        return canDo(LIRC_CAN_REC_LIRCCODE);
    }

#endif // SUPPORT_NEVER_IMPLEMENTED_OPTIONS
};

// There is a number of never implemented options in the API. We do not
// ("presenty") implement these. For reference, they are listed here:
// (copied from lirc.h)

//    LIRC_SET_REC_MODE (int)
//              Set  the  receive  mode.   val  is  either LIRC_MODE_LIRCCODE or
//              LIRC_MODE_MODE2.
//
//       LIRC_GET_LENGTH (void)
//              Return the length of the returned codes for  LIRC_MODE_LIRCCODE-
//              type drivers, otherwise fail with the error ENOIOCTLCMD.
//
//       LIRC_GET_SEND_MODE (void)
//              Return  the  send  mode.  Currently only LIRC_MODE_PULSE is sup-
//              ported.
//
//       LIRC_SET_SEND_MODE (int)
//              Set the send mode.   Currently  serves  no  purpose  since  only
//              LIRC_MODE_PULSE is supported.

// Implemented in ModeLircDevice:
//    /** Set  the  modulation  frequency.   The argument is the frequency (Hz).*/
//    int setSendCarrier(int frequency) {
//        return sendIoctl(LIRC_SET_SEND_CARRIER, frequency);
//    }
//
// Implemented in ModeLircDevice:
//    Set the carrier duty cycle.   val  is  a  number  in  the  range
//    [0,100]  which  describes the pulse width as a percentage of the
//    total cycle.  Currently, no special meaning is defined for 0  or
//    100, but the values are reserved for future use.*/
//    int setSendDutyCycle(int dutyCycle) {
//        return sendIoctl(LIRC_SET_SEND_DUTY_CYCLE, dutyCycle);
//    }

//       LIRC_GET_MIN_TIMEOUT (void), LIRC_GET_MAX_TIMEOUT (void)
//              Some  devices  have  internal  timers that can be used to detect
//              when there's no IR activity for a  long  time.   This  can  help
//              lircd(8)  in  detecting  that  an  IR signal is finished and can
//              speed up the decoding process.  These operations return  integer
//              values   with  the  minimum/maximum  timeout  that  can  be  set
//              (microseconds).  Some devices have a fixed  timeout.   For  such
//              drivers,   LIRC_GET_MIN_TIMEOUT  and  LIRC_GET_MAX_TIMEOUT  will
//              return the same value.
//
//       LIRC_SET_REC_TIMEOUT (int)
//              Set the integer value for IR inactivity timeout  (microseconds).
//              To  be  accepted, the value must be within the limits defined by
//              LIRC_GET_MIN_TIMEOUT and LIRC_GET_MAX_TIMEOUT.  A value of 0 (if
//              supported  by  the  hardware) disables all hardware timeouts and
//              data should be reported as soon as possible.  If the exact value
//              cannot  be  set,  then  the next possible value greater than the
//              given value should be set.
//
//       LIRC_SET_REC_TIMEOUT_REPORTS (int)
//              Enable (val is 1) or disable (val  is  0)  timeout  packages  in
//              LIRC_MODE_MODE2.   By  default, timeout reports should be turned
//              off.
//
//       LIRC_SET_REC_CARRIER (int)
//              Set the receive carrier frequency (Hz).
//
//       LIRC_SET_REC_CARRIER_RANGE (int)
//              After opening device, the first use of this operation  sets  the
//              lower  bound  of  the carrier range, and the second use sets the
//              upper bound (Hz).
//
//       LIRC_SET_MEASURE_CARRIER_MODE (int)
//              Enable (val is 1) or disable (val is 0) the  measure  mode.   If
//              enabled,  from  the  next  key  press  on,  the driver will send
//              LIRC_MODE2_FREQUENCY packets.  By default this should be  turned
//              off.
//
//       LIRC_GET_REC_RESOLUTION (void)
//              Return the driver resolution (microseconds).
//
//       LIRC_GET_MIN_FILTER_PULSE (void), LIRC_GET_MAX_FILTER_PULSE (void)
//              Some  devices are able to filter out spikes in the incoming sig-
//              nal using given filter rules.  These ioctls return the  hardware
//              capabilities  that  describe the bounds of the possible filters.
//              Filter settings depend on the IR protocols  that  are  expected.
//              lircd(8)  derives  the  settings  from all protocols definitions
//              found in its lircd.conf(5) config file.
//
//       LIRC_GET_MIN_FILTER_SPACE (void), LIRC_GET_MAX_FILTER_SPACE (void)
//              See LIRC_GET_MIN_FILTER_PULSE.
//
//       LIRC_SET_REC_FILTER (int)
//              Pulses/spaces shorter than this (microseconds) are filtered  out
//              by hardware.
//
//    LIRC_SET_REC_FILTER_PULSE (int), LIRC_SET_REC_FILTER_SPACE (int)
//              Pulses/spaces  shorter than this (microseconds) are filtered out
//              by  hardware.   If  filters  cannot  be  set  independently  for
//              pulse/space,  the  corresponding ioctls must return an error and
//              LIRC_SET_REC_FILTER should be used instead.

//       LIRC_SET_WIDEBAND_RECEIVER (int)
//              Some devices are equipped with  a  special  wide  band  receiver
//              which  is intended to be used to learn the output of an existing
//              remote.  This ioctl can be used to enable (val equals 1) or dis-
//              able  (val  equals  0) this functionality.  This might be useful
//              for devices that otherwise have narrow band receivers that  pre-
//              vent  them to be used with certain remotes.  Wide band receivers
//              may also be more precise.  On the other  hand  its  disadvantage
//              usually is reduced range of reception.
//
//              Note: wide band receiver may be implicitly enabled if you enable
//              carrier reports.  In that case, it will be disabled as  soon  as
//              you  disable  carrier  reports.   Trying  to disable a wide band
//              receiver while carrier reports are active will do nothing.
//
//       LIRC_SETUP_START (void), LIRC_SETUP_END (void)
//              Setting of several driver parameters can be optimized by  brack-
//              eting  the  actual  ioctl  calls  LIRC_SETUP_START and LIRC_SET-
//              UP_END.  When a driver receives a LIRC_SETUP_START ioctl, it can
//              choose  to  not  commit  further setting changes to the hardware
//              until a LIRC_SETUP_END is received.  But this  is  open  to  the
//              driver  implementation and every driver must also handle parame-
//              ter changes which are not encapsulated by  LIRC_SETUP_START  and
//              LIRC_SETUP_END.  Drivers can also choose to ignore these ioctls.
//
//       LIRC_NOTIFY_DECODE (void)
//              This  ioctl is called by lircd(8) whenever a successful decoding
//              of an incoming IR signal is possible.  This can be used by  sup-
//              porting  hardware  to  give visual user feedback, for example by
//              flashing an LED.

