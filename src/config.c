// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void az_config_setup(AZ_Config *cfg, char *shell)
{
    cfg->cwd = getcwd(NULL, 0);
    cfg->shell = shell;

    cfg->dir_color = getenv("AZILE_DIR_COLOR");
    if (cfg->dir_color == NULL)
        cfg->dir_color = "bright_yellow";

    cfg->dir_git_sep = getenv("AZILE_DIR_GIT_SEP");
    if (cfg->dir_git_sep == NULL)
        cfg->dir_git_sep = "::";

    cfg->dir_git_sep_color = getenv("AZILE_DIR_GIT_SEP_COLOR");
    if (cfg->dir_git_sep_color == NULL)
        cfg->dir_git_sep_color = "white";

    cfg->git_color = getenv("AZILE_GIT_COLOR");
    if (cfg->git_color == NULL)
        cfg->git_color = "bright_red";

    cfg->git_status_symbol = getenv("AZILE_GIT_STATUS_SYMBOL");
    if (cfg->git_status_symbol == NULL)
        cfg->git_status_symbol = "()";

    cfg->prompt_end_symbol = getenv("AZILE_PROMPT_END_SYMBOL");
    if (cfg->prompt_end_symbol == NULL)
        cfg->prompt_end_symbol = "";

    cfg->prompt_end_symbol_color = getenv("AZILE_PROMPT_END_SYMBOL_COLOR");
    if (cfg->prompt_end_symbol_color == NULL)
        cfg->prompt_end_symbol_color = "blue";

    // don't care what the value is, just if it exists in env variable, set to true
    char *prompt_end_symbol_newline = getenv("AZILE_PROMPT_END_SYMBOL_NEWLINE");
    cfg->prompt_end_symbol_newline = prompt_end_symbol_newline != NULL;
}

void az_config_teardown(AZ_Config *cfg)
{
    // Do NOT free vars set using getenv!

    if (cfg->cwd)
        free(cfg->cwd);
}
