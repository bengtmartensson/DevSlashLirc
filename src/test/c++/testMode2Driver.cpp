#include <iostream>
#include "Mode2LircDriver.h"

using namespace std;
const microseconds_t nec1_122_29[] = {
    9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 39756
 };

int main(int argc, char** argv) {
    IrSequence* yama_power_on = new IrSequence(nec1_122_29, sizeof(nec1_122_29)/sizeof(microseconds_t));
    LircDriver::setupLog();
    //LircDriver* lircDriver = LircDriver::newLircDriver("girs");
    //cout << lircDriver->setDeviceName("/dev/arduino") << endl;
    //LircDriver* lircDriver = LircDriver::newLircDriver("commandir");
    //const char* driverName = argc > 1 ? argv[1] : "default";
    Mode2LircDriver* lircDriver = argc > 1 ? Mode2LircDriver::newInstance(argv[1]) : Mode2LircDriver::newInstance();
    cout << lircDriver->getVersion() << endl;
    bool success = argc > 2 ? lircDriver->open(argv[2]) : lircDriver->open();
    cout << "Open " << (success ? "succeeded" : "failed") << endl;
    cout << lircDriver->canGetRecResolution() << endl;
    cout << lircDriver->getRecResolution() << endl;
    success = lircDriver->init();
    cout << "Init " << (success ? "succeeded" : "failed") << endl;

    if (success) {
        cout << "canSetTransmitterMask = " << lircDriver->canSetTransmitterMask() << endl;
        cout << "isReady = " << lircDriver->isReady() << endl;
        cout << "deInit returned " << lircDriver->deinit() << endl;
        cout << "isReady = " << lircDriver->isReady() << endl;

        cout << ">>>>>>>>>>>>>> Now fire an IR signal" << endl;
        IrSequence* irSequence = lircDriver->receive();
        if (irSequence == NULL || irSequence->isEmpty())
            cout << "Nuthin..." << endl;
        else
            irSequence->dumpWithSigns();

        cout << "setTransmitterMask(2) returned " << lircDriver->setTransmitterMask(2) << endl;

        cout << "lircDriver->send(*yama_power_on, 38400) returned "
                << lircDriver->send(*yama_power_on, 38400) << endl;
    }

    Mode2LircDriver::deleteInstance();
    LircDriver::closeLog();
}

