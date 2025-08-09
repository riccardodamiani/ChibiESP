// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef HANDLE_H
#define HANDLE_H

#include <cstdint>
#include <atomic>
#include <memory>

#include "core/kernel/components/handle_state.h"

class KernelHandle {
public:
    KernelHandle(std::shared_ptr <HandleStateWrapper> handleStateWrapper, uint32_t id);

    // Not copiable nor movable
    KernelHandle(const KernelHandle&) = delete;
    KernelHandle& operator=(const KernelHandle&) = delete;
    KernelHandle(KernelHandle&&) = delete;
    KernelHandle& operator=(KernelHandle&&) = delete;

    HandleState getState() const;
    void setState(HandleState state);
    uint32_t getId() const { return _id; } // Getter for the handle ID
private:
    std::shared_ptr<HandleStateWrapper> _state;
    uint32_t _id; // Unique identifier for the handle, if needed
};

#endif // KERNEL_HANDLE_H