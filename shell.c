#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include "shell.h"
#include "msgs.h"
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
      char *EXIT_ERR = "exit: ";
      write(STDERR_FILENO, EXIT_ERR, strlen(EXIT_ERR));
      write(STDERR_FILENO, TMA_MSG, strlen(TMA_MSG));
      char *NEWL = "\n";
      write(STDERR_FILENO, NEWL, strlen(NEWL));
    }

    else
      exit(0);

    ////////////////////////////////////////////////////////////////////////////////////////////
    // PWD HANDLER
  } else if (strcmp(argv[0], "pwd") == 0) {

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      write(STDOUT_FILENO, cwd, strlen(cwd));
      write(STDOUT_FILENO, "$\n", 2);
    } else {
      write(STDERR_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    // CD HANDLER (FIX ME FIX ME)
  } else if (strcmp(argv[0], "cd") == 0) {
    static char prev_dir[1024] = "";
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
      write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
      return;
    }

    // IF THERES TOO MANY INPUTS
    if (argv[2] != NULL) {
      write(STDOUT_FILENO, "cd: ", 4);
      write(STDOUT_FILENO, TMA_MSG, strlen(TMA_MSG));
      write(STDOUT_FILENO, "\n", 1);
      return;
    }

    const char *target = argv[1];
    char new_dir[1024];
    new_dir[0] = '\0';

    // COPY HELPERS
    int i = 0, j = 0;

    // GO HOME WITH NOTHING OR ~
    if (target == NULL || strcmp(target, "~") == 0) {
      const char *home = getenv("HOME");
      if (home == NULL) {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }
      i = 0;
      while (home[i] != '\0' && i < 1023) {
        new_dir[i] = home[i];
        i++;
      }
      new_dir[i] = '\0';
      /*(
      if (chdir(home) != 0) {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }
      */
    }
    // GO SOMEHWERE WITH ~/SOME/PATH
    else if (target[0] == '~') {
      const char *home = getenv("HOME");
      if (home == NULL) {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }
      i = 0;
      while (home[i] != '\0' && i < 1023) {
        new_dir[i] = home[i];
        i++;
      }

      j = 1;
      while (target[j] != '\0' && i < 1023) {
        new_dir[i++] = target[j++];
      }
      new_dir[i] = '\0';
    }
    // GO TO PREVIOUS WITH -
    else if (strcmp(target, "-") == 0) {
      if (prev_dir[0] == '\0') {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }
      write(STDERR_FILENO, prev_dir, strlen(prev_dir));
      write(STDERR_FILENO, "\n", 1);

      char temp[1024];
      i = 0;
      while (cwd[1] != '\0' && i < 1023) {
        temp[i] = cwd[i];
        i++;
      }
      temp[i] = '\0';
      if (chdir(prev_dir) != 0) {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }
      i = 0;
      while (temp[i] != '\0' && i < 1023) {
        prev_dir[i] = temp[i];
        i++;
      }
      prev_dir[i] = '\0';

      return;
    }

    else {
      i = 0;
      while (target[i] != '\0' && i < 1023) {
        new_dir[i] = target[i];
        i++;
      }
      new_dir[i] = '\0';

      if (chdir(new_dir) != 0) {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }

      i = 0;
      while (cwd[i] != '\0' && i < 1023) {
        prev_dir[i] = cwd[i];
        i++;
      }
      prev_dir[i] = '\0';
      /*
      if (chdir(target) != 0) {
        write(STDERR_FILENO, CHDIR_ERROR_MSG, strlen(CHDIR_ERROR_MSG));
        return;
      }
      */
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // HELP HANDLER
  } else if (strcmp(argv[0], "help") == 0) {
    char *NEWL = "\n";

    if (argv[1] == NULL) {
      char *HELP_EXIT = "exit: ";
      write(STDOUT_FILENO, HELP_EXIT, strlen(HELP_EXIT));
      write(STDOUT_FILENO, EXIT_HELP_MSG, strlen(EXIT_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
      char *HELP_CD = "cd: ";
      write(STDOUT_FILENO, HELP_CD, strlen(HELP_CD));
      write(STDOUT_FILENO, CD_HELP_MSG, strlen(CD_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
      char *HELP_PWD = "pwd: ";
      write(STDOUT_FILENO, HELP_PWD, strlen(HELP_PWD));
      write(STDOUT_FILENO, PWD_HELP_MSG, strlen(PWD_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
      char *HELP_HELP = "help: ";
      write(STDOUT_FILENO, HELP_HELP, strlen(HELP_HELP));
      write(STDOUT_FILENO, HELP_HELP_MSG, strlen(HELP_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
      return;
    }

    if (argv[2] != NULL) {
      char *TMY = "cd: ";
      write(STDOUT_FILENO, TMY, strlen(TMY));
      write(STDOUT_FILENO, TMA_MSG, strlen(TMA_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
      return;
    }

    if (strcmp(argv[1], "exit") == 0) {
      char *HELP_EXIT = "exit: ";
      write(STDOUT_FILENO, HELP_EXIT, strlen(HELP_EXIT));
      write(STDOUT_FILENO, EXIT_HELP_MSG, strlen(EXIT_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
    } else if (strcmp(argv[1], "cd") == 0) {
      char *HELP_CD = "cd: ";
      write(STDOUT_FILENO, HELP_CD, strlen(HELP_CD));
      write(STDOUT_FILENO, CD_HELP_MSG, strlen(CD_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
    } else if (strcmp(argv[1], "pwd") == 0) {
      char *HELP_PWD = "pwd: ";
      write(STDOUT_FILENO, HELP_PWD, strlen(HELP_PWD));
      write(STDOUT_FILENO, PWD_HELP_MSG, strlen(PWD_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
    } else if (strcmp(argv[1], "help") == 0) {
      char *HELP_HELP = "help: ";
      write(STDOUT_FILENO, HELP_HELP, strlen(HELP_HELP));
      write(STDOUT_FILENO, HELP_HELP_MSG, strlen(HELP_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
    } else {
      write(STDOUT_FILENO, argv[1], strlen(argv[1]));
      char *EXT = ": ";
      write(STDOUT_FILENO, EXT, strlen(EXT));
      write(STDOUT_FILENO, EXTERN_HELP_MSG, strlen(EXTERN_HELP_MSG));
      write(STDOUT_FILENO, NEWL, strlen(NEWL));
    }
  }
}

void external_handle(char **argv, int bg) {
  pid_t pid = fork();

  if (pid == -1) {
    write(STDERR_FILENO, FORK_ERROR_MSG, strlen(FORK_ERROR_MSG));
    return;
  }

  if (pid == 0) {
    execvpe(argv[0], argv, environ);
    //
  } else {
    if (!bg) {
      waitpid(pid, NULL, 0);
    }
  }
}
