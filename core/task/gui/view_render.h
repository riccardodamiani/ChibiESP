// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef VIEW_RENDER_H
#define VIEW_RENDER_H

#include <string>
#include <vector>

struct ItemRenderStruct{
    std::string text;   //text to display
    int16_t view_x, view_y; //position of the item in the view
    int16_t real_x, real_y;
    uint16_t width, height;
};

struct ViewRenderStruct{
    std::vector <ItemRenderStruct> elements;    //list of items to be displayed
    int focusedItemIndex;    //index of the focused item
};

#endif  //VIEW_RENDER_H