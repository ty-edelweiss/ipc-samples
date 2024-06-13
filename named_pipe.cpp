#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <string>

int main(int argc, char const *argv[])
{
    std::fstream fs;

    std::string buf;

    std::string path = "/tmp/myfifo";

    if (!std::filesystem::exists(path)) {
        if (mkfifo(path.c_str(), 0666) < 0) {
            std::perror("mkfifo() failed");
            return -1;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::perror("fork() failed");
        return -1;
    }

    if (pid == 0) {
        // child process
        fs.open(path, std::ios_base::in);
        std::getline(fs, buf);
        std::cout << buf << std::endl;
        fs.close();
    } else {
        // parent process
        fs.open(path, std::ios_base::out);
        fs << "Hello World!!";
        fs.close();
        wait(nullptr);
    }

    return 0;
}
