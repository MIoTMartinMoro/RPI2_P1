/* FICHERO: read_opt_ble.c
 * DESCRIPCION: codigo para leer la luminosidad del sensortag */

#include "common.h"

float sensorOpt3001Convert (uint16_t rawData)
{
    uint16_t e, m;
 
    m = rawData & 0x0FFF;
    e = (rawData & 0xF000) >> 12;
 
    /** e on 4 bits stored in a 16 bit unsigned => it can store 2 << (e - 1) with e < 16 */
    e = (e == 0) ? 1 : 2 << (e - 1);
 
    return m * (0.01 * e);
}

float read_opt (uint8_t* op)
{
    FILE* fp;
    char resp[1035];
    char respError[1035];
    char command[200];
    uint16_t rawOpt = 0;

    /* Crea el comando que va a ejecutar */
    memset(command, '\0', 200);
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_OPT_WRITE);
    strcat(command, " -n 01; sleep 1; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_OPT_READ);

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

        /* Parsea la salida para quedarse con los bytes que nos interesan (Está en formato Little Endian)*/
        char* optBytes = strchr(resp, ':') + 2;
        char strRawOpt[5];

        memset(strRawOpt, '\0', 5);
        strRawOpt[0] = optBytes[3];
        strRawOpt[1] = optBytes[4];
        strRawOpt[2] = optBytes[0];
        strRawOpt[3] = optBytes[1];

        /* Pasa de string a unsigned int de 16 bits*/
        rawOpt = (uint16_t) strtol(strRawOpt, NULL, 16);

    } while(rawOpt == 0);

    /* Convierte el valor y lo pasa */
    return sensorOpt3001Convert(rawOpt);
}