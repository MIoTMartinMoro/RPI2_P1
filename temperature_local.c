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


int main(int argc, char* argv[])
{

	if (argc != 3)
    {
        fprintf (stderr, "uso: thermo ip topic\n");
        exit (1);
	}

    float temperature = 0.f;
	char ip[16];
	char topic[50];
    char command[70];
    memset(command, '\0', 70);
    memset(topic, '\0', 50);
    memset(ip, '\0', 16);

    strcpy(ip, argv[1]);
    strcpy(topic, argv[2]);

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&temperature);
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t '%s' -m %f", ip, topic, temperature);
    printf("%s: %d\n", command, strlen(command));
    thermo3_click_disable();

    i2c_release();

    return 0;
}
