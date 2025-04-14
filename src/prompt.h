#ifndef AZILE_PROMPT_H
#define AZILE_PROMPT_H

#include "config.h"
#include "strings.h"

/*
 * The heart of the Azile shell prompt. Builds each individual component
 * (dir, git branch, git status, and end prompt) of the shell prompt.
 *
 */
void az_prompt_build(AZ_String_Builder *prompt, AZ_Config *cfg);

#endif // AZILE_PROMPT_H
