/*
* UART0
* IO0 - 2
* IO2 - 3
* EN - 4
*/
#include "pico/stdio.h"
#include <string>
#include "esp01s.h"
#include <cstring>
#include "pico/stdlib.h"

int main(void) {
    stdio_init_all();
    sleep_ms(10000);
    ESP01S::SoftAP ap(uart0, 0, 1);
    ap.setup();
    WiFi espeditoWiFi=ap.query_wifi(100);
    espeditoWiFi.pwd="geter013";
    ap.set_wifi(espeditoWiFi);
    ap.set_softap();
    ESP01S::Station st(uart1,4,5);
    st.setup();
    st.set_station();
    ap.configure_softap("esp01s","esp01spwd",0,1);
    WiFi softapWiFi;
    softapWiFi.bssid="esp01s";
    softapWiFi.pwd="esp01spwd";
    softapWiFi.security=1;
    st.set_wifi(softapWiFi);
    ap.start_tcp(80);
    st.establish_tcp(80);
    st.send("Hello SoftAP!");
    sleep_ms(1000);
    printf(ap.receive(12).c_str());
}
