#include "../include/Wifi.hpp"
#include "hardware/gpio.h"
#include "hardware/i2c.h"



server::server()
{
 gpio_pin_init(outputs, false);
    

}


void server::server_initialization(const char *WIFI_SSID,const char *WIFI_PASSWORD)
{
   
     cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
      
    }
    // Print a success message once connected
    printf("Connected! \n");

      //Prints IP address of device  
      extern cyw43_t cyw43_state;
        auto ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
        printf("IP Address: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, (ip_addr >> 24) & 0x000000FF);
    
    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
    
}
//CGI Funtions ========================================================================================================
void server::cgi_init(void)
{

  http_set_cgi_handlers(cgi_handlers, 1);
}

const tCGI server::cgi_handlers[]
{
        
        "/Garage", server::cgi_handler,
      
};

const char * server::cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
     //EXMAPLE CODE FROM HTML
     if(strcmp(pcParam[0], "gar") == 0)
    {
      if(strcmp(pcValue[0], "0") == 0)
      {
        garage_doors = SMALL;
        
      } else if(strcmp(pcValue[0], "1") ==0)
      {
        
        garage_doors = LARGE;
      }
       
    } 

    //Assures that website sends data back
     return "/index.shtml";
}
//==========================================================================================================

//SSI FUNCTIONS=============================================================================================
const char * server::ssi_tags[] = 
{

};

void server::ssi_init()
{
   //  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}

u16_t server::ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
  size_t printed;

  //EXAMPLE FROM HTML
  switch(iIndex)
  {
  
    default:
    printed = 0;
    break;
  }
  return (u16_t)printed;
}

//==================================================================================================






void server::push_button(int gpio)
{
  gpio_put(gpio,1);
  sleep_ms(500);
  gpio_put(gpio, 0);
  sleep_ms(500);

}