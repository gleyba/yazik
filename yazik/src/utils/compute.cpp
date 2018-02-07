#include <yazik/utils/compute.hpp>

namespace yazik {
namespace compute {
    uint64_t upperPowerOfTwo(uint64_t v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        v++;
        return v;
    }
} //end of compute namespace
} //end of yazik namespace
