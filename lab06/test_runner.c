#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h> // FILE, fopen, fgets, printf, perror, fclose
#include <string.h> // strtok, strcspn

int main(void) {
  int status;
  char buffer[256];
  char *args[256];
  int exeTests = 0; // # of executed tests
  int passTests = 0; // # of passed tests
  int failTests = 0; // # of failed tests

  // Opens tests.txt in read only mode
  FILE *file = fopen("tests.txt", "r");


  // Check if the file opened properly
  if (file == NULL) {
    perror("Error Thrown [Couldn't open tests.txt]");
    return 1;
  }

  // Inserts a string from file into buffer
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    int i = 0;
    int j = 0;
 
    // Replaces the newline character in buffer with the end of line character (Removes trailing newline)
    buffer[strcspn(buffer, "\n")] = '\0';

    // checks for white space in current line of buffer
    while (isspace((unsigned char)buffer[i])) {
      i++;
    }

    // Skips lines that start with #
    if (buffer[i] == '#') {
      continue;
    }

    // Skips blank lines
    if (buffer[i] == '\0') {
      continue;
    }
    
    printf("Test command: %s\n", buffer); // Prints the current command being tested

    // creates a char pointer that points towards elements in the buffer minus spaces, escape sequences, and newlines
    char *token = strtok(buffer, " \t\n");

    // Assigns argument array with elements from token to create an array suited for execvp()
    while (token != NULL) {
      args[j] = token;
      j++;
      token = strtok(NULL, " \t\n");
    }
    args[j] = NULL;
    
    printf("Starting test...\n"); // Prints right before the test starts to denote such
    fflush(stdout); // Forces any standard output to be written before child program is initiated

    ++exeTests; // Increments executed tests by one for the following if-else statement

    pid_t pid = fork();

    // Runs tests if in the child process only
    if (pid == 0) {
      execvp(args[0], args);
      perror("execvp failed");
      _exit(127);
    }
    else if (pid == -1) { // If Fork failed
      perror("Fork failed");
      printf("FAIL\n");
      ++failTests;
    }
    else if (pid > 0) { // Waits for child to terminate and passes status's memory address
      if (wait(&status) == -1) {
        perror("Wait failed");
        printf("FAIL\n");
        ++failTests;
        continue;
      }
      // Checks if the child exited normally or not and if a test was passed or failed
      if (WIFEXITED(status)) {
        printf("Exited Normally\n");
        printf("Exit code: %d\n", WEXITSTATUS(status));
        if (WEXITSTATUS(status) == 0) {
          printf("PASS\n");
          ++passTests;
        }
        else {
        printf("FAIL\n");
          ++failTests;
        }
      }
      else {
        printf("FAIL\n");
        ++failTests;
      }
    }
  }

  printf("================================\n"
         " TEST SUMMARY\n"
         "================================\n"
         "Tests executed:  %d\n"
         "Tests passed:    %d\n"
         "Tests failed:    %d\n"
         "================================\n", exeTests, passTests, failTests);

  // Closes the file
  fclose(file);

  return 0;
}
