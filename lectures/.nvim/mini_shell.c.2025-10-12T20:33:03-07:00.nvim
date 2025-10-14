#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFER_SIZE 2014

int main() {
  while (1) {
    char cwd[BUFFER_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      write(STDOUT_FILENO, cwd, strlen(cwd));
      write(STDOUT_FILENO, "$ ", 2);
    } else {
      perror("getcwd error");
    }

    char buffer[BUFFER_SIZE];
    ssize_t n = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
    buffer[n] = '\0';

    if (n == -1) {
      perror("read");
      return 1;
    }
  }

  return 0;
}
