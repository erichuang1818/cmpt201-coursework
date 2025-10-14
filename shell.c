#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include "shell.h"
#include "msgs.h"
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void internal_handle(char **argv) {
  /////////////////////////////////////////////////////////////////////////////////////////////
  // EXIT HANDLER
  if (strcmp(argv[0], "exit") == 0) {

    // error checks exit
    if (argv[1]) {
      write(STDERR_FILENO, "exit: ", 6);
      write(STDERR_FILENO, TMA_MSG, strlen(TMA_MSG));
      write(STDERR_FILENO, "\n", 1);
    }

    else
      exit(0);

    ////////////////////////////////////////////////////////////////////////////////////////////
    // PWD HANDLER
  } else if (strcmp(argv[0], "pwd") == 0) {
    if (argv[1] != NULL) {
      write(STDERR_FILENO, "pwd: ", 5);
      write(STDERR_FILENO, TMA_MSG, strlen(TMA_MSG));
      write(STDERR_FILENO, "\n", 1);
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      write(STDOUT_FILENO, cwd, strlen(cwd));
      write(STDOUT_FILENO, "\n", 1);
    } else {
      write(STDERR_FILENO, "pwd: ", 5);
      write(STDERR_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
      write(STDERR_FILENO, "\n", 1);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    // CD HANDLER
  } else if (strcmp(argv[0], "cd") == 0) {
    static char prev_dir[1024] = "";
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
      write(STDERR_FILENO, "cd: ", 4);
      write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
      write(STDERR_FILENO, "\n", 1);
      return;
    }

    // IF THERES TOO MANY INPUTS
    if (argv[2] != NULL) {
      write(STDERR_FILENO, "cd: ", 4);
      write(STDERR_FILENO, TMA_MSG, strlen(TMA_MSG));
      write(STDERR_FILENO, "\n", 1);
      return;
    }

    const char *target = argv[1];

    // GO HOME WITH NOTHING OR ~
    if (target == NULL || strcmp(target, "~") == 0) {
      const char *home = getenv("HOME");
      if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw)
          home = pw->pw_dir;
      }
      if (!home || chdir(home) != 0) {
        write(STDERR_FILENO, "cd: ", 4);
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        write(STDERR_FILENO, "\n", 1);
        return;
      }
    }

    // GO HOME WITH ~
    else if (target[0] == '~') {
      const char *home = getenv("HOME");
      if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw)
          home = pw->pw_dir;
      }
      if (!home) {
        write(STDERR_FILENO, "cd: ", 4);
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        write(STDERR_FILENO, "\n", 1);
        return;
      }

      char full_path[1024];
      size_t home_len = strlen(home);
      size_t extra_len = strlen(target + 1);
      if (home_len + extra_len >= sizeof(full_path)) {
        write(STDERR_FILENO, "cd: ", 4);
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        write(STDERR_FILENO, "\n", 1);
        return;
      }

      memcpy(full_path, home, home_len);
      memcpy(full_path + home_len, target + 1, extra_len + 1);

      if (chdir(full_path) != 0) {
        write(STDERR_FILENO, "cd: ", 4);
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        write(STDERR_FILENO, "\n", 1);
        return;
      }
    }

    // GO HOME WITH -
    else if (strcmp(target, "-") == 0) {
      if (prev_dir[0] == '\0') {
        const char *home = getenv("HOME");
        if (!home) {
          struct passwd *pw = getpwuid(getuid());
          if (pw)
            home = pw->pw_dir;
        }
        if (!home || chdir(home) != 0) {
          write(STDERR_FILENO, "cd: ", 4);
          write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
          write(STDERR_FILENO, "\n", 1);
          return;
        }
      } else {
        write(STDOUT_FILENO, prev_dir, strlen(prev_dir));
        write(STDOUT_FILENO, "\n", 1);
        char temp[1024];
        strcpy(temp, cwd);

        if (chdir(prev_dir) != 0) {
          write(STDERR_FILENO, "cd: ", 4);
          write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
          write(STDERR_FILENO, "\n", 1);
          return;
        }
        strcpy(prev_dir, temp);
        return;
      }
    } else {
      // STANDARD CD
      if (chdir(target) != 0) {
        write(STDERR_FILENO, "cd: ", 4);
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        write(STDERR_FILENO, "\n", 1);
      }
    }
    strcpy(prev_dir, cwd);

    ////////////////////////////////////////////////////////////////////////////////////////////
    // HELP HANDLER
  } else if (strcmp(argv[0], "help") == 0) {

    if (argv[1] == NULL) {
      write(STDOUT_FILENO, "help: ", 6);
      write(STDOUT_FILENO, HELP_HELP_MSG, strlen(HELP_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
      write(STDOUT_FILENO, "cd: ", 4);
      write(STDOUT_FILENO, CD_HELP_MSG, strlen(CD_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
      write(STDOUT_FILENO, "exit: ", 6);
      write(STDOUT_FILENO, EXIT_HELP_MSG, strlen(EXIT_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
      write(STDOUT_FILENO, "pwd: ", 5);
      write(STDOUT_FILENO, PWD_HELP_MSG, strlen(PWD_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
      write(STDOUT_FILENO, "history: ", 9);
      write(STDOUT_FILENO, HISTORY_HELP_MSG, strlen(PWD_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
      return;
    }

    if (argv[2] != NULL) {
      char *TMY = "cd: ";
      write(STDOUT_FILENO, TMY, strlen(TMY));
      write(STDOUT_FILENO, TMA_MSG, strlen(TMA_MSG));
      write(STDOUT_FILENO, "\n", 1);
      return;
    }

    if (strcmp(argv[1], "exit") == 0) {
      char *HELP_EXIT = "exit: ";
      write(STDOUT_FILENO, HELP_EXIT, strlen(HELP_EXIT));
      write(STDOUT_FILENO, EXIT_HELP_MSG, strlen(EXIT_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);

    } else if (strcmp(argv[1], "cd") == 0) {
      char *HELP_CD = "cd: ";
      write(STDOUT_FILENO, HELP_CD, strlen(HELP_CD));
      write(STDOUT_FILENO, CD_HELP_MSG, strlen(CD_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);

    } else if (strcmp(argv[1], "pwd") == 0) {
      char *HELP_PWD = "pwd: ";
      write(STDOUT_FILENO, HELP_PWD, strlen(HELP_PWD));
      write(STDOUT_FILENO, PWD_HELP_MSG, strlen(PWD_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);

    } else if (strcmp(argv[1], "help") == 0) {
      char *HELP_HELP = "help: ";
      write(STDOUT_FILENO, HELP_HELP, strlen(HELP_HELP));
      write(STDOUT_FILENO, HELP_HELP_MSG, strlen(HELP_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
    } else {
      write(STDOUT_FILENO, argv[1], strlen(argv[1]));
      char *EXT = ": ";
      write(STDOUT_FILENO, EXT, strlen(EXT));
      write(STDOUT_FILENO, EXTERN_HELP_MSG, strlen(EXTERN_HELP_MSG));
      write(STDOUT_FILENO, "\n", 1);
    }
  }
}

void external_handle(char **argv, int bg) {
  pid_t pid = fork();

  if (pid == -1) {
    write(STDERR_FILENO, "shell: ", 7);
    write(STDERR_FILENO, FORK_ERROR_MSG, strlen(FORK_ERROR_MSG));
    write(STDERR_FILENO, "\n", 1);
    return;
  }

  // ChatGPT used to help mend some broken logic
  if (pid == 0) {
    execvpe(argv[0], argv, environ);
    write(STDERR_FILENO, "shell: ", 7);
    write(STDERR_FILENO, EXEC_ERROR_MSG, strlen(EXEC_ERROR_MSG));
    write(STDERR_FILENO, "\n", 1);
    _exit(1);
  } else {
    if (!bg) {
      if (waitpid(pid, NULL, 0) != 0) {
        write(STDERR_FILENO, "shell: ", 7);
        write(STDERR_FILENO, WAIT_ERROR_MSG, strlen(EXEC_ERROR_MSG));
        write(STDERR_FILENO, "\n", 1);
      }
    }
  }
}

void zombie_reaper() {
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
  }
}
