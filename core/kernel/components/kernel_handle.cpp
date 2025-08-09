// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/handle_state.h"
#include "core/kernel/components/kernel_handle.h"

KernelHandle::KernelHandle(std::shared_ptr <HandleStateWrapper> handleStateWrapper, uint32_t id){
    _state = handleStateWrapper;
    _id = id;
}

/**
 * @brief Sets the state of the handle
 */
void KernelHandle::setState(HandleState state) {
    _state->state.store(state);
}


HandleState KernelHandle::getState() const{
    return _state->state.load();
}