#include "PicoGPIOLib.hpp"
#include "hardware/i2c.h"
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include <cstdint>


enum buttonStatus
{
    IDLE,
    SMALL,
    LARGE

};

class server
{

private:



public:


void server_initialization(const char *WIFI_SSID,const char *WIFI_PASSWORD);

//CGI=============================================
void cgi_init(void);

static const tCGI cgi_handlers[];

static const char * cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//================================================


//SSI============================================
void ssi_init();
static u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen);
static const char * ssi_tags[];
//===============================================

//outputs============================
static std::vector<int> outputs; // Used to drive transitor

//==========================================

//Push button function===============================

static void push_button(int gpio); // toggles pin high for .5 seconds
static buttonStatus garage_doors;
//====================================================

server();
};