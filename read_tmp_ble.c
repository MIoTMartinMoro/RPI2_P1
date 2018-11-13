/* FICHERO: read_tmp_ble.c
 * DESCRIPCION: codigo para leer temperatura del sensortag */

#include "common.h"

void sensorTmp007Convert(uint16_t rawAmbTemp, uint16_t rawObjTemp, float* tAmb, float* tObj)
{
    const float SCALE_LSB = 0.03125;
    float t;
    int it;
    it = (int)((rawObjTemp) >> 2);
    t = ((float)(it)) * SCALE_LSB;
    *tObj = t;
    it = (int)((rawAmbTemp) >> 2);
    t = (float)it;
    *tAmb = t * SCALE_LSB;
}

float read_tmp (uint8_t value, uint8_t* op)
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char respError[1035];
    char command[200];
    uint16_t rawTAmb = 0;
    uint16_t rawTObj = 0;
    float tAmb;
    float tObj;

    /* Crea el comando que va a ejecutar */
    memset(command, '\0', 200);
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_TMP_WRITE);
    strcat(command, " -n 01; sleep 1; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_TMP_READ);

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
        char* tempBytes = strchr(resp, ':') + 2;
        char strRawTAmb[5];
        char strRawTObj[5];

        memset(strRawTAmb, '\0', 5);
        strRawTAmb[0] = tempBytes[9];
        strRawTAmb[1] = tempBytes[10];
        strRawTAmb[2] = tempBytes[6];
        strRawTAmb[3] = tempBytes[7];

        memset(strRawTObj, '\0', 5);
        strRawTObj[0] = tempBytes[3];
        strRawTObj[1] = tempBytes[4];
        strRawTObj[2] = tempBytes[0];
        strRawTObj[3] = tempBytes[1];

        /* Pasa de string a unsigned int de 32 bits */
        rawTAmb = (uint16_t) strtol(strRawTAmb, NULL, 16);
        rawTObj = (uint16_t) strtol(strRawTObj, NULL, 16);
        
    } while(rawTAmb == 0 && rawTObj == 0);

    /* Convierte los valores */
    sensorTmp007Convert(rawTAmb, rawTObj, &tAmb, &tObj);

    /* Devuelve el valor que se le pide */
    if (value == AMB_TMP_VALUE) {
        return tAmb;
    } else if (value == OBJ_TMP_VALUE) {
        return tObj;
    }

    return 0;
}