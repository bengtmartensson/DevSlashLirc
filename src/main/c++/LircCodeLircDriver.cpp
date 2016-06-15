#include "LircCodeLircDriver.h"
#include <lirc/ir_remote.h>
#include <lirc/receive.h>

LircCodeLircDriver* LircCodeLircDriver::newInstance(const char* name) {
    if (LircDriver::getInstance() != NULL)
        return NULL;

    LircCodeLircDriver* instance = new LircCodeLircDriver(name);
    setInstance(instance);
    return instance;
}

LircCodeLircDriver* LircCodeLircDriver::getInstance() {
    LircDriver* instance = LircDriver::getInstance();
    return instance->isLircCode() ? (LircCodeLircDriver *) instance : NULL;
}

void LircCodeLircDriver::deleteInstance() {
    LircDriver::deleteInstance();
}

LircCodeLircDriver::LircCodeLircDriver(const char* name, const char* path) : LircDriver(name, path) {
}

LircCodeLircDriver::LircCodeLircDriver(const char* name) : LircDriver(name) {
}


LircCodeLircDriver::~LircCodeLircDriver() {
}

bool LircCodeLircDriver::send(struct ir_ncode* code) {
    return curr_driver->send_func(remotes, code) != 0;
}

const char* LircCodeLircDriver::read() {
    return curr_driver->rec_func(remotes);
}

uint32_t LircCodeLircDriver::getLength() const {
    return curr_driver->code_length;
}

void LircCodeLircDriver::setRemotes(struct ir_remote* remotes) {
    this->remotes = remotes;
}