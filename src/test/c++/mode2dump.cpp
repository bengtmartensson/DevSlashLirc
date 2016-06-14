#include "Mode2LircDevice.h"

#define LIRC_DEVICE "/dev/lirc0"

/*
 *
 */
int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    Mode2LircDevice lirconian;
    lirconian.open();
    lirconian.report();

    std::cerr << "Now fire IR at the receiver. This program loops forever, and must be interrupted with Ctrl-C." << std::endl;

    lirconian.mode2dump();
}
