/* This function has everything related to networking
 *
*/
//comment while compiling
//#include "user_config.h"

#define AP_SSID "Shinu BSNL"
#define AP_PASSWORD "987654321"

//function definitons
void wifi_event_handler(System_Event_t * ev);
void connection_setup();
void start_listening_on_socket();
void socket_connect_callback_function(void * arg);
void socket_disconnect_callback_function(void * arg);

//data structures used
struct espconn esp;
uint8 isConnected; //0 for not connected, 1 for connected
esp_tcp tcp;

/* This function sets up connecting to WiFi network
 * Also sets WiFi event handler function
*/
void ICACHE_FLASH_ATTR
connection_setup()
{
    struct station_config * statcon = (struct station_config *)os_zalloc(sizeof(struct station_config));
#ifdef DEBUG
    os_printf("Memory allocated to ESP\r\n");
#endif
    os_strcpy(statcon->ssid, AP_SSID);
    os_strcpy(statcon->password, AP_PASSWORD);
#ifdef DEBUG
    os_printf("Written details\r\n");
#endif
    wifi_set_opmode(STATION_MODE);
#ifdef DEBUG
    os_printf("Station mode set\r\n");
#endif
    wifi_station_set_config_current(statcon);
#ifdef DEBUG
    os_printf("Configuration set\r\n");
#endif
    os_free(statcon);
    wifi_set_event_handler_cb(wifi_event_handler);
}

void ICACHE_FLASH_ATTR wifi_event_handler(System_Event_t * ev)
{
    if(ev->event == EVENT_STAMODE_GOT_IP)
        start_listening_on_socket();
}

void ICACHE_FLASH_ATTR
start_listening_on_socket()
{
    esp.state = ESPCONN_NONE;
    esp.type = ESPCONN_TCP;
    esp.proto.tcp = &tcp;
    esp.proto.tcp->local_port = 8123;
    espconn_regist_connectcb(&esp, socket_connect_callback_function);
    espconn_regist_disconcb(&esp, socket_disconnect_callback_function);
    espconn_accept(&esp);
}

void ICACHE_FLASH_ATTR socket_connect_callback_function(void *arg)
{
    isConnected = 1;
}

void ICACHE_FLASH_ATTR socket_disconnect_callback_function(void *arg)
{
    isConnected = 0;
}
