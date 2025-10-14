#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "history.h"
#include "msgs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// history buffer
static char history[HISTORY_SIZE][MAX_CMD_LEN];
// parallel array to store command numbers
static int history_num[HISTORY_SIZE];

// number of commands in history
static int history_count = 0;
// index in the circular array buffer where the next would be stored
static int history_index = 0;
// number of valid entries in history
static int next_cmd_num = 0;

void hist_init() {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    // everything is empty at first
    history[i][0] = '\0';
    history_num[i] = -1;
  }
  history_count = 0;
  history_index = 0;
  next_cmd_num = 0;
}

void hist_add(const char *cmd) {
  // returns upon detecting that there is no input
  if (!cmd || cmd[0] == '\0')
    return;

  size_t len = strlen(cmd);
  if (len >= MAX_CMD_LEN)
    len = MAX_CMD_LEN - 1;

  // copies the command into the circlar history buffer
  memcpy(history[history_index], cmd, len);
  history[history_index][len] = '\0';
  history_num[history_index] = next_cmd_num++;

  // moves the pointer in accordance to circular buffer
  history_index = (history_index + 1) % HISTORY_SIZE;

  if (history_count < HISTORY_SIZE) {
    history_count++;
  }
}

void hist_print() {
  char numbuf[32];

  for (int i = 0; i < history_count; i++) {
    int idx = (history_index - 1 - i + HISTORY_SIZE) % HISTORY_SIZE;
    int num = history_count - 1 - i;

    int n = num;
    int len = 0;
    if (n == 0) {
      numbuf[len++] = '0';
    } else {
      char rev[32];
      int rlen = 0;
      while (n > 0 && rlen < 32) {
        rev[rlen++] = '0' + (n % 10);
        n /= 10;
      }
      for (int j = rlen - 1; j >= 0; j--) {
        numbuf[len++] = rev[j];
      }
    }

    write(STDOUT_FILENO, numbuf, len);
    write(STDOUT_FILENO, "\t", 1);
    write(STDOUT_FILENO, history[idx], strlen(history[idx]));
    write(STDOUT_FILENO, "\n", 1);
  }
}

const char *hist_recent() {
  if (history_count == 0)
    return NULL;

  int idx = (history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE;
  return history[idx];
}

const char *hist_index(int index) {
  for (int i = 0; i < history_count; i++) {
    int idx = (history_index - 1 - i + HISTORY_SIZE) % HISTORY_SIZE;
    if (history_num[idx] == index) {
      return history[idx];
    }
  }
  return NULL;
}

int hist_count() { return history_count; }
