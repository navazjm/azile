// Copyright (c) 2025 Michael Navarro
// MIT license, see LICENSE for more details.

#include "prompt.h"
#include <assert.h>
#include <git2.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ESC_SEQ_BEGIN true
#define ESC_SEQ_END false
#define ANSI_RESET "\033[0m"

/*
 * Iterate through the global .gitconfig to get the init.defaultbranch
 *
 * Had issue with git_config_get_string not returning the value from
 * both "init.defaultbranch" and "init.defaultBranch" even though,
 * I had it set in my .gitconfig file.
 * The only workaround was to manually iterate through the config.
 */
static char *az_prompt_get_git_default_branch()
{
    git_config *git_global_cfg = NULL;
    git_config_iterator *git_global_cfg_iter = NULL;
    git_config_entry *git_global_cfg_entry = NULL;
    const char *git_default_branch = NULL;
    char *result = NULL;

    if (git_config_open_default(&git_global_cfg) == 0)
    {
        if (git_config_iterator_new(&git_global_cfg_iter, git_global_cfg) == 0)
        {
            while (git_config_next(&git_global_cfg_entry, git_global_cfg_iter) == 0)
            {
                if (git_global_cfg_entry && strcmp(git_global_cfg_entry->name, "init.defaultbranch") == 0)
                {
                    git_default_branch = git_global_cfg_entry->value;
                    if (git_default_branch)
                        result = strdup(git_default_branch);
                    break;
                }
            }
        }
    }

    if (git_global_cfg_iter)
        git_config_iterator_free(git_global_cfg_iter);
    if (git_global_cfg)
        git_config_free(git_global_cfg);

    return result;
}

/*
 * Based on the passed in shell, append the escape sequence to the prompt.
 * If is_begin, append the beginning ansi escape sequence. Otherwise, append the
 * ending ansi escape sequence.
 *
 * Allows the ansi codes to NOT be counted in the prompt length.
 * Otherwise, there could be weird behavior with cursor position
 * with stuff like tab completion, zsh auto-completion, and so on.
 */
static void az_prompt_append_escape_sequence(AZ_String_Builder *prompt, const char *shell, bool is_begin)
{
    if (strcmp(shell, "fish") == 0)
    {
        // Fish shell does not require escape sequence for ansi codes
        return;
    }

    const char *esc_seq = "";

    if (strcmp(shell, "bash") == 0)
    {
        // equivalent to =>  "\\["  : "\\]";
        esc_seq = is_begin ? "\001" : "\002";
    }
    else if (strcmp(shell, "zsh") == 0)
    {
        esc_seq = is_begin ? "%{" : "%}";
    }
    else
    {
        assert(false && "Unsupported shell");
        return;
    }

    az_sb_append(prompt, esc_seq);
}

/*
 * Appends the ansi code values to display text as a specific color
 *
 * Possible examples of colors
 *   red
 *   bold_cyan
 *   underline_blue
 *   bright_yellow
 *   bright_bold_magenta
 *   bright_underline_green
 *
 * But since we allow users to modify their colors by setting ENV VARS
 * and we do not validate their input, it is possible for the user to enter
 * ill-formatted colors. For Example they do not specify the actual color value,
 *   bright
 *   bold
 *   bold_underline
 *
 * Therefore we default the output to display in the color of red
 */
static void az_prompt_append_ansi_color_code(AZ_String_Builder *prompt, const char *color)
{
    if (color == NULL || strcmp(color, ANSI_RESET) == 0)
    {
        az_sb_append(prompt, ANSI_RESET);
        return;
    }

    az_sb_append(prompt, "\033[");
    char *modifier = "0;"; // no modifier (regular) by default
    if (strstr(color, "bold") != NULL)
        modifier = "1;";
    else if (strstr(color, "underline") != NULL)
        modifier = "4;";
    az_sb_append(prompt, modifier);

    // red by default, so if user does not specify color or color is ill-formatted (i.e., color = bright)
    // output is still readable as black or white are typically used for terminal bg
    uint8_t color_ansi_code = 31;
    if (strstr(color, "black") != NULL)
        color_ansi_code = 30;
    else if (strstr(color, "green") != NULL)
        color_ansi_code = 32;
    else if (strstr(color, "yellow") != NULL)
        color_ansi_code = 33;
    else if (strstr(color, "blue") != NULL)
        color_ansi_code = 34;
    else if (strstr(color, "magenta") != NULL)
        color_ansi_code = 35;
    else if (strstr(color, "cyan") != NULL)
        color_ansi_code = 36;
    else if (strstr(color, "white") != NULL)
        color_ansi_code = 37;

    // Bright (high intensity values range from 90-97
    if (strstr(color, "bright") != NULL)
        color_ansi_code += 60;

    char *temp_buf;
    asprintf(&temp_buf, "%um", color_ansi_code);
    az_sb_append(prompt, temp_buf);
    free(temp_buf);
}

/*
 * Builds the needed components to display text in a specific color and appends
 * it to the prompt.
 *
 * First, we append the ansi value of the provided color with the appropriate escape sequence
 * based on the passed in shell.
 * Then, we append the text to display to the user.
 * Finally, we append the reset ansi value, to reset the color back to the terminal's
 * default color.
 */
static void az_prompt_append_text(AZ_String_Builder *prompt, const char *text, const char *shell, const char *color)
{
    az_prompt_append_escape_sequence(prompt, shell, ESC_SEQ_BEGIN);
    az_prompt_append_ansi_color_code(prompt, color);
    az_prompt_append_escape_sequence(prompt, shell, ESC_SEQ_END);

    az_sb_append(prompt, text);

    az_prompt_append_escape_sequence(prompt, shell, ESC_SEQ_BEGIN);
    az_prompt_append_ansi_color_code(prompt, ANSI_RESET);
    az_prompt_append_escape_sequence(prompt, shell, ESC_SEQ_END);
}

void az_prompt_build(AZ_String_Builder *prompt, AZ_Config *cfg)
{
    if (cfg->cwd == NULL)
    {
        // Failed to get cwd, just append end prompt symbol
        az_prompt_append_text(prompt, cfg->prompt_end_symbol, cfg->shell, cfg->prompt_end_symbol_color);
        return;
    }

    git_libgit2_init();
    git_repository *git_repo;
    if (git_repository_open_ext(&git_repo, cfg->cwd, GIT_REPOSITORY_OPEN_FROM_ENV, NULL) != 0)
    {
        // no git repo, just render directory and prompt end symbol

        const char *home = getenv("HOME");
        if (home == NULL || strncmp(cfg->cwd, home, strlen(home)) != 0)
        {
            // print full cwd if no home dir found or home dir not a prefix of cwd
            az_prompt_append_text(prompt, cfg->cwd, cfg->shell, cfg->dir_color);

            if (cfg->prompt_end_symbol_newline)
                az_sb_append(prompt, "\n");
            else
                az_sb_append(prompt, " ");
            az_prompt_append_text(prompt, cfg->prompt_end_symbol, cfg->shell, cfg->prompt_end_symbol_color);

            git_libgit2_shutdown();
            return;
        }

        // truncate home path to "~"
        az_prompt_append_text(prompt, "~", cfg->shell, cfg->dir_color);
        az_prompt_append_text(prompt, cfg->cwd + strlen(home), cfg->shell, cfg->dir_color);
        if (cfg->prompt_end_symbol_newline)
            az_sb_append(prompt, "\n");
        else
            az_sb_append(prompt, " ");
        az_prompt_append_text(prompt, cfg->prompt_end_symbol, cfg->shell, cfg->prompt_end_symbol_color);

        git_libgit2_shutdown();
        return;
    }

    // truncate until path begins with git root dir
    // Ex: /Users/michaelnavarro/repos/azile/src -> azile/src
    const char *workdir = git_repository_workdir(git_repo);
    char *workdir_copy = strdup(workdir); // take ownership of workdir to modify it
    char *last_slash_pos = strrchr(workdir_copy, '/');
    *last_slash_pos = '\0'; // Temporarily terminate the string at the last slash
    char *second_last_slash_pos = strrchr(workdir_copy, '/');
    *second_last_slash_pos = '\0'; // Temporarily terminate the string at the second to last slash
    az_prompt_append_text(prompt, cfg->cwd + strlen(workdir_copy) + 1, cfg->shell, cfg->dir_color);
    az_prompt_append_text(prompt, cfg->dir_git_sep, cfg->shell, cfg->dir_git_sep_color);
    free(workdir_copy);

    git_reference *git_head_ref = NULL;
    if (git_repository_head(&git_head_ref, git_repo) == 0)
    {
        // get current checked out branch
        const char *branch = git_reference_shorthand(git_head_ref);
        if (branch)
            az_prompt_append_text(prompt, branch, cfg->shell, cfg->git_color);
    }
    else
    {
        // typically reach here when user is in newly created git dir, i.e., has no head reference yet.
        char *git_default_branch = az_prompt_get_git_default_branch();
        az_prompt_append_text(prompt, git_default_branch ? git_default_branch : "main", cfg->shell, cfg->git_color);
        if (git_default_branch)
            free(git_default_branch);
    }

    // display icon if git status is not clean
    git_status_options git_status_opts;
    git_status_options_init(&git_status_opts, GIT_STATUS_OPTIONS_VERSION);
    git_status_opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    git_status_opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX;
    git_status_list *git_status = NULL;
    git_status_list_new(&git_status, git_repo, &git_status_opts);
    size_t git_status_size = git_status_list_entrycount(git_status);
    if (git_status_size != 0)
        az_prompt_append_text(prompt, cfg->git_status_symbol, cfg->shell, cfg->git_color);

    if (cfg->prompt_end_symbol_newline)
        az_sb_append(prompt, "\n");
    else
        az_sb_append(prompt, " ");
    az_prompt_append_text(prompt, cfg->prompt_end_symbol, cfg->shell, cfg->prompt_end_symbol_color);

    git_repository_free(git_repo);
    git_reference_free(git_head_ref);
    git_status_list_free(git_status);
    git_libgit2_shutdown();
}
