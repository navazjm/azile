#include "git2/repository.h"
#include "git2/status.h"
#include <git2.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *
ansi_text(const char *msg, uint8_t ansi_code)
{
    char *text;
    asprintf(&text, "\033[%dm%s\033[0m", ansi_code, msg);
    return text;
}

int
main()
{
    char *end_prompt_symbol = "";

    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        // no cwd, just render end_prompt_symbol (">>")
        printf("%s%s", ansi_text(end_prompt_symbol, 36), ansi_text("", 0));
        return EXIT_SUCCESS;
    }

    git_libgit2_init();
    git_repository *git_repo;
    git_repository_open_ext(&git_repo, cwd, GIT_REPOSITORY_OPEN_FROM_ENV, NULL);

    // no git repo, just render "dir >> "
    if (git_repo == NULL)
    {
        const char *home = getenv("HOME");
        // print full cwd if no home or not in home dir
        if (home == NULL || strncmp(cwd, home, strlen(home)) != 0)
        {
            printf("%s %s%s", ansi_text(cwd, 33), ansi_text(end_prompt_symbol, 36), ansi_text("", 0));
            git_libgit2_shutdown();
            return EXIT_SUCCESS;
        }
        // truncate cwd to ~ to represent home dir
        printf("%s%s %s%s", ansi_text("~", 33), ansi_text(cwd + strlen(home), 33), ansi_text(end_prompt_symbol, 36),
               ansi_text("", 0));
        git_libgit2_shutdown();
        return EXIT_SUCCESS;
    }

    char *full_prompt;

    // truncate until path begins with git root dir
    // Ex: /Users/michaelnavarro/repos/azile/src -> azile/src
    const char *workdir = git_repository_workdir(git_repo);
    char *git_root_dir = strdup(workdir);
    char *last_slash_pos = strrchr(git_root_dir, '/');
    *last_slash_pos = '\0'; // Temporarily terminate the string before the last slash
    char *second_last_slash = strrchr(git_root_dir, '/');
    memmove(git_root_dir, second_last_slash + 1, strlen(second_last_slash) + 1);
    char *git_root_dir_pos = strstr(cwd, git_root_dir);
    memmove(cwd, git_root_dir_pos, strlen(git_root_dir_pos) + 1);
    asprintf(&full_prompt, "%s", ansi_text(cwd, 33));

    git_reference *git_head_ref = NULL;
    git_repository_head(&git_head_ref, git_repo);
    const char *branch = git_reference_shorthand(git_head_ref);
    if (branch)
        asprintf(&full_prompt, "%s%s%s", full_prompt, ansi_text("::", 0), ansi_text(branch, 95));

    // display icon if git status is not clean
    git_status_options git_status_opts;
    git_status_options_init(&git_status_opts, GIT_STATUS_OPTIONS_VERSION);
    git_status_opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    git_status_opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX;
    git_status_list *git_status = NULL;
    git_status_list_new(&git_status, git_repo, &git_status_opts);
    size_t git_status_size = git_status_list_entrycount(git_status);
    if (git_status_size != 0)
    {
        char *status_symbol = ansi_text("()", 95);
        asprintf(&full_prompt, "%s%s", full_prompt, status_symbol);
    }

    git_repository_free(git_repo);
    git_reference_free(git_head_ref);
    git_status_list_free(git_status);

    asprintf(&full_prompt, "%s %s%s", full_prompt, ansi_text(end_prompt_symbol, 36), ansi_text("", 0));
    printf("%s", full_prompt);
    git_libgit2_shutdown();
    return EXIT_SUCCESS;
}
