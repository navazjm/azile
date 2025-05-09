// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#ifndef AZILE_CONFIG_H
#define AZILE_CONFIG_H

#include <stdbool.h>

typedef struct
{
    char *cwd;
    char *shell;
    char *dir_color;
    char *dir_git_sep;
    char *dir_git_sep_color;
    char *git_status_symbol;
    char *git_color;
    char *prompt_end_symbol;
    char *prompt_end_symbol_color;
    bool prompt_end_symbol_newline;
} AZ_Config;

/*
 * Set Azile's config with sane defaults.
 */
void az_config_setup(AZ_Config *cfg, char *shell);

/*
 * Free the memory allocated the config
 */
void az_config_teardown(AZ_Config *cfg);

/*
 * Prints all AZILE_ environment variables and their current values.
 */
void az_config_print_env(AZ_Config *cfg);

#endif // AZILE_CONFIG_H
