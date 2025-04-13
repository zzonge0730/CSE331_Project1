#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:
    Timer() { reset(); }

    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMilliseconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = now - start;
        return elapsed.count();
    }

private:
    std::chrono::high_resolution_clock::time_point start;
};

#endif // TIMER_HPP
