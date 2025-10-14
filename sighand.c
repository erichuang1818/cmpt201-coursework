#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "sighand.h"
#include "msgs.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sigint_handler(int sig) {
  (void)sig;

  write(STDOUT_FILENO, "help: ", 6);
  write(STDOUT_FILENO, HELP_HELP_MSG, strlen(HELP_HELP_MSG));
  write(STDOUT_FILENO, "\n", 1);

  write(STDOUT_FILENO, "cd: ", 4);
  write(STDOUT_FILENO, CD_HELP_MSG, strlen(CD_HELP_MSG));
  write(STDOUT_FILENO, "\n", 1);

  write(STDOUT_FILENO, "exit: ", 6);
  write(STDOUT_FILENO, EXIT_HELP_MSG, strlen(EXIT_HELP_MSG));
  write(STDOUT_FILENO, "\n", 1);

  write(STDOUT_FILENO, "pwd: ", 6);
  write(STDOUT_FILENO, PWD_HELP_MSG, strlen(PWD_HELP_MSG));
  write(STDOUT_FILENO, "\n", 1);

  write(STDOUT_FILENO, "history: ", 9);
  write(STDOUT_FILENO, HISTORY_HELP_MSG, strlen(HISTORY_HELP_MSG));
  write(STDOUT_FILENO, "\n", 1);

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    write(STDOUT_FILENO, cwd, strlen(cwd));
    write(STDOUT_FILENO, "$ ", 2);
  } else {
    write(STDOUT_FILENO, "cwd: ", 6);
    write(STDOUT_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
    write(STDOUT_FILENO, "\n", 1);
  }
}
