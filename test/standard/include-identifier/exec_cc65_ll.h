// vim: set et ts=2 sw=2:

#ifndef _WIN_

#include <unistd.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void execute_cc65(std::string executable, const std::vector<std::string> & arguments)
{
  pid_t pid = 0;
  int status;

  pid = fork();
  if (pid == 0) {
    // child
    char * arguments_c[arguments.size() + 2] = { const_cast<char*>(executable.c_str()), NULL };

    int i = 1;

    for (auto & it : arguments) {
      arguments_c[i++] = const_cast<char*>(it.c_str());
    }

    arguments_c[i] = NULL;

    // redirect output
    int fd = ::open(names_and_parameters::filename_output.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);

    execv(executable.c_str(), arguments_c);
  }
  if (pid > 0) {
    // parent
    pid = wait(&status);

    if (WIFEXITED(status)) {
      if (WEXITSTATUS(status)) {
        printf("The process ended with exit(%d).\n", WEXITSTATUS(status));
        exit(WEXITSTATUS(status));
      }
    }
    if (WIFSIGNALED(status)) {
      printf("The process ended with kill -%d.\n", WTERMSIG(status));
      exit(1);
    }
  }
  if (pid < 0) {
    perror("failure in fork(): ");
  }
}
#else

#include <windows.h>

static void join(std::stringstream & stream, std::vector<std::string> & arguments)
{
  for (auto & it : arguments) {
    stream << " \"" << it << '"';
  }
}

static void execute_cc65(std::string executable, std::vector<std::string> arguments)
{
  std::stringstream commandline_stream;

  commandline_stream << '"' << executable << '"';

  join(commandline_stream, arguments);

  std::cerr << "Using commandline: " << commandline_stream.str() << std::endl;
}
#endif

