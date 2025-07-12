// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef VIEW_ERROR_H
#define  VIEW_ERROR_H

enum class ViewError{
    NO_ERROR = 0,
    ITEM_NOT_AVAILABLE = -1,
    FUNCTION_NOT_AVAILABLE = -2,
    INVALID_PARAM = -3,
    EMPTY_FIELD = -4,
    ITEM_ALREADY_EXISTS = -5
};

#endif