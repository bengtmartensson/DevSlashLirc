#include "Mode2LircDevice.h"
#include <unistd.h>

#define LIRC_DEVICE "/dev/lirc0"

/*
 *
 */
int main(int argc, char** argv) {
    const milliseconds_t beginTimeout = 10000U;
    unsigned int secs = 0U;
    if (argc > 1)
        secs = atoi(argv[1]);

    Mode2LircDevice lirconian;
    lirconian.setBeginTimeout(beginTimeout);
    lirconian.open();
    lirconian.report();
    if (secs) {
        std::cerr << "Opened, waiting for " << secs << " seconds." << std::endl;
        sleep(secs);
    }

    std::cerr << "Listening, now fire an IR signal at the receiver. Timeout = " << beginTimeout << " ms." << std::endl;

    IrSequence *seq = lirconian.receive();
    if (seq != NULL) {
        if (seq->isEmpty())
            std::cerr << "Empty signal received." << std::endl;
        else
            seq->dumpWithSigns();
    } else
        std::cerr << "NULL received." << std::endl;
}
