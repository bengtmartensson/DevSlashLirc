#include "Mode2LircDevice.h"

#define LIRC_DEVICE "/dev/lirc0"

/*
 *
 */
int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    frequency_t nec1_frequency = 38400;
    microseconds_t nec1_122_29[] = {
        9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 39756
    };

    IrSequence irSequence(nec1_122_29, sizeof(nec1_122_29)/sizeof(microseconds_t));

    Mode2LircDevice lirconian;
    lirconian.open();
    lirconian.report();
    lirconian.setTransmitterMask(1);
    std::cout << lirconian.send(irSequence, nec1_frequency) << std::endl;
}
