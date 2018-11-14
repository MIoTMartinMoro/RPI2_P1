# Proyecto temperature_local

## Compilar:

1. Añadir "+libmosquitto" a las dependencias (DEPENDS) del Makefile del direcorio padre.
2. Hacer el make de manera normal para compilar el ipk para la Ci40.
3. Cambiar a OFF los flags "LWS_SSL_CLIENT_USE_OS_CA_CERTS", "LWS_SSL_SERVER_WITH_ECDH_CERT" y "LWS_USE_*" del fichero ~/CreatorDev/creator-sdk/build_dir/target-mipsel_mips32r2_musl-1.1.15/libwebsockets-cyassl/libwebsockets-2.0.3/CMakeCache.txt.
4. Volver a hacer el paso 2 con sudo.