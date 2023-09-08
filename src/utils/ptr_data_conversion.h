#pragma once

#include <stdexcept>
#include <memory>

template <typename T>
const T &GetPtrData(const void* ptr)
{
    if (ptr == nullptr) {
        throw std::runtime_error("Null Pointer Exception.");
    }

    return *(static_cast<const T*>(ptr));
}

template <typename T>
const void* GetDataPtr(const T &data)
{
    return static_cast<const void*>(std::addressof(data));
}
