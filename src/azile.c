// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "config.h"
#include "prompt.h"
#include "strings.h"
#include "usage.h"
#include <stdio.h>
#include <string.h>

#define AZILE_VERSION "0.1.0"

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
            return 0;
        }
        else if (strcmp(shell, "fish") == 0)
        {
            printf("function fish_prompt\n"
                   "    azile prompt fish\n"
                   "end\n\n");
            return 0;
        }
        else if (strcmp(shell, "help") == 0)
        {
            az_usage_print_command_init();
            return 0;
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
        if (strcmp(shell, "help") == 0)
        {
            az_usage_print_command_prompt();
            return 0;
        }

        AZ_Config cfg = {0};
        az_config_setup(&cfg, shell);
        AZ_String_Builder prompt = {0};
        az_prompt_build(&prompt, &cfg);
        printf("%s ", prompt.items ? prompt.items : "");
        fflush(stdout);
        az_config_teardown(&cfg);
        az_sb_free(&prompt);
        return 0;
    }
    else if (strcmp(cmd, "cfg") == 0)
    {
        if (argc < 3)
        {
            az_usage_print_command_cfg();
            return 1;
        }

        char *sub_cmd = argv[2];

        if (strcmp(sub_cmd, "list") == 0)
        {

            if (argc != 3)
            {
                az_usage_print_command_cfg();
                return 1;
            }
            AZ_Config cfg = {0};
            az_config_setup(&cfg, NULL);
            az_config_print_env(&cfg);
            az_config_teardown(&cfg);
            return 0;
        }
        else if (strcmp(sub_cmd, "help") == 0)
        {

            az_usage_print_command_cfg();
            return 0;
        }
        else
        {
            printf("ERROR: unknown cfg subcommand {%s}\n", sub_cmd);
            az_usage_print_command_cfg();
            return 1;
        }
    }
    else if (strcmp(cmd, "help") == 0)
    {
        az_usage_print_full_help();
        return 0;
    }
    else if (strcmp(cmd, "version") == 0)
    {
        printf("Azile version => " AZILE_VERSION "\n");
        return 0;
    }
    else
    {
        printf("ERROR: unknown command {%s}\n", cmd);
        az_usage_print_full_help();
        return 1;
    }

    return 0;
}
