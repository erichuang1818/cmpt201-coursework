#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char buffer[1024];

  while (1) {
    printf("Enter programs to run.\n");
    printf("> ");
    fflush(stdout);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      break;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork failed");
      exit(1);
    } else if (pid == 0) {

      execl(buffer, buffer, (char *)NULL);

      perror("Exec failure");
      exit(1);
    } else {

      int status;
      waitpid(pid, &status, 0);
    }
  }
  return 0;
}
