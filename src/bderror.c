#include <stdio.h>

#include "bderror.h"

void bd_ErrInit() {
	g_bd_lasterr_desc = malloc(sizeof(char) * BD_ERR_DESC_LENGTH);
}

void bd_Err(enum bd_error err, char *errstr) {
	g_bd_lasterr = err;
	g_bd_lasterr_desc = 
}
