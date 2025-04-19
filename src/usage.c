// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "usage.h"
#include <stdio.h>

void az_usage_print_full_help()
{
    printf("Usage: azile (command) [options]\n\n"
           "Commands:\n");
    az_usage_print_command_init();
    az_usage_print_command_prompt();
    az_usage_print_command_help();
}

void az_usage_print_command_init()
{
    printf("  init: Outputs shell-specific code to configure your prompt to use azile\n"
           "    Usage: azile init <shell>\n"
           "    Shell: bash, fish, zsh\n"
           "\n");
}

void az_usage_print_command_prompt()
{
    printf("  prompt: Builds the prompt string to render based on the shell option\n"
           "    Usage: azile prompt <shell>\n"
           "    Shell: bash, fish, zsh\n"
           "\n");
}

void az_usage_print_command_help()
{
    printf("  help: Displays azile usage help\n"
           "    Usage: azile help\n"
           "\n");
}
