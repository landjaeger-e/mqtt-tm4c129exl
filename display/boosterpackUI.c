#include "boosterpackUI.h"
#include "header.h"

void staticcontentboosterpackUISubscribe(){
    CFAF128128B0145T_clear(CFAF128128B0145T_color_black);
    CFAF128128B0145T_text(25, 5, "MQTT Subscribe", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(0, 15, "---------------------", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(25, 25, "Eigene Werte:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 35, "IP:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 45, ipArray, CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 55, "Temp:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(110, 55, "°C", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(0, 65, "---------------------", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 75, "Empfangende Werte:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 85, "Device Name:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 105, "Temp:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(110, 105, "°C", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 115, "LichtSe.:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(110, 115, "lx", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}

void dynamiccontentboosterpackUICoreTemperature(){
    CFAF128128B0145T_text(70, 55, getTemperature(), CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}
void dynamiccontentboosterpackUIPublisherDeviceName(char* deviceName){
    CFAF128128B0145T_text(10, 95, deviceName, CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}
void dynamiccontentboosterpackUIPublisherCoreTemeperature(char* temp){
    CFAF128128B0145T_text(70, 105, temp, CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}
void dynamiccontentboosterpackUIPublisherAmbientLight(char* ambLight){
    CFAF128128B0145T_text(70, 115, ambLight, CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}


void staticcontentboosterpackUIPublish(){
    CFAF128128B0145T_clear(CFAF128128B0145T_color_black);
    CFAF128128B0145T_text(30, 5, "MQTT Publish", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(0, 15, "---------------------", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(25, 25, "Eigene Werte:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 35, "IP:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 45, ipArray, CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 55, "Temp:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(110, 55, "°C", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(0, 65, "---------------------", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(15, 75, "Gesendete Werte:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 95, "Temp:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(110, 95, "°C", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(10, 105, "LichtSe.:", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(110, 105, "lx", CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);

}

void dynamiccontentboosterpackUIPublishTemperature(){
    CFAF128128B0145T_text(70, 55, getTemperature(), CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(70, 95, getTemperature(), CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}
void dynamiccontentboosterpackUIPublishAmbientLight(){
    CFAF128128B0145T_text(70, 105, OPT3001Value(), CFAF128128B0145T_color_white,CFAF128128B0145T_color_black, 1, 1);
}

