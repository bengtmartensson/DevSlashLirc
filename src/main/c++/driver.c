/**
 *  @file driver.c
 *  @author Alec Leamas
 *  @date August 2014
 *  @license GPL2 or later
 *  @brief Implements driver.h
 *
 * Access and support for driver.h, the global driver.
 */

#include	<stdio.h>
#include	<lirc/driver.h>
//#include	"config.h"
#include	<lirc/lirc_log.h>

#define LIRC_DRIVER_DEVICE "/dev/lirc0"

static const logchannel_t logchannel = LOG_LIB;

/**
 * The global driver data that drivers etc are accessing.
 * Set by hw_choose_driver().
 */
struct driver drv= {
	.name		= "null",
	.device		= "/dev/null",
	.features	= 0,
	.send_mode	= 0,
	.rec_mode	= 0,
	.code_length	= 0,
	.init_func	= NULL,
	.deinit_func	= NULL,
	.send_func	= NULL,
	.rec_func	= NULL,
	.decode_func	= NULL,
	.readdata	= NULL,
	.drvctl_func	= default_drvctl,
	.open_func	= default_open,
	.close_func	= default_close,
	.api_version	= 2,
	.driver_version = "0.9.2"
};

/** sscanf format to parse option_t. */
const char* const OPTION_FMT = "%32s%64s";

/** Read-only access to drv for client code. */
const struct driver* const curr_driver = &drv;

/** Allocation chunk in  glob_t_* routines. */
const int GLOB_CHUNK_SIZE = 32;


void glob_t_init(glob_t* glob)
{
	memset(glob, 0, sizeof(glob_t));
	glob->gl_offs = GLOB_CHUNK_SIZE;
	glob->gl_pathv = (char**) calloc(glob->gl_offs, sizeof(char*));
}


void glob_t_add_path(glob_t* glob, const char* path)
{
	if (glob->gl_pathc >= glob->gl_offs) {
		glob->gl_offs += GLOB_CHUNK_SIZE;
		glob->gl_pathv = realloc(glob->gl_pathv,
					 glob->gl_offs * sizeof(char*));
	}
	glob->gl_pathv[glob->gl_pathc] = strdup(path);
	glob->gl_pathc += 1;
}


void glob_t_free(glob_t* glob)
{
	unsigned int i;

	for (i = 0; i < glob->gl_pathc; i += 1)
		free(glob->gl_pathv[i]);
	free(glob->gl_pathv);
}


int default_open(const char* path)
{
	static char buff[128];

	if (path == NULL) {
		if (drv.device == NULL)
			drv.device = LIRC_DRIVER_DEVICE;
	} else {
		strncpy(buff, path, sizeof(buff) - 1);
		drv.device = buff;
	}
	log_info("Initial device: %s", drv.device);
	return 0;
}

int default_close(void)
{
	return 0;
}

int default_drvctl(unsigned int fd __attribute__((unused)), void* arg __attribute__((unused)))
{
	return DRV_ERR_NOT_IMPLEMENTED;
}


int drv_handle_options(const char* options)
{
	char* s;
	char* token;
	struct option_t option;
	int found;
	char* colon;
	int result;

	s = alloca(strlen(options) + 1);
	strcpy(s, options);
	for (token = strtok(s, "|"); token != NULL; token = strtok(NULL, "|")) {
		colon = strstr(token, ":");
		if (colon == NULL)
			return DRV_ERR_BAD_OPTION;
		*colon = ' ';
		found = sscanf(token, OPTION_FMT, option.key, option.value);
		if (found != 2)
			return DRV_ERR_BAD_OPTION;
		if (!curr_driver->drvctl_func)
			continue;
		result = curr_driver->drvctl_func(DRVCTL_SET_OPTION, (void*) &option);
		if (result != 0)
			return result;
	}
	return 0;
}
