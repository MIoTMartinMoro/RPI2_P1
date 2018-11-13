#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>

// Server connection parameters
#define MQTT_HOSTNAME "localhost/192.168.1.1"
#define MQTT_BROKER "192.168.1.91"
#define MQTT_PORT 1883
#define MQTT_USERNAME "miot"
#define MQTT_PASSWORD "masteriot"
#define MQTT_TOPIC "sensors"

struct mosquitto* mosq;

void my_message_callback(struct mosquitto* mosq, void* Obj, const struct mosquitto_message* message)
{
    // TO-DO
}

int main(int argc, char** argv)
{
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    
    if(!mosq) {
        fprintf(stderr, "Can't init Mosquitto library\n");
        exit(1);
    }

    mosquitto_username_pw_set(mosq, MQTT_USERNAME, MQTT_PASSWORD);


    int ret = mosquitto_connect(mosq, MQTT_BROKER, MQTT_PORT, 0);

    if(ret) {
        fprintf(stderr, "Can't connect to Mosquitto server\n");
        exit(1);
    }

    ret = mosquitto_subscribe(mosq, NULL, MQTT_TOPIC, 0);

    if(ret) {

        fprintf(stderr, "Can't publish to Mosquitto server\n");
        exit(1);
    }

    mosquitto_message_callback_set(mosq,my_message_callback);

    mosquitto_loop_forever(mosq,-1,1);

    mosquitto_lib_cleanup();

    return 0;
}
 