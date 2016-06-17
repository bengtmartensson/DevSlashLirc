#include "Mode2LircDevice.h"

int main(int argc __attribute__((unused)), char** argv __attribute__((unused))) {
    frequency_t nec1_frequency = 38400;
    microseconds_t nec1_122_29[] = {
        9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692,
        564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 564,
        564, 564, 564, 564, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 1692,
        564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 564,
        564, 564, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 39756
    };

    IrSequence irSequence(nec1_122_29, sizeof(nec1_122_29)/sizeof(microseconds_t));

    Mode2LircDevice lircDevice;
    lircDevice.open();
    lircDevice.setTransmitterMask(1);
    lircDevice.send(irSequence, nec1_frequency);
}
