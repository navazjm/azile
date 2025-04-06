#ifndef AZILE_PROMPT_H
#define AZILE_PROMPT_H

#include "strings.h"

char *az_prompt_get_git_default_branch();
char *az_prompt_ansi_text(const char *msg, char *ansi_code, char *ansi_esc_seq_begin, char *ansi_esc_seq_end);
void az_prompt_build(AZ_String_Builder *prompt, char *shell);

#endif // AZILE_PROMPT_H
