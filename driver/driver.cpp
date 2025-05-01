/**
 * @file driver.cpp
 * @brief Driver class implementation for handling hardware devices.
 * @details This file contains the implementation of the base Driver class, which provides methods to handle a generic hardware device
 */

#include "driver/driver.h"

//Configure the driver with all the device that needs to handle
int CESP_Driver::configure(void* arg){

}

//Initialize the communication with the hardware device
//This is called every time the driver module is loaded in the kernel
int CESP_Driver::init(void* arg){

}

//Deinitialize the communication with the hardware device
//This is called every time the driver module is unloaded from the kernel
int CESP_Driver::deinit(void* arg){

}

//Open a file (or device). This function is not alwayes required, depends on the application
int CESP_Driver::open(void* arg){

}

//Close a file (or device). This function is not alwayes required, depends on the application
int CESP_Driver::close(void* arg){

}

//Seek to a specific position in a file (or device). This function is not alwayes required, depends on the application
int CESP_Driver::seek(void* arg){

}

//Update the devices state. This function is called in the main driver update loop
int CESP_Driver::update(void* arg){

}

//Read data from the device
int CESP_Driver::read(void* arg){

}

//Write data to the device
int CESP_Driver::write(void* arg){

}

//Get the device information
int CESP_Driver::get_device_info(void* arg){

}
