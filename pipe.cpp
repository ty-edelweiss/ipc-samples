#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <cstdio>
#include <string>

int main(int argc, char const *argv[])
{
    int pipefd[2];
    // pipefd[0] : read end of pipe
    // pipefd[1] : write end of pipe

    char buf[100];

    if (pipe(pipefd) < 0) {
        std::perror("pipe() failed");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::perror("fork() failed");
        return -1;
    }

    if (pid == 0) {
        // child process
        close(pipefd[1]);

        read(pipefd[0], &buf, sizeof(buf));
        close(pipefd[0]);

        std::cout << buf << std::endl;
    } else {
        // parent process
        close(pipefd[0]);

        const std::string msg = "Hello World!!";
        write(pipefd[1], msg.c_str(), msg.size());
        close(pipefd[1]);

        wait(nullptr);
    }

    return 0;
}
