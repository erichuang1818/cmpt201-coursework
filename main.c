#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "helpers.c"
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

int main() {
  hist_init();
  while (1) {

    prompt_printer();

    // read in user input
    char *buffer = read_input();
    buffer[strcspn(buffer, "\n")] = '\0';
    if (buffer[0] == '\0')
      continue;

    char original_line[BUFFER_SIZE];
    strncpy(original_line, buffer, BUFFER_SIZE - 1);
    original_line[BUFFER_SIZE - 1] = '\0';

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

    if (strcmp(buffer, "!!") != 0 && buffer[0] != '!') {
      hist_add(original_line);
    }
    // handle according to command type
    switch (type) {
    case INTERNAL_CMD:
      // internal handler
      internal_handle(argv);
      continue;

    case HISTORY_CMD:
      // history handler
      if (strcmp(argv[0], "history") == 0) {
        /*
        hist_print();

      } else if (strcmp(argv[0], "!!") == 0) {

        const char *cmd = hist_recent();

        if(!cmd){
          write(STDERR_FILENO, "history: ", 9);
          write(STDERR_FILENO, HISTORY_NO_LAST_MSG,
      strlen(HISTORY_INVALID_MSG)); write(STDERR_FILENO, "\n", 1); break;
        }
        write(STDOUT_FILENO, cmd, strlen(cmd));
        write(STDOUT_FILENO, "\n", 1);

        */
      } else if (argv[0][0] == '!') {
      }
      continue;

    case EXTERNAL_CMD:
      // external handler
      external_handle(argv, bg);
      continue;

    default:
      break;
    }
  }
  return 0;
}
