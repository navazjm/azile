#include "prompt.h"
#include "strings.h"
#include <stdio.h>
#include <string.h>

void print_usage()
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
        AZ_String_Builder prompt = {0};
        az_prompt_build(&prompt, shell);
        printf("%s ", prompt.items ? prompt.items : "");
        // printf("\nlen: %lu\n", strlen(prompt->items));
        fflush(stdout);
    }
    else
    {
        printf("ERROR: unknown command {%s}\n", cmd);
        print_usage();
        return 1;
    }

    return 0;
}
