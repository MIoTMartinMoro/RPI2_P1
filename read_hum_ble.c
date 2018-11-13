/* FICHERO: read_hum_ble.c
 * DESCRIPCION: codigo para leer la humedad del sensortag */

#include "common.h"

/*Función de conversión de datos */
void sensorHdc1000Convert(uint16_t rawTempFun, uint16_t rawHumFun,
                        float *tempFun, float *humFun)
{
  //-- calculate temperature [°C]
  *tempFun = ((double)(int16_t)rawTempFun / 65536)*165 - 40;
 
  //-- calculate relative humidity [%RH]
  rawHumFun &= ~0x0003; // remove status bits
  *humFun = ((double)rawHumFun / 65536)*100;
}

/*Función de cálculo de sensación térmica */
void calculate_thermal_sesation(float tempFun, float humFun, float *senTermFun)
{
    *senTermFun=-8.785+1.6139*tempFun+2.3385*humFun-0.1461*tempFun*humFun-
        0.0123*tempFun*tempFun-0.0164*humFun*humFun+0.0022*tempFun*tempFun*humFun+
        0.00073*humFun*humFun-0.0000036*tempFun*tempFun*humFun*humFun;
}

/*Función de leer la humedad del sensor */
float read_hum (uint8_t value, uint8_t* op)
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char respError[1035];
    char command[200];
    uint16_t rawHum = 0;
    uint16_t rawTemp = 0;
    uint16_t rawSenTer = 0;
    float temp;
    float hum;
    float senTerm;    

    /* Crea el comando que va a ejecutar */
    memset(command, '\0', 200);
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_HUM_WRITE);
    strcat(command, " -n 01; sleep 1; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_HUM_READ);

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
        char* humBytes = strchr(resp, ':') + 2;
        char strRawHum[5];
        char strRawTemp[5];
        char strRawSen[5];

        if (strlen(humBytes) == 19) {
            memset(strRawSen, '\0', 5);
            strRawSen[0] = humBytes[15];
            strRawSen[1] = humBytes[16];
            strRawSen[2] = humBytes[12];
            strRawSen[3] = humBytes[13];
            rawSenTer = (uint16_t) strtol(strRawSen, NULL, 16);
        }

        memset(strRawHum, '\0', 5);
        strRawHum[0] = humBytes[9];
        strRawHum[1] = humBytes[10];
        strRawHum[2] = humBytes[6];
        strRawHum[3] = humBytes[7];

        memset(strRawTemp, '\0', 5);
        strRawTemp[0] = humBytes[3];
        strRawTemp[1] = humBytes[4];
        strRawTemp[2] = humBytes[0];
        strRawTemp[3] = humBytes[1];

        /* Pasa de string a unsigned int de 16 bits*/
        rawHum = (uint16_t) strtol(strRawHum, NULL, 16);
        rawTemp = (uint16_t) strtol(strRawTemp, NULL, 16);        
    } while(rawHum == 0 && rawTemp == 0);

    /* Convierte los datos */
    sensorHdc1000Convert(rawTemp, rawHum, &temp, &hum);
    /* Calcula la sensación térmica */
    if(rawSenTer != 0) {
        senTerm = rawSenTer/1000.0;  // Dividimos por 1000 para convertirlo
    } else {
        calculate_thermal_sesation(temp, hum/100, &senTerm);
    }

    /* Pasa el valor que se le pida */
    if (value == AMB_TMP_VALUE) {
        return temp;
    } else if (value == HUM_VALUE) {
        return hum;
    } else if (value == SEN_TMP_VALUE) {
        return senTerm;
    }

    return 0;
}