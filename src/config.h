#ifndef AZILE_CONFIG_H
#define AZILE_CONFIG_H

typedef struct
{
    char *shell;
    char *ansi_code_esc_seq_begin;
    char *ansi_code_esc_seq_end;
    char *prompt_end_symbol;
    char *prompt_end_symbol_ansi_code;
    char *dir_ansi_code;
    char *git_status_symbol;
    char *git_ansi_code;
} AZ_Config;

void az_config_setup(AZ_Config *cfg, char *shell);
void az_config_teardown(AZ_Config *cfg);

#endif // AZILE_CONFIG_H
