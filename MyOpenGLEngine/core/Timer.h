#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>


// FROM CHAT GPT

class Timer {
public:
    // Start the timer with the given name
    static void startTimer(const std::string& name) {
        _startTimes[name] = std::chrono::high_resolution_clock::now();
        std::cout << "Timer '" << name << "' started." << std::endl;
    }

    // Stop the timer and print the elapsed time
    static void stopTimer(const std::string& name) {
        if (_startTimes.find(name) == _startTimes.end()) {
            std::cout << "Timer '" << name << "' was not started." << std::endl;
            return;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto startTime = _startTimes[name];
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        std::cout << "Timer '" << name << "' stopped. Elapsed time: " << elapsed << " ms." << std::endl;

        _startTimes.erase(name); // Remove the timer after stopping it
    }

private:
    static std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> _startTimes;
};