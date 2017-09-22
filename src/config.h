#ifndef BD_CONFIG_H_
#define BD_CONFIG_H_

/***
 *
 * \short configuration
 *
 */
struct {
	char *datadir;
	char *editor;
} g_bd_config;

int bd_configHandler(void *user, const char *section, const char *name, const char *value);
int bd_Config(const char *configdir);
void bd_FreeConfig();

#endif // BD_CONFIG_H_
