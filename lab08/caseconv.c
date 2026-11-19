#include <stdio.h> 
#include <string.h> // strcmp
#include <fcntl.h> // open(), O_RDONLY
#include <sys/stat.h> // fstat()
#include <unistd.h>
#include <sys/mman.h> // mmap()
#include <ctype.h> // toupper(), tolower()
#include <sys/types.h> // off_t

// argument count and argument array
int main(int argc, char *argv[]) {

  int returnVal;
  int converted;
  int exitNum = 0;
  struct stat fileBytes;
  char *mapping;

  if (argc != 3) { // Check if there are 3 arguments present
    fprintf(stderr, "Usage: %s <upper|lower> <filename>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "upper") && strcmp(argv[1], "lower")) { // Check if 2nd argument is upper or lower
    fprintf(stderr, "Usage: <program_name> <upper|lower> <filename>\n");
    return 1;
  }

  returnVal = open(argv[2], O_RDONLY); // Assign returnVal with file descriptor

  if (returnVal == -1) { // check for open fail
    perror("open");
    return 1;
  }

  if (fstat(returnVal, &fileBytes) == -1) { // check for fstat fail, if successful then fileBytes.st_size is filled with the amount of bytes in our file
    perror("fstat");

    if (close(returnVal) == -1) { // check for close fail
      perror("close");
    }

    return 1;
  }

  if (fileBytes.st_size == 0) {
    
    if (close(returnVal) == -1) { // close fail check
      perror("close");
      return 1;
    }

    return 0;
  }

  mapping = mmap(NULL, fileBytes.st_size, PROT_READ, MAP_PRIVATE, returnVal, 0); // read-only private map

  if (mapping == MAP_FAILED) { // map fail check
    perror("mmap");

    if (close(returnVal) == -1) { // close fail check
      perror("close");
    }
    return 1;
  }

  for (off_t i = 0; i < fileBytes.st_size; i++) { // loop that runs through each byte of the file and converts each character into uppercase or lowercase
    unsigned char c = (unsigned char)mapping[i];

    if (strcmp(argv[1], "upper") == 0) {
      converted = toupper(c);
    }
    else {
      converted = tolower(c);
    }

    if (putchar(converted) == EOF) { // putchar fail check
      perror("putchar");
      exitNum = 1;
      break;
    }

  }

  if (fflush(stdout) == EOF) { // fflush fail check
    perror("fflush");
    exitNum = 1;
  }

  if (munmap(mapping, fileBytes.st_size) == -1) { // munmap fail check
    perror("munmap");
    exitNum = 1;
  }

  if (close(returnVal) == -1) { // close fail check
    perror("close");
    exitNum = 1;
  }

  return exitNum;
}
