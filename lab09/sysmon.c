#include <stdio.h>      // fclose(), fgets(), fopen(), perror(), printf()
#include <stdlib.h>     // exit()
#include <string.h>     // strlen()
#include <sys/wait.h>   // waitpid()
#include <unistd.h>     // close(), fork(), pipe(), read(), sleep(), write()

#define BUFFER_SIZE 1024
#define INTERVAL 5

int main(void)
{
  // File Descriptors
  int cpu_pipe[2];
  int memMon_pipe[2];

  char cpu_buffer[BUFFER_SIZE];
  char memMon_buffer[BUFFER_SIZE];

  // CPU Pipe
  if (pipe(cpu_pipe) == -1)
  {
    perror("pipe");
    return -1;
  }

  // Memory Monitor (MM) Pipe
  if (pipe(memMon_pipe) == -1)
  {
    perror("pipe");
    return -1;
  }

  // Create Child Process.
  pid_t cpu_pid = fork();

  if (cpu_pid == 0)
  {
    // CPU MONITOR PROCESS

    // Close Read-End of CPU Pipe and Close MEM Pipe
    close(cpu_pipe[0]);
    close(memMon_pipe[0]);
    close(memMon_pipe[1]);

    // Buffer for Reading CPU Info.
    char buffer[BUFFER_SIZE];

    while(1)
    {
      FILE *file = fopen("/proc/loadavg", "r");

      if (file != NULL)
      {
        if (fgets(buffer, sizeof(buffer), file) != NULL)
        {
          // Send the CPU Info Through the Pipe.
          write(cpu_pipe[1], buffer, strlen(buffer) + 1);

          // Close the CPU file.
          fclose(file);
        }
      }

      sleep(INTERVAL);
    }

    // Close the Write-End of the CPU Pipe.
    close(cpu_pipe[1]);

    exit(0);
  }
  else
  {
    // PARENT PROCESS

    // Create Sibling Process
    pid_t memMon_pid = fork();

    if (memMon_pid == 0)
    {
      // MEM MONITOR PROCESS

      // Close CPU Pipe and Read-End of MEM Pipe
      close(cpu_pipe[0]);
      close(cpu_pipe[1]);
      close(memMon_pipe[0]);

      // Buffer for Reading MEM Info
      char buffer[BUFFER_SIZE];

      while(1)
      {
        FILE *file = fopen("/proc/meminfo", "r");

        if (file != NULL)
        {
          long MemTotal = -1;
          long MemAvailable = -1;

          while (fgets(buffer, sizeof(buffer), file) != NULL)
          {
            if (strncmp(buffer, "MemTotal:", 9) == 0)
            {
              sscanf(buffer, "MemTotal: %ld", &MemTotal);
            }
            else if (strncmp(buffer, "MemAvailable:", 13) == 0)
            {
              sscanf(buffer, "MemAvailable: %ld", &MemAvailable);
            }
          }

          if (MemTotal != -1 && MemAvailable != -1)
          {
            long MemUsed = MemTotal - MemAvailable;
            snprintf(buffer, sizeof(buffer), "Memory Used: %ld kB\n", MemUsed);

            // Send the Mem Info Through the Pipe.
            write(memMon_pipe[1], buffer, strlen(buffer) + 1);
          }

          // Close MEM file
          fclose(file);
        }

        sleep(INTERVAL);
      }
    }

    // Close the Write-End of the CPU Pipe.
    close(cpu_pipe[1]);

    // Close the Write-End of the MEM Pipe.
    close(memMon_pipe[1]);

    while(1)
    {
      // Read from the CPU Pipe.
      read(cpu_pipe[0], cpu_buffer, sizeof(cpu_buffer));

      // Read from the MEM Pipe
      read(memMon_pipe[0], memMon_buffer, sizeof(memMon_buffer));

      // Clear the Terminal and Move Cursor to the Top.
      printf("\033[2J\033[H");

      // Display CPU Info.
      printf("CPU Load Average: %s", cpu_buffer);

      // Display MEM Info
      printf("%s", memMon_buffer);
    }

    // Close the Read-End of the CPU Pipe.
    close(cpu_pipe[0]);

    // Wait for Child Process.
    waitpid(cpu_pid, NULL, 0);
  }

  return 0;
}
