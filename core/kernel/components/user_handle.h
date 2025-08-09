// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef USER_HANDLE_H
#define USER_HANDLE_H

#include <cstdint>
#include <atomic>
#include <memory>

#include "core/kernel/components/handle_state.h"

class Handle {
public:
    Handle(std::shared_ptr <HandleStateWrapper> handleStateWrapper, uint32_t id);

    // Not copiable nor movable
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    // Movable
    Handle(Handle&& other) noexcept
        : _state(std::move(other._state)) {}

    Handle& operator=(Handle&& other) noexcept {
        if (this != &other) {
            _state = std::move(other._state);
        }
        return *this;
    }

    HandleState getState() const;
    uint32_t getId() const { return _id; } // Getter for the handle ID
private:
    std::shared_ptr<HandleStateWrapper> _state;
    uint32_t _id; // Unique identifier for the handle, if needed
};

#endif // USER_HANDLE_H