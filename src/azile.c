#include <git2.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *
ansi_text(const char *msg, uint8_t ansi_code)
{
    // use %%{ and %%} to markers that identify the escape sequences. Without these,
    // zsh will count escape sequences as part of the display length of your prompt,
    // and you'll end up with bizarre cursor positioning and line editor behavior on longer commands

    char *text;
    asprintf(&text, "%%{\e[0;%dm%%}%s%%{\e[0m%%}", ansi_code, msg);

    return text;
}

int
main(void)
{
    char *prompt_end_symbol = "";

    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        // no cwd, just render end_prompt_symbol (">>")
        printf("%s", ansi_text(prompt_end_symbol, 36));
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
            printf("%s %s", ansi_text(cwd, 33), ansi_text(prompt_end_symbol, 36));
            git_libgit2_shutdown();
            return EXIT_SUCCESS;
        }
        // truncate cwd to ~ to represent home dir
        printf("%s%s %s", ansi_text("~", 33), ansi_text(cwd + strlen(home), 33), ansi_text(prompt_end_symbol, 36));
        git_libgit2_shutdown();
        return EXIT_SUCCESS;
    }

    char *prompt;

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
    asprintf(&prompt, "%s", ansi_text(cwd, 33));

    git_reference *git_head_ref = NULL;
    git_repository_head(&git_head_ref, git_repo);
    const char *branch = git_reference_shorthand(git_head_ref);
    if (branch)
        asprintf(&prompt, "%s%s%s", prompt, ansi_text("::", 0), ansi_text(branch, 95));

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
        asprintf(&prompt, "%s%s", prompt, status_symbol);
    }

    git_repository_free(git_repo);
    git_reference_free(git_head_ref);
    git_status_list_free(git_status);
    git_libgit2_shutdown();

    printf("%s %s", prompt, ansi_text(prompt_end_symbol, 36));
    fflush(stdout);
    return EXIT_SUCCESS;
}
