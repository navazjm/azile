// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "config.h"
#include "prompt.h"
#include "strings.h"
#include <stdio.h>
#include <string.h>

static void print_usage()
{
    printf("Usage: azile (init | prompt) <shell>\n");
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        print_usage();
        return 1;
    }

    char *cmd = argv[1];
    char *shell = argv[2];

    if (strcmp(cmd, "init") == 0)
    {
        if (strcmp(shell, "zsh") == 0)
            printf("PS1=$'$(azile prompt %s)'", shell);
        else
        {
            printf("ERROR: unknown shell {%s}\n", shell);
            print_usage();
            return 1;
        }
    }
    else if (strcmp(cmd, "prompt") == 0)
    {
        AZ_Config cfg = {0};
        az_config_setup(&cfg, shell);
        AZ_String_Builder prompt = {0};
        az_prompt_build(&prompt, &cfg);
        printf("%s ", prompt.items ? prompt.items : "");
        fflush(stdout);
        az_config_teardown(&cfg);
        az_sb_free(&prompt);
    }
    else
    {
        printf("ERROR: unknown command {%s}\n", cmd);
        print_usage();
        return 1;
    }

    return 0;
}
