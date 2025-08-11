#pragma once
#include <string>
#include <algorithm>

class Countdown {
public:
    Countdown(float seconds = 3.f)
        : total_(seconds), remaining_(seconds), started_(false), finished_(false) {
    }
    ~Countdown() = default;

    void
        start() {
        started_ = true; finished_ = false; remaining_ = total_;
    }
    void stop() {
        started_ = false;
    }
    bool isStarted() const {
        return started_;
    }
    bool isFinished() const {
        return finished_;
    }

    std::string tick(float dt) {
        if (!started_ || finished_) return "";
        remaining_ = std::max(0.f, remaining_ - dt);
        if (remaining_ == 0.f) { finished_ = true; started_ = false; }
        int num = static_cast<int>(remaining_ + 0.999f);
        if (finished_) return "GO!";
        return (num >= 1) ? std::to_string(num) : "GO!";
    }

private:
    float total_;
    float remaining_;
    bool  started_;
    bool  finished_;
};