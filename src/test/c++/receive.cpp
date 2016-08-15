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

    std::cerr << "Now fire an IR signal at the receiver." << std::endl;

    IrSequence *seq = lirconian.receive();
    if (seq != NULL)
        seq->dumpWithSigns();
    else
        std::cerr << "NULL received." << std::endl;

}
