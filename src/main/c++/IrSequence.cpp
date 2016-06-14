#include "IrSequence.h"
#include <string.h>
#include <string>
#include <sstream>

IrSequence::IrSequence() : durations(NULL), length(0U), toBeFreed(false) {
};

IrSequence::IrSequence(const microseconds_t *durations_, size_t length_, bool toBeFreed_)
: durations(durations_), length(length_), toBeFreed(toBeFreed_) {
}

IrSequence::IrSequence(const IrSequence& orig) : durations(orig.durations), length(orig.length), toBeFreed(orig.toBeFreed) {
};

IrSequence::IrSequence(const IrSequence& orig, bool toBeFreed_) : durations(orig.durations), length(orig.length), toBeFreed(toBeFreed_) {
};

IrSequence::~IrSequence() {
    if (toBeFreed)
        delete [] durations;
}

IrSequence *IrSequence::clone() const {
    microseconds_t *durationsClone = new microseconds_t[length];
    memcpy(durationsClone, durations, length*sizeof(microseconds_t));
    return new IrSequence(durationsClone, length, true);
}

void IrSequence::dump(std::ostream& stream, bool usingSigns) const {
    for (unsigned int i = 0U; i < length; i++) {
        if (i > 0U)
            stream << ' ';
        if (usingSigns)
            stream << ((i & 1) ? '-' : '+');
        stream << durations[i];
    }
    stream << std::endl;
}

const char* IrSequence::toString() const {
    std::stringstream os;
    dump(os, true);
    std::string s = os.str();
    char* buf = new char[s.length()+1];
    strncpy(buf, s.c_str(), s.length());
    return buf;
}

microseconds_t IrSequence::getTotalDuration() const {
    microseconds_t sum = 0;
    for (unsigned int i = 0; i < length; i++)
        sum += durations[i];

    return sum;
}