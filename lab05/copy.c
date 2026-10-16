#include <fcntl.h> // open()
#include <unistd.h> // read, write, close
#include <stdlib.h> // printf
#include <stdio.h>

// Function Prototype
int copy(const char *source_file, const char *target_file);

int main(int argc, char *argv[]) {

  // Check Command-Line Arguments
  if(argc != 3) {
    printf("Usage: %s source target\n", argv[0]);
    return 1;
  }

  return copy(argv[1], argv[2]);
}

int copy(const char *source_file, const char *target_file) {

  char buffer[32];
  ssize_t bytes_read;
  ssize_t bytes_written;

  // Open Source File for Reading
  int fd_source = open(source_file, O_RDONLY);

  if(fd_source == -1) {
    perror("open source");
    close(fd_source);
    return 1;
  }

  // Open Target File for Writing
  int fd_target = open(target_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

  if(fd_target == -1) {
    perror("open target");
    close(fd_target);
    return 1;
  }

  // Read into Buffer from Source and Write from Buffer to Target
  while((bytes_read = read(fd_source, buffer, sizeof(buffer))) > 0) {
    bytes_written = write(fd_target, buffer, bytes_read);

    if(bytes_written == -1) {
      perror("write");
      close(fd_source);
      close(fd_target);
      return 1;
    }

    printf("%ld bytes_written...\n", bytes_written);
  }

  // Close the File Descriptors
  if(close(fd_source) == -1) {
    perror("close source");
    return 1;
  }

  if(close(fd_target) == -1) {
    perror("close target");
    return 1;
  }

  return 0;
}
