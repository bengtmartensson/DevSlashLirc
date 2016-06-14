#include "LircT.h"

LircT::LircT(lirc_t data) {
    this->data = data;
}

LircT::LircT(microseconds_t data, LircTType type) {
    // TODO: test input validity
    this->data = data | type;
}

LircT::LircT(const LircT& orig) {
    data = orig.data;
}

LircT::~LircT() {
}

void LircT::dump(std::ostream& stream) const {
    stream << (isSpace() ? "space"
            : isPulse() ? "pulse"
            : isFrequency() ? "frequency"
            : isTimeout() ? "timeout"
            : "unknown");
    stream << " " << getDuration();
}