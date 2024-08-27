#include <iostream>
#include "../include/Wifi.hpp"
#include "hardware/i2c.h"
#include "lwipopts.h"
#include "hardware/gpio.h"
#include "pico/stdio.h"


const char WIFI_SSID[] = "XXX"; //WIFI NAME
const char WIFI_PASSWORD[] = "XXX"; //WIFI Password

std::vector<int> server::outputs = 
{
    14,//Small garage
    15,//Big garage
    16,//Conntection Light

};



buttonStatus server::garage_doors = IDLE;

void activate_button();

int main()
{
    
    stdio_init_all();
    printf("Hello");
    sleep_ms(250);
    server obj;
    gpio_put(server::outputs[2], 1);
    obj.server_initialization(WIFI_SSID, WIFI_PASSWORD);
    gpio_put(server::outputs[2], 0);
    while(true)
    {
        
        activate_button();
    
    }
    return 0;

}


void activate_button()
{

switch(server::garage_doors)
{

case IDLE:

break;
case SMALL:
server::push_button(server::outputs[0]);
server::garage_doors = IDLE;
break;

case LARGE:
server::push_button(server::outputs[1]);
server::garage_doors = IDLE;

break;
}
}