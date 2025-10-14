#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "msgs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef enum { EXTERNAL_CMD, INTERNAL_CMD, HISTORY_CMD, NO_CMD } CommandType;

typedef struct {
  CommandType type;
  char **argv;
} Command;

void prompt_printer() {
  char cwd[BUFFER_SIZE];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    write(STDOUT_FILENO, cwd, strlen(cwd));
    write(STDOUT_FILENO, "$ ", 2);
  } else {
    write(STDERR_FILENO, "cwd: ", 5);
    write(STDERR_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
    write(STDERR_FILENO, "\n", 1);
  }
}

CommandType get_command_type(char **argv) {
  if (!argv || !argv[0])
    return NO_CMD;

  if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "pwd") == 0 ||
      strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "help") == 0)
    return INTERNAL_CMD;

  if (strcmp(argv[0], "history") == 0 || strcmp(argv[0], "!!") == 0 ||
      argv[0][0] == '!')
    return HISTORY_CMD;

  return EXTERNAL_CMD;
}

char *read_input() {
  static char buffer[BUFFER_SIZE];
  ssize_t n = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);

  // plugs last input to be an endline
  buffer[n] = '\0';
  if (n <= 0)
    return NULL;

  // error handling
  if (n == -1) {
    write(STDERR_FILENO, "read: ", 6);
    write(STDERR_FILENO, READ_ERROR_MSG, strlen(READ_ERROR_MSG));
    write(STDERR_FILENO, "\n", 1);
  }
  return buffer;
}
