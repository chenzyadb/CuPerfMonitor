#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <thread>
#include <mutex>
#include "singleton.h"
#include "utils/cu_misc.h"

class Timer : public Singleton<Timer> 
{
    public:
        using TimerTask = std::function<void(void)>;

        Timer();
        void AddTimer(const std::string &name, const TimerTask &task, const int &intervalMs);
        void DeleteTimer(const std::string &name);
        bool IsTimerExist(const std::string &name);

    private:
        typedef struct {
            TimerTask task;
            int intervalMs;
        } TimerData;
        
        std::unordered_map<std::string, TimerData> timerMap_;
        std::mutex mtx_;

        void TimerThread_(const std::string &name);
};
