#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

typedef unsigned char uint8_t;        /* Declara nuevo tipo unsigned int de 8 bits */
typedef unsigned short int uint16_t;  /* Declara nuevo tipo unsigned int de 16 bits */
typedef unsigned int uint32_t;        /* Declara nuevo tipo unsigned int de 32 bits */

/************************************************************************************************/
/* MACs */
#define BLE_MAC_ALDA        "24:71:89:CC:1D:02"  /* Dirección MAC del SensorTag */
#define BLE_MAC_CARLOS      "B0:91:22:F7:38:81"  /* Dirección MAC del SensorTag */
#define BLE_MAC             BLE_MAC_ALDA

/************************************************************************************************/
/* Valores a pedir */
#define AMB_TMP_VALUE   0x95
#define BMP_VALUE       0x96
#define HUM_VALUE       0x97
#define OBJ_TMP_VALUE   0x98
#define OPT_TMP_VALUE   0x99
#define SEN_TMP_VALUE   0xA0
#define OP_ERROR        0xFF  /* error */
#define OP_RESULTADO	0xBB

/************************************************************************************************/
/* Handles */
#define HANDL_BMP_WRITE   "0x37"  /* Handle para activar el sensor de presión */
#define HANDL_BMP_READ    "0x34"  /* Handle para leer del sensor de presión */

#define HANDL_HUM_WRITE   "0x2f"  /* Handle para activar el sensor de humedad */
#define HANDL_HUM_READ    "0x2c"  /* Handle para leer del sensor de humedad */

#define HANDL_OPT_WRITE   "0x47"  /* Handle para activar el sensor óptico */
#define HANDL_OPT_READ    "0x44"  /* Handle para leer del sensor óptico */

#define HANDL_TMP_WRITE   "0x27"  /* Handle para activar el sensor de temperatura */
#define HANDL_TMP_READ    "0x24"  /* Handle para leer del sensor de temperatura */

/************************************************************************************************/
/* Datos para el servidor */
#define PUERTO 3490     /* puerto en el servidor */
#define BACKLOG 10      /* numero maximo de conexiones pendientes en cola */
#define MAXDATASIZE 256 /* maximo numero de bytes que podemos recibir */

#define ID_HEADER_LEN sizeof (uint8_t) * 3

/* formato de la unidad de datos de aplicacion para Datagramas*/
struct idappdata
{
    uint8_t op;                               /* codigo de operacion */
    uint8_t id;                               /* identificador */
    uint8_t len;                              /* longitud de datos */
    char data[MAXDATASIZE - ID_HEADER_LEN];   /* datos */
};

/************************************************************************************************/
/* Funciones de los sensores */
/* Temperatura */
float read_tmp (uint8_t value, uint8_t* op);
void sensorTmp007Convert(uint16_t rawAmbTemp, uint16_t rawObjTemp, float* tAmb, float* tObj);

/* Luminosidad */
float read_opt(uint8_t* op);
float sensorOpt3001Convert (uint16_t rawData);

/* Presión */
float read_bmp(uint8_t value, uint8_t* op);
float sensorBmp280Convert(uint32_t rawValue);

/* Humedad */
float read_hum(uint8_t value, uint8_t* op);
void sensorHdc1000Convert(uint16_t rawTempFun, uint16_t rawHumFun, float *tempFun, float *humFun);
void calculate_thermal_sesation(float tempFun, float humFun, float *senTermFun);
