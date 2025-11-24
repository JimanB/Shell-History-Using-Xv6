#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  int fd, n;

  // Open the history file for reading only.
  fd = open("sh_history", O_RDONLY);
  if(fd < 0){
    fprintf(2, "history: cannot open sh_history\n");
    exit(1);
  }

  // Read from sh_history and write to the console until the file ends.
  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      fprintf(2, "history: write error\n");
      close(fd);
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
