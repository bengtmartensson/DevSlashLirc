#pragma once

#include "LircDriver.h"

class LircCodeLircDriver : public LircDriver {
public:
    static LircCodeLircDriver* newInstance(const char *name = "default");
    static LircCodeLircDriver* getInstance();
    static void deleteInstance();

private:
    struct ir_remote* remotes;

    LircCodeLircDriver(const char* name);
    LircCodeLircDriver(const char* name, const char* path);
    virtual ~LircCodeLircDriver();

public:
    void setRemotes(struct ir_remote* remotes);
    bool isMode2() const { return false; }
    bool send(struct ir_ncode* code);
    const char* read();
    uint32_t getLength() const;
};
