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

#define MQTT_BROKER "192.168.1.91"
#define MQTT_PORT 1883
#define MQTT_TOPIC "sensors"

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

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&data);

    // Temperatura ambiente Ci40
    sprintf(msg, "Temperatura ambiente Ci40: %f", ip, MQTT_TOPIC, data);
    mosquitto_publish(mosq, NULL, MQTT_TOPIC, sizeof(msg), msg, 0, 1);    
    /*system(command);
    sleep(2);
    // Temperatura ambiente SensorTag temperatura
    data = read_tmp(AMB_TMP_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Temperatura ambiente SensorTag(tmp): %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Temperatura objeto SensorTag temperatura
    data = read_tmp(OBJ_TMP_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Temperatura objeto SensorTag(tmp): %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Temperatura ambiente SensorTag presion
    data = read_bmp(AMB_TMP_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Temperatura ambiente SensorTag(bmp): %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Presion SensorTag presion
    data = read_bmp(BMP_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Presion SensorTag(bmp): %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Temperatura ambiente SensorTag humedad
    data = read_hum(AMB_TMP_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Temperatura ambiente SensorTag(hum): %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Humedad SensorTag humedad
    data = read_hum(HUM_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Humedad SensorTag(hum): %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Sensacion termica SensorTag humedad
    data = read_hum(SEN_TMP_VALUE, NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Sensacion termica SensorTag(hum) %f'", ip, topic, data);
    system(command);
    sleep(2);
    // Luminosidad SensorTag luminosidad
    data = read_opt(NULL);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m 'Luminosidad SensorTag(opt) %f'", ip, topic, data);
    system(command);*/
    

    thermo3_click_disable();

    i2c_release();

    return 0;
}
