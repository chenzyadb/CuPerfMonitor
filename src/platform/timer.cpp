#include "timer.h"

Timer::Timer() : timerMap_() { }

void Timer::AddTimer(const std::string &name, const TimerTask &task, const int &intervalMs)
{
    std::unique_lock<std::mutex> lck(mtx_);
    if (timerMap_.count(name) == 1) {
        return;
    }
    {
        TimerData timerData{};
        timerData.task = task;
        timerData.intervalMs = intervalMs;
        timerMap_[name] = timerData;
    }
    {
        std::thread timerThread(std::bind(&Timer::TimerThread_, this, name));
        timerThread.detach();
    }
}

void Timer::DeleteTimer(const std::string &name)
{
    std::unique_lock<std::mutex> lck(mtx_);
    if (timerMap_.count(name) == 1) {
        timerMap_.erase(name);
    }
}

bool Timer::IsTimerExist(const std::string &name)
{
    std::unique_lock<std::mutex> lck(mtx_);
    bool exist = false;
    if (timerMap_.count(name) == 1) {
        exist = true;
    }

    return exist;
}

void Timer::TimerThread_(const std::string &name) 
{
    SetThreadName(StrMerge("Timer-%llu", timerMap_.size()));

    std::string timerName = name;
    for (;;) {
        TimerData data{};
        {
            std::unique_lock<std::mutex> lck(mtx_);
            const auto &iter = timerMap_.find(timerName);
            if (iter == timerMap_.end()) {
                break;
            }
            data = iter->second;
        }
        {
            const auto &task = data.task;
            task();
        }
        usleep(data.intervalMs * 1000);
    }

    return;
}
