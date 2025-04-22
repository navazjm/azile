// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "usage.h"
#include <stdio.h>

void az_usage_print_full_help()
{
    printf("Usage: azile (command) [options]\n"
           "\n"
           "Commands:\n"
           "  init: Outputs shell-specific code to configure your prompt to use azile\n"
           "    Usage: azile init <shell>\n"
           "    Shell: bash, fish, zsh\n"
           "\n"
           "  prompt: Builds the prompt string to render based on the shell option\n"
           "    Usage: azile prompt <shell>\n"
           "    Shell: bash, fish, zsh\n"
           "\n"
           "  cfg: Configure azile settings to your preferences\n"
           "    Usage: azile cfg (subcommand) [options]\n"
           "    Subcommands:\n"
           "      list: View all AZILE_ env vars and their values\n"
           "        Usage: azile cfg list\n"
           "\n"
           "  help: Show usage information\n"
           "    Usage: azile help\n"
           "\n"
           "  version: Show installed version of azile\n"
           "    Usage: azile version\n"
           "\n");
}

void az_usage_print_command_init()
{
    printf("Outputs shell-specific code to configure your prompt to use azile\n"
           "\n"
           "Usage: azile init <shell>\n"
           "  Shell: bash, fish, zsh\n"
           "\n");
}

void az_usage_print_command_prompt()
{
    printf("Builds the prompt string to render based on the shell option\n"
           "\n"
           "Usage: azile prompt <shell>\n"
           "  Shell: bash, fish, zsh\n"
           "\n");
}

void az_usage_print_command_cfg()
{
    printf("Configure azile settings to your preferences\n"
           "\n"
           "Usage: azile cfg (subcommand)\n"
           "\n"
           "Subcommands:\n"
           "  list: View all AZILE_ env vars and their values\n"
           "    Usage: azile cfg list\n"
           "\n");
}
