#include "core/graphics/task_view_renderer.h"
#include "core/graphics/view_render.h"
#include "core/logging/logging.h"

#include "core/driver/display_driver.h"
#include "chibiESP.h"

TaskViewRenderer::TaskViewRenderer(ChibiESP* kernelInstance){
    _displayDriver = kernelInstance->getDisplayDriver("ssd1306");
    if(!_displayDriver) Logger::error("No fucking display driver");
}

bool TaskViewRenderer::renderView(ViewRenderStruct &renderView){
    if(_displayDriver == nullptr) return false;
    if(renderView.elements.size() == 0) return true;

    //get text offsets and size
    for(int i = 0; i < renderView.elements.size(); i++){
        ItemRenderStruct &item = renderView.elements[i];
        _displayDriver->getTextSize(item.text, item.view_x, item.view_y, 1, &item.real_x, &item.real_y, &item.width, &item.height);
    }
    
    //for now hardcoded. TODO: should come from the driver
    int displayWidth = 128;
    int displayHeight = 64;
    int half_screen_size =  displayHeight / 2;
    int upperTextScreenPixel;
    int firstElementOnScreen = 0;

    //find out if the selected element can stay in the center
    for(int i = renderView.focusedItemIndex-1; i >= 0; i--){
        int upperTextScreenPixel = half_screen_size - renderView.elements[renderView.focusedItemIndex].height / 2 - 
            (renderView.elements[renderView.focusedItemIndex].real_y - renderView.elements[i].real_y);
        int lowerTextScreenPixel = upperTextScreenPixel + renderView.elements[renderView.focusedItemIndex].height;
        if(upperTextScreenPixel < 0 && lowerTextScreenPixel < 0){ //done
            firstElementOnScreen = i+1;
            break;
        }
    }

    //draws the elements until done
    int offsetY = renderView.elements[firstElementOnScreen].view_y;

    _displayDriver->clearScreen();
    for(int i = firstElementOnScreen; i < renderView.elements.size(); i++){
        BW_Color bg_color = renderView.focusedItemIndex == i ? BW_Color::CESP_WHITE : BW_Color::CESP_BLACK;
        BW_Color fg_color = renderView.focusedItemIndex == i ? BW_Color::CESP_BLACK : BW_Color::CESP_WHITE;

        int displayItemRectY = renderView.elements[i].real_y - offsetY;
        if(displayItemRectY > displayHeight){   //done drawing stuff
            break;
        }
        //Logger::info("%d, %d, %d, %d, %s", renderView.elements[i].view_x, renderView.elements[i].view_y - offsetY, 
         //   renderView.elements[i].width,  renderView.elements[i].height, renderView.elements[i].text.c_str() );
        _displayDriver->drawText(renderView.elements[i].text, 
            renderView.elements[i].view_x, renderView.elements[i].view_y - offsetY, 
            1, bg_color, fg_color);
    }
    _displayDriver->updateScreen();
    return true;
}