all:
	$(CC) temperature_local.c read_*_ble.c -Wall -Wextra -o temperature_local -lmosquitto -lawa -lletmecreate_core -lletmecreate_click
