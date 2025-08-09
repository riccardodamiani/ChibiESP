// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef HANDLE_STATE_H
#define HANDLE_STATE_H

#include <cstdint>
#include <atomic>

enum class HandleState : uint8_t{
    Invalid,
    Idle,       // The handle is idle and the resource cannot be accessed
    Active,     // The handle is currently in use
    Revoked,    // The handle has been revoked
    Suspended,  // The handle is temporarily suspended. Can be resumed later
    Released    // The handle has been released. The resource cannot be accessed anymore
};

class HandleStateWrapper{
public:
    HandleStateWrapper(HandleState init_state) : state(init_state) {}

    // Not copiable nor movable
    HandleStateWrapper(const HandleStateWrapper&) = delete;
    HandleStateWrapper& operator=(const HandleStateWrapper&) = delete;
    HandleStateWrapper(HandleStateWrapper&&) = delete;
    HandleStateWrapper& operator=(HandleStateWrapper&&) = delete;

    std::atomic<HandleState> state;
};


#endif // HANDLE_STATE_H