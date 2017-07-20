#ifndef BD_ERROR_H_
#define BD_ERROR_H_

#define BD_ERR_DESC_LENGTH 256

enum bd_error {
	BD_INIT_MEM,
	BD_CFG_DATADIR_NONE,
	BD_CFG_DATADIR_NODIR,
	BD_CFG_DATADIR_ACCESS
};

enum bd_error g_bd_lasterr;
char *g_bd_lasterr_desc;

#endif // BD_ERROR_H_
