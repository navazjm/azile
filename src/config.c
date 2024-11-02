#include "config.h"
#include <stdlib.h>
#include <string.h>

void az_config_setup(AZ_Config *cfg) {
    // ansi_code_esc_seq_start, ansi_code_esc_seq_end -> allow the ansi codes to NOT be
    // counted in the prompt length. Otherwise, there could be weird behavior with cursor position
    // with stuff like tab completion, zsh auto-completion, and so on.

    cfg->shell = getenv("SHELL");
    cfg->ansi_code_esc_seq_start = NULL;
    cfg->ansi_code_esc_seq_end = NULL;
    if (cfg->shell != NULL) {
        if (strstr(cfg->shell, "zsh") != NULL) {
            cfg->ansi_code_esc_seq_start = "%%{";
            cfg->ansi_code_esc_seq_end = "%%}";
        } else if (strstr(cfg->shell, "bash") != NULL) {
            cfg->ansi_code_esc_seq_start = "\\[";
            cfg->ansi_code_esc_seq_end = "\\]";
        }
    }

    cfg->prompt_end_symbol = getenv("AZILE_PROMPT_END_SYMBOL");
    if (cfg->prompt_end_symbol == NULL) {
        cfg->prompt_end_symbol = "";
    }

    cfg->prompt_end_symbol_ansi_code = "36";
    cfg->dir_ansi_code = "33";
    cfg->git_ansi_code = "95";

    cfg->git_status_symbol = getenv("AZILE_GIT_STATUS_SYMBOL");
    if (cfg->git_status_symbol == NULL) {
        cfg->git_status_symbol = "()";
    }
}

void az_config_teardown(AZ_Config *cfg) { (void)cfg; }