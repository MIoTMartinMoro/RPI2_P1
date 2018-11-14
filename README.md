# Proyecto temperature_local

## Compilar:

1. Añadir "+libmosquitto" a las dependencias (DEPENDS) del Makefile del direcorio padre.
2. Hacer el make de manera normal para compilar el ipk para la Ci40.
3. Cambiar a OFF los flags "LWS_SSL_CLIENT_USE_OS_CA_CERTS", "LWS_SSL_SERVER_WITH_ECDH_CERT" y "LWS_USE_*".
4. Volver a hacer el paso 2 con sudo.