#ifndef _QUICK_PROFILER_HPP_
#define _QUICK_PROFILER_HPP_

#include <chrono>

class QuickProfiler {
public:
    std::chrono::high_resolution_clock::time_point t0;
    long long millis;
    double seconds;
    long long diff;

    QuickProfiler() {
        millis = diff = 0;
        seconds = 0.0;
    }

    inline void Start() {
        t0 = std::chrono::high_resolution_clock::now();
    }

    inline void Stop() {
        using namespace std::chrono;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        millis = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        diff = (t1 - t0).count();
        seconds = static_cast<double>(millis) / 1000.0;
    }
};

#endif