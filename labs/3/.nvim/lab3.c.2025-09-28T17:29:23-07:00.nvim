#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define HISTORY_SIZE 5

static void trim_newLine(char *s) {
  if (!s)
    return;
  size_t len = strlen(s);
  while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
    s[--len] = '\0';
  }
}

static void add_history(char *history[], int *start, int *count, char *line) {
  if (*count < HISTORY_SIZE) {
    int pos = (*start + *count) % HISTORY_SIZE;
    history[pos] = line;
    (*count)++;
  } else {
    free(history[*start]);
    history[*start] = line;
    *start = (*start + 1) % HISTORY_SIZE;
  }
}

static void print_history(char *history[], int start, int count) {
  for (int i = 0; i < count; ++i) {
    int idx = (start + i) % HISTORY_SIZE;

    printf("%s\n", history[idx]);
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {0};
  int start = 0;
  int count = 0;

  char *line = NULL;
  size_t cap = 0;
  ssize_t nread;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    nread = getline(&line, &cap, stdin);
    if (nread == -1) {

      if (feof(stdin)) {
        printf("\n(EOF)\n");
      } else {
        perror("getline");
      }
      break;
    }

    trim_newLine(line);

    char *stored = strdup(line);
    if (!stored) {
      perror("strdup");
      break;
    }

    add_history(history, &start, &count, stored);

    if (strcmp(line, "print") == 0) {
      print_history(history, start, count);
    }
  }

  free(line);
  for (int i = 0; i < HISTORY_SIZE; ++i) {
    free(history[i]);
  }

  return 0;
}
