#pragma once

#include <thread>
#include <chrono>


namespace yazik {
namespace atomics {

class baton {

    using nanosec_t = std::chrono::nanoseconds;
    using h_r_clock = std::chrono::high_resolution_clock;
    mutable size_t _attempts = 0;
    size_t _maxAttempts = 15;
    nanosec_t _sleepNanosec = nanosec_t{2000};

public:

    inline baton(){}

    inline baton(
        size_t maxAttempts,
        nanosec_t sleepNanosec
    )
    : _maxAttempts{maxAttempts}
    , _sleepNanosec{sleepNanosec} {}

    inline void incrementAttemptsAndCheck() const {
        if (_maxAttempts && _attempts++ > _maxAttempts) {
            auto yieldStart = h_r_clock::now();
            std::this_thread::yield();
            nanosec_t yieldTime = std::chrono::duration_cast<nanosec_t>(h_r_clock::now() - yieldStart);
            nanosec_t estimateSleep = _sleepNanosec - yieldTime;
            if (estimateSleep.count() > 0) {
                std::this_thread::sleep_for(estimateSleep);
            }
            _attempts = 0;
        }
    }

    inline void flush() const {
        _attempts = 0;
    }


};

} //end of atomics namespace
} //end of yazik namespace
