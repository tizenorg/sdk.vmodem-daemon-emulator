/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

#include "fileio.h"

gchar *get_simulator_path(void)
{
    gchar *simulator_path = NULL;
    int length = 0;

#ifndef _WIN32
    simulator_path = (char *)malloc( 512);
    if(simulator_path == NULL){
	return NULL;
    }
    memset(simulator_path, 0, 512);

    length = readlink("/proc/self/exe", simulator_path, 512);
    simulator_path[length] = '\0';
#else
    gchar simulator_org_path[512] = {0, };
    GetModuleFileName(NULL, simulator_org_path, 512);
    simulator_path = change_path_to_slash(simulator_org_path);
#endif	

    /* exception precess */

    if (length < 0) {
	exit(-1);
    }

    if (length >= 512) {
	fprintf(stderr, "Path too long. Truncated.\n");
	exit(-1);
    }

    return simulator_path;
}

gchar *get_bin_path(void)
{
    gchar *simulator_path = NULL;
    simulator_path = get_simulator_path();

    gchar *bin_path = NULL;
    bin_path = g_path_get_dirname(simulator_path);

    if (simulator_path)
	g_free(simulator_path);

    return bin_path;
}

gchar *get_path(void)
{
    gchar *simulator_path = NULL;
    simulator_path = get_simulator_path();

    gchar *path = NULL;
    path = g_path_get_dirname(simulator_path);

    if (simulator_path)
	g_free(simulator_path);

    return path;
}

gchar *get_skin_path(void)
{
    gchar *path = NULL;
    path = get_path();
    if(path == NULL){
	return NULL;
    }

    gchar *skin_path = NULL;
    skin_path = (char *)malloc( 512);
    if(skin_path == NULL){
	return NULL;
    }
    memset(skin_path, 0, 512);
    strncpy(skin_path, path, 512);

    sprintf(skin_path, "%s%s", path, "/skins");

    g_free(path);

    return skin_path;
}

gchar *get_data_path(void)
{
    gchar *path = NULL;
    path = get_path();
    if(path == NULL){
	return NULL;
    }

    gchar *data_path = NULL;
    data_path = (char *)malloc( 512);
    if(data_path == NULL){
	return NULL;
    }
    memset(data_path, 0, 512);
    strncpy(data_path, path, 512);

    sprintf(data_path, "%s%s", path, "/data");

    g_free(path);

    return data_path;
}

gchar *get_kernel_path(void)
{
    gchar *path = NULL;
    path = get_data_path();
    if(path == NULL){
	return NULL;
    }

    gchar *kernel_path = NULL;
    kernel_path = (char *)malloc( 512);
    if(kernel_path == NULL){
	g_free(path);
	return NULL;
    }
    memset(kernel_path, 0, 512);
    strncpy(kernel_path, path, 512);

    sprintf(kernel_path, "%s%s", path, "/kernel-img");

    g_free(path);

    return kernel_path;
}


/**
 * @brief 	change_path_to_slash (\, \\ -> /)
 * @param	org path to change (C:\\test\\test\\test)
 * @return	changed path (C:/test/test/test)
 * @date    Nov 19. 2009
 * */
gchar *change_path_to_slash(gchar *org_path)
{	
    if (!org_path || strlen(org_path) < 1)
	return NULL;

    int	length, i = 0;
    length = strlen(org_path);

    gchar *changed_path = NULL;
    changed_path = (char *)malloc( 512);
    memset(changed_path, 0, 512);

    sprintf(changed_path, "%s", org_path);

    for (i = 0 ; i < length; i ++) {
	if (org_path[i] == '\\') {
	    changed_path[i] = '/';	
	}
    }

    return changed_path;
}

gchar *get_conf_path(void)
{	
    gchar *conf_path = NULL;

#ifndef _WIN32	
    conf_path = (char *)malloc( 512);
    if(conf_path == NULL){
	return NULL;
    }
    memset(conf_path, 0, 512);

    gchar *path = NULL;
    path = get_path();
    if(path == NULL){
	g_free(conf_path);
	return NULL;
    }

    strncpy(conf_path, path, 512);

    sprintf(conf_path, "%s%s", path, "/conf");

    g_free(path);
#else
    TCHAR APP_PATH[MAX_PATH];
    gchar org_conf_path[128] = {0, };

    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, APP_PATH);
    sprintf(org_conf_path, "%s%s", APP_PATH, PID_DIR);
    conf_path = change_path_to_slash(org_conf_path);		
#endif

    return conf_path;
}



/**
 * @brief 	get_simulator conf file path
 * */
