#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // pipe
  int fd[2];
  // fd[0] - read
  // fd[1] - write
  if (pipe(fd) == -1) {
    fprintf(stderr, "An error ocurred with opening the pipe\n");
    return EXIT_FAILURE;
  }
  pid_t id = fork();

  if (id == 0) {
    close(fd[0]);
    int x;
    printf("Input a number: ");
    scanf("%d", &x);

    write(fd[1], &x, sizeof(int));
    close(fd[1]);
  } else {
    close(fd[1]);
    int y;
    read(fd[0], &y, sizeof(int));
    close(fd[0]);
    printf("Got from child process %d\n", y);
  }

  return EXIT_SUCCESS;
}