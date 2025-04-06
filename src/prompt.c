#include "prompt.h"
#include "config.h"
#include <git2.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *az_prompt_get_git_default_branch()
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

char *az_prompt_ansi_text(const char *msg, char *ansi_code, char *ansi_esc_seq_begin, char *ansi_esc_seq_end)
{
    char *text;
    if (ansi_esc_seq_begin == NULL)
        asprintf(&text, "\e[0;%sm%s\e[0m", ansi_code, msg);
    else
        asprintf(&text, "%s\e[0;%sm%s%s%s\e[0m%s", ansi_esc_seq_begin, ansi_code, ansi_esc_seq_end, msg,
                 ansi_esc_seq_begin, ansi_esc_seq_end);
    return text;
}

void az_prompt_build(AZ_String_Builder *prompt, char *shell)
{
    AZ_Config cfg = {0};
    az_config_setup(&cfg, shell);

    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return;

    git_libgit2_init();
    git_repository *git_repo;
    if (git_repository_open_ext(&git_repo, cwd, GIT_REPOSITORY_OPEN_FROM_ENV, NULL) != 0)
    {
        // no git repo, just render "dir >> "
        const char *home = getenv("HOME");
        // print full cwd if no home or not in home dir
        if (home == NULL || strncmp(cwd, home, strlen(home)) != 0)
        {
            az_sb_append_cstr(prompt, az_prompt_ansi_text(cwd, cfg.dir_ansi_code, cfg.ansi_code_esc_seq_begin,
                                                          cfg.ansi_code_esc_seq_end));
            az_sb_append_cstr(prompt, " ");

            az_sb_append_cstr(prompt, az_prompt_ansi_text(cfg.prompt_end_symbol, cfg.prompt_end_symbol_ansi_code,
                                                          cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
            az_config_teardown(&cfg);
            git_libgit2_shutdown();
            free(cwd);
            return;
        }
        // truncate home path to "~"
        az_sb_append_cstr(prompt, az_prompt_ansi_text("~", cfg.dir_ansi_code, cfg.ansi_code_esc_seq_begin,
                                                      cfg.ansi_code_esc_seq_end));
        az_sb_append_cstr(prompt, az_prompt_ansi_text(cwd + strlen(home), cfg.dir_ansi_code,
                                                      cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
        az_sb_append_cstr(prompt, " ");
        az_sb_append_cstr(prompt, az_prompt_ansi_text(cfg.prompt_end_symbol, cfg.prompt_end_symbol_ansi_code,
                                                      cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
        az_config_teardown(&cfg);
        git_libgit2_shutdown();
        free(cwd);
        return;
    }

    // truncate until path begins with git root dir
    // Ex: /Users/michaelnavarro/repos/azile/src -> azile/src
    const char *workdir = git_repository_workdir(git_repo);
    char *last_slash_pos = strrchr(workdir, '/');
    *last_slash_pos = '\0'; // Temporarily terminate the string at the last slash
    char *second_last_slash_pos = strrchr(workdir, '/');
    *second_last_slash_pos = '\0'; // Temporarily terminate the string at the second to last slash
    az_sb_append_cstr(prompt, az_prompt_ansi_text(cwd + strlen(workdir) + 1, cfg.dir_ansi_code,
                                                  cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
    az_sb_append_cstr(prompt, az_prompt_ansi_text("::", "0", cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
    // restore slashes
    *last_slash_pos = '/';
    *second_last_slash_pos = '/';

    git_reference *git_head_ref = NULL;
    if (git_repository_head(&git_head_ref, git_repo) == 0)
    {
        const char *branch = git_reference_shorthand(git_head_ref);
        if (branch)
            az_sb_append_cstr(prompt, az_prompt_ansi_text(branch, cfg.git_ansi_code, cfg.ansi_code_esc_seq_begin,
                                                          cfg.ansi_code_esc_seq_end));
    }
    else
    {
        // typically reach here when user is in newly created git dir, i.e., has no head reference yet.
        char *git_default_branch = az_prompt_get_git_default_branch();
        az_sb_append_cstr(prompt,
                          az_prompt_ansi_text(git_default_branch ? git_default_branch : "main", cfg.git_ansi_code,
                                              cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
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
        az_sb_append_cstr(prompt, az_prompt_ansi_text(cfg.git_status_symbol, cfg.git_ansi_code,
                                                      cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));
    az_sb_append_cstr(prompt, " ");

    git_repository_free(git_repo);
    git_reference_free(git_head_ref);
    git_status_list_free(git_status);

    az_sb_append_cstr(prompt, az_prompt_ansi_text(cfg.prompt_end_symbol, cfg.prompt_end_symbol_ansi_code,
                                                  cfg.ansi_code_esc_seq_begin, cfg.ansi_code_esc_seq_end));

    az_config_teardown(&cfg);
    git_libgit2_shutdown();
    free(cwd);
}
