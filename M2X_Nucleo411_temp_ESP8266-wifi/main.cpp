#include "mbed.h"
#include "M2XStreamClient.h"
#include "ESP8266Interface.h"
#include "TCPSocketConnection.h"


/*
*  ESP8266 Wifi Config for nucleo 411
*/
ESP8266Interface wifi(D8,D2,D3,"4free","qwerty12p",115200); // TX,RX,Reset,SSID,Password,Baud 
// The LED that we want to control
DigitalOut led(LED1);

// Temperature sensor is attached to pin A0, and is an 'analog' sensor 
AnalogIn temp(A0);
//fill these fields in from your ATT M2X account
char deviceId[] = "ed41fa464073d0c461504d32727f720e"; // Device you want to push to
char streamName[] = "temperature"; // Stream you want to push
char m2xKey[] = "31e031c53eb7609093152ccdd3af1907"; // Your M2X API Key or Master API Key
int main()
{
      // Set the baud rate that is used to communicate back to the computer to 115,200
    Serial pc(USBTX, USBRX);
    pc.baud(115200);
    pc.printf("Starting...\r\n");

    // connect to wifi
    wifi.init(); //Reset
    wifi.connect(); //Use DHCP
    pc.printf("IP Address is %s \n\r", wifi.getIPAddress());

    // Initialize the M2X client
    Client client;
    M2XStreamClient m2xClient(&client, m2xKey,1,"52.22.150.98"); // api-m2x.att.com
    
    int ret;
     unsigned int a, beta = 3975, units, tens;
    float temperature, resistance;

    while (true) {
        // send a temperature to M2X every 5 seconds
        a = temp.read_u16(); /* Read analog value */

    /* Calculate (calibrate) the resistance of the thermistor from analog votage read. */
    resistance = (float) 10000.0 * ((65536.0 / a) - 1.0);

    /* Convert the resistance to temperature using Steinhart's Hart equation */
    
    temperature =(1/((log(resistance/10000.0)/beta) + (1.0/298.15)))-273.15;
    //printf("Moisture value is %f\r\n", moist);

    pc.printf("temperature is %f\n", temperature);
        ret = m2xClient.updateStreamValue(deviceId, streamName, temperature);
        pc.printf("send() returned %d\r\n", ret);
        wait(5.0);
    }
}
