/**
 * This example shows how to use the Thermo3 Click wrapper of the LetMeCreate
 * library.
 *
 * It reads the temperature from the sensor and exits.
 *
 * The Thermo3 Click must be inserted in Mikrobus 1 before running this program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <letmecreate/letmecreate.h>
#include <mosquitto.h>
#include "common.h"

#define MQTT_PORT 1883

struct mosquitto* mosq;

int main(int argc, char* argv[])
{

    if (argc != 3)
    {
        fprintf (stderr, "uso: thermo ip topic\n");
        exit (1);
    }

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    
    if(!mosq) {
        fprintf(stderr, "Can't init Mosquitto library\n");
        exit(1);
    }

    float data = 0.f;
    char ip[16];
    char topic[50];
    char msg[170];

    memset(msg, '\0', 170);
    memset(topic, '\0', 50);
    memset(ip, '\0', 16);

    strcpy(ip, argv[1]);
    strcpy(topic, argv[2]);

    int ret = mosquitto_connect(mosq, ip, MQTT_PORT, 0);

    if(ret) {
        fprintf(stderr, "Can't connect to Mosquitto server\n");
        exit(1);
    }

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&data);

    // Temperatura ambiente Ci40
    sprintf(msg, "Temperatura ambiente Ci40: %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Temperatura ambiente SensorTag temperatura
    data = read_tmp(AMB_TMP_VALUE, NULL);
    sprintf(msg, "Temperatura ambiente SensorTag(tmp): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Temperatura objeto SensorTag temperatura
    data = read_tmp(OBJ_TMP_VALUE, NULL);
    sprintf(msg, "Temperatura objeto SensorTag(tmp): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Temperatura ambiente SensorTag presion
    data = read_bmp(AMB_TMP_VALUE, NULL);
    sprintf(msg, "Temperatura ambiente SensorTag(bmp): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Presion SensorTag presion
    data = read_bmp(BMP_VALUE, NULL);
    sprintf(msg, "Presion SensorTag(bmp): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Temperatura ambiente SensorTag humedad
    data = read_hum(AMB_TMP_VALUE, NULL);
    sprintf(msg, "Temperatura ambiente SensorTag(hum): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Humedad SensorTag humedad
    data = read_hum(HUM_VALUE, NULL);
    sprintf(msg, "Humedad SensorTag(hum): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Sensacion termica SensorTag humedad
    data = read_hum(SEN_TMP_VALUE, NULL);
    sprintf(msg, "Sensacion termica SensorTag(hum): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
    sleep(2);
    // Luminosidad SensorTag luminosidad
    data = read_opt(NULL);
    sprintf(msg, "Luminosidad SensorTag(opt): %f", data);
    mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);    

    thermo3_click_disable();

    i2c_release();

    return 0;
}