gchar *get_simulator_conf_filepath(void)
{
    gchar *simulator_conf_filepath = NULL;
    simulator_conf_filepath = (char *)malloc( 512);
    memset(simulator_conf_filepath, 0, 512);

#ifndef _WIN32	
    if (!strncmp(g_get_user_name(), "root", 4))
	sprintf(simulator_conf_filepath, "/root%ssimulator.conf", PID_DIR);
    else
	sprintf(simulator_conf_filepath, "/home/%s%ssimulator.conf", g_get_user_name(), PID_DIR);
#else
    gchar *conf_path = get_conf_path();
    sprintf(simulator_conf_filepath, "%ssimulator.conf", conf_path);

    if (conf_path)
	g_free(conf_path);
#endif

    return simulator_conf_filepath;
}


/**
 * @brief 	get_default target conf file path
 * */
gchar *get_platform_conf_filepath(void)
{
    gchar *simultor_path = NULL;
    simultor_path = get_path();

    gchar *default_conf_filepath = NULL;
    default_conf_filepath = (char *)malloc( 512);
    memset(default_conf_filepath, 0, 512);

    sprintf(default_conf_filepath, "%s/.default_target.conf", simultor_path);

    if (simultor_path)
	g_free(simultor_path);

    return default_conf_filepath;
}


/**
  @brief	search target name in directory name
  @param 	directory_name: target directory name of scratchbox
  @return	target name
 */
char *search_target_name(char *directory_name)
{
    int directory_len = 0;
    int search_i = 0;
    char *found_name = NULL;

    directory_len = strlen(directory_name);

    /* search target name pointer */
    for (search_i = directory_len; search_i > 0; search_i--) {
	if (directory_name[search_i-1] == '/') {
	    found_name = directory_name + search_i;
	    return found_name;
	}
    }

    return NULL;
}

char *find_exec_path(char *line) {
    int loc;
    int find_loc = 0;
    char *ldpath = NULL;
    char *start_p = NULL;
    static char exec_path[512] = "";
    int nfound = 0;

#if 0
    // ld path -> factoryfs of bash_profile
    //ldpath = strstr(line, "LD_LIBRARY_PATH");
#endif

    ldpath = strstr(line, "FACTORY_FS");
    if (ldpath == NULL) {
	return NULL;
    }

    find_loc = (int)ldpath - (int)line;

    for (loc = 0; loc < find_loc;loc++) {
	if (line[loc] == '#') {
	    return NULL;
	}
    }

    for (loc = 0; loc < strlen(ldpath);loc++) {

	if (ldpath[loc] == '#') {
	    return NULL;
	}
	if (ldpath[loc] == '=') {
	    start_p = &ldpath[loc];
	    nfound = 1;
	    break;
	}
    }

    if (nfound == 0) {
	return NULL;
    }

#if 0
    // lib directory
    end_p = strstr(start_p, "/lib");
    if (end_p == NULL) {
	return NULL;
    }

    strncpy(exec_path, start_p+1, (int)end_p - (int)start_p -1);
#endif

    strncpy(exec_path, start_p+1, strlen(start_p+1)-1);


    return exec_path;
}

char *find_exec_path_by_bash_profile(char *target_path)
{
    char *exec_path;
    char pathname[BUFSIZ] = "";
    char line[BUFSIZ];
    FILE * fp;

    sprintf(pathname, "%s/root/.bash_profile", target_path);

    if ((fp = fopen(pathname, "r")) == NULL) {
	return NULL;
    }

    while (fgets(line, BUFSIZ, fp)) {
	exec_path = find_exec_path(line);

	if (exec_path != NULL) {
	    fclose(fp);
	    return exec_path;
	}
    }

    fclose(fp);
    return NULL;
}


char *find_ld_path(char *line) {
    int loc;
    char *ldpath = NULL;
    char *start_p = NULL;
    int find_loc = 0;

    ldpath = strstr(line, "LD_LIBRARY_PATH");
    if (ldpath == NULL) {
	return NULL;
    }

    find_loc = (int)ldpath - (int)line;

    for (loc = 0; loc < find_loc;loc++) {
	if (line[loc] == '#') {
	    return NULL;
	}
    }

    for (loc = 0; loc < strlen(ldpath);loc++) {
	if (ldpath[loc] == '#') {
	    return NULL;
	}
	if (ldpath[loc] == '=') {
	    start_p = &ldpath[loc];
	    break;
	}
    }

    return &ldpath[loc] + 1;
}

char *find_ld_path_by_bash_profile(char *target_path)
{
    char *exec_path;
    char pathname[BUFSIZ] = "";
    static char line[BUFSIZ] = "";
    FILE * fp;

    sprintf(pathname, "%s/root/.bash_profile", target_path);

    if ((fp = fopen(pathname, "r")) == NULL) {
	return NULL;
    }

    while (fgets(line, BUFSIZ, fp)) {
	exec_path = find_ld_path(line);

	if (exec_path != NULL) {
	    if (exec_path[strlen(exec_path) - 1] == '\n')
		exec_path[strlen(exec_path) - 1] = '\0';
	    fclose(fp);
	    return exec_path;
	}
    }

    fclose(fp);
    return NULL;
}


