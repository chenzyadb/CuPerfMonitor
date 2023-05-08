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

        static T* GetInstance(void) {
            std::call_once(FLAG, CreateInstance);
            return INSTANCE.get();
        }

        ~Singleton() {
            INSTANCE.reset();
        }

    private:
        static void CreateInstance() {
            INSTANCE.reset(new T());
        }

        static std::unique_ptr<T> INSTANCE;
        static std::once_flag FLAG;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::INSTANCE = nullptr;

template<typename T>
std::once_flag Singleton<T>::FLAG;
