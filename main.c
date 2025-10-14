#define _POSIX_C_SOURCE 200809L
#include "history.h"
#include "msgs.h"
#include "shell.h"
#include "sighand.h"

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
    write(STDERR_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
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
    char *read_err = "shell: unable to read command ";
    write(STDERR_FILENO, read_err, strlen(read_err));
  }
  return buffer;
}

// used ChatGPT to help think up of what type of functions are needed to parse
// the user inputs
int main() {
  while (1) {
    // print prompt
    prompt_printer();

    // read in user input
    char *buffer = read_input();
    buffer[strcspn(buffer, "\n")] = '\0';
    if (buffer[0] == '\0')
      continue;

    int bg = 0;
    char *saveptr;
    char *token;
    char *argv[64];
    int argc = 0;

    token = strtok_r(buffer, " \t", &saveptr);
    while (token) {
      if (strcmp(token, "&") == 0) {
        bg = 1;
        break;
      }
      argv[argc++] = token;
      token = strtok_r(NULL, " \t", &saveptr);
    }
    argv[argc] = NULL;

    CommandType type = get_command_type(argv);

    // handle according to command type
    switch (type) {
    case INTERNAL_CMD:
      // internal handler
      internal_handle(argv);
      break;

    case HISTORY_CMD:
      // history handler
      break;

    case EXTERNAL_CMD:
      // external handler
      external_handle(argv, bg);
      break;

    default:
      break;
    }
  }
  return 0;
}
