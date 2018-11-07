all:
	$(CC) temperature_local.c -Wall -Wextra -o temperature_local -lawa -lletmecreate_core -lletmecreate_click
