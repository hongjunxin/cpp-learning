#ifndef WORKER_H
#define WORKER_H

#include <atomic>
#include <string>

struct InProcessMsg
{
    enum class Type : int {
        kShell,
        kShellAck,
    };
    int fd = -1;      // so far we needn't fd
    Type type;
    uint size;        // size of value include '\0'
    char value[256];  // max length sizeof(value)
};

class Worker {
public:
    Worker(bool exit_with_parent=true) : m_exit_with_parent(exit_with_parent) {}
    ~Worker();
    Worker(Worker&& o) = delete;
    Worker(Worker& o) = delete;
    Worker& operator=(Worker& o) = delete;
    Worker& operator=(Worker&& o) = delete;

    enum class State : int {
        kOnRunning = 1,
        kRunSuccess,
        kForkFailed,
        kInitFailed,
        kExit,
    };

    State run(void);
    std::string execShell(std::string cmd);

private:
    std::atomic_flag running{false};
    bool m_exit_with_parent; // whether exit if parent exited
    // main 0 --> 1 worker
    // main 0 <-- 1 worker
    int m_socket_pair[2] = {-1, -1};

    int openSocketPair(void);
    void closeSocketPair(void);
    int writeToWorkerProcess(InProcessMsg* msg);
    int writeToMainProcess(InProcessMsg* msg);
    int readFromWorkerProcess(InProcessMsg* msg);
    int readFromMainProcess(InProcessMsg* msg);

    int writeSocketPair(InProcessMsg* msg, int socket_pair_fd);
    int readSocketPair(InProcessMsg* msg, int socket_pair_fd);

    void handleShellCmd(const char* cmd);
};

#endif