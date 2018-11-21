/* FICHERO: read_bmp_ble.c
 * DESCRIPCION: código para leer presión del sensortag */

#include "common.h"

float sensorBmp280Convert(uint32_t rawValue)
{
    return rawValue / 100.0f;
}

float read_bmp (uint8_t value, uint8_t* op)
{
    FILE* fp;
    char resp[1035];
    char respError[1035];
    char command[200];
    uint32_t rawPres = 0;
    uint32_t rawTemp = 0;
    float data;

    /* Crea el comando que va a ejecutar */
    memset(command, '\0', 200);
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_BMP_WRITE);
    strcat(command, " -n 01; sleep 1; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_BMP_READ);

    /* Recoge los datos que nos interesa */
    do {
        fp = popen(command, "r");
        /* Lee la salida de los que escribe el comando */
        while (fgets(resp, sizeof(resp)-1, fp) != NULL) {
        }
        pclose(fp);

        /* Comprueba si hay error en la lectura */
        strcpy(respError, resp);
        if (strlen(resp) == 0|| strcmp(strtok(respError, ":"), "Characteristic value/descriptor") != 0) {
            // Si hay error se manda un 0 y si se ha pasado la operación se dice que es error
            if (op != NULL) *op = OP_ERROR;
            return 0;
        }
        if (op != NULL) *op = OP_RESULTADO;  // Si se ha pasado la operación se dice que pasa el resultado

        /* Parsea la salida para quedarse con los bytes que nos interesan (Está en formato Little Endian) */
        char* bmpBytes = strchr(resp, ':') + 2;
        char strRawPres[7];
        char strRawTemp[7];

        memset(strRawPres, '\0', 7);
        strRawPres[0] = bmpBytes[15];
        strRawPres[1] = bmpBytes[16];
        strRawPres[2] = bmpBytes[12];
        strRawPres[3] = bmpBytes[13];
        strRawPres[4] = bmpBytes[9];
        strRawPres[5] = bmpBytes[10];

        memset(strRawTemp, '\0', 7);
        strRawTemp[0] = bmpBytes[6];
        strRawTemp[1] = bmpBytes[7];
        strRawTemp[2] = bmpBytes[3];
        strRawTemp[3] = bmpBytes[4];
        strRawTemp[4] = bmpBytes[0];
        strRawTemp[5] = bmpBytes[1];

        /* Pasa de string a unsigned int de 32 bits */
        rawPres = (uint32_t) strtol(strRawPres, NULL, 16);
        rawTemp = (uint32_t) strtol(strRawTemp, NULL, 16);

    } while(rawTemp == 0 && rawPres == 0);

    /* Convierte los datos y pasa el que se le pida */
    if (value == BMP_VALUE) {
        data = sensorBmp280Convert(rawPres);
    } else if (value == AMB_TMP_VALUE) {
        data = sensorBmp280Convert(rawTemp);
    }

    return data;  
}