#pragma once

#include <iostream>
#include <memory>
#include <mutex>

template<typename T>
class Singleton {
    public:
        Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton &operator = (const Singleton&) = delete;

        static T* GetInstance(void) 
        {
            std::call_once(flag_, CreateInstance_);
            return instance_;
        }

    private:
        static void CreateInstance_() 
        {
            instance_ = new T();
        }

        static T* instance_;
        static std::once_flag flag_;
};

template<typename T>
T* Singleton<T>::instance_ = nullptr;

template<typename T>
std::once_flag Singleton<T>::flag_;
