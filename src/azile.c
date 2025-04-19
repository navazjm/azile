// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "config.h"
#include "prompt.h"
#include "strings.h"
#include "usage.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        az_usage_print_full_help();
        return 1;
    }

    char *cmd = argv[1];

    if (strcmp(cmd, "init") == 0)
    {
        if (argc != 3)
        {
            az_usage_print_command_init();
            return 1;
        }

        char *shell = argv[2];

        if (strcmp(shell, "bash") == 0 || strcmp(shell, "zsh") == 0)
        {
            printf("PS1=$'$(azile prompt %s)'", shell);
        }
        else if (strcmp(shell, "fish") == 0)
        {
            printf("function fish_prompt\n"
                   "    azile prompt fish\n"
                   "end\n\n");
        }
        else
        {
            printf("ERROR: unknown shell {%s}\n", shell);
            az_usage_print_command_init();
            return 1;
        }
    }
    else if (strcmp(cmd, "prompt") == 0)
    {
        if (argc != 3)
        {
            az_usage_print_command_prompt();
            return 1;
        }

        char *shell = argv[2];
        AZ_Config cfg = {0};
        az_config_setup(&cfg, shell);
        AZ_String_Builder prompt = {0};
        az_prompt_build(&prompt, &cfg);
        printf("%s ", prompt.items ? prompt.items : "");
        fflush(stdout);
        az_config_teardown(&cfg);
        az_sb_free(&prompt);
    }
    else if (strcmp(cmd, "help") == 0)
    {
        az_usage_print_full_help();
    }
    else
    {
        printf("ERROR: unknown command {%s}\n", cmd);
        az_usage_print_full_help();
        return 1;
    }

    return 0;
}
