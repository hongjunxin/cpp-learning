#include <sys/types.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <errno.h>
#include "worker.h"

Worker::State Worker::run(void)
{
    auto on = running.test_and_set();
    if (on) {
        return State::kOnRunning;
    }

    if (openSocketPair() == -1) {
        return State::kInitFailed;
    }

    auto pid = fork();
    if (pid == -1) {
        closeSocketPair();
        return State::kForkFailed;
    } else if (pid > 0) {
        return State::kRunSuccess;
    }
    
    prctl(PR_SET_NAME, "worker");
    if (m_exit_with_parent) {
        prctl(PR_SET_PDEATHSIG, SIGTERM, 0, 0, 0);
    }

    // auto fd = open("/dev/null", O_RDWR);
    // dup2(fd, STDIN_FILENO);
    // dup2(fd, STDOUT_FILENO);
    // dup2(fd, STDERR_FILENO);

    InProcessMsg inpc_msg;
    for (;;) {
        if (readFromMainProcess(&inpc_msg) != 0) {
            continue;
        }
        switch (inpc_msg.type) {
        case InProcessMsg::Type::kShell:
            std::cout << "worker recv cmd: " << std::string(inpc_msg.value) << std::endl;
            handleShellCmd(inpc_msg.value);
            break;
        default:
            break;
        }
    }

    // reach here abnormally
    return State::kExit;
}

void Worker::handleShellCmd(const char* cmd)
{
    const char* ret;
    int status = system(cmd);
    if (WIFEXITED(status)) {
        ret = strerror(WEXITSTATUS(status));
    } else {
        ret = "system() call terminated abnormally";
    }

    InProcessMsg inpc_msg;
    inpc_msg.type = InProcessMsg::Type::kShellAck;
    memset(inpc_msg.value, '\0', sizeof(inpc_msg.value));
    memcpy(inpc_msg.value, ret, strlen(ret));
    inpc_msg.value[sizeof(inpc_msg.value) - 1] = '\0';
    inpc_msg.size = strlen(inpc_msg.value) + 1;

    writeToMainProcess(&inpc_msg);
}

std::string Worker::execShell(std::string cmd)
{
    InProcessMsg inpc_msg;
    inpc_msg.type = InProcessMsg::Type::kShell;
    
    if (cmd.length() > sizeof(inpc_msg.value) - 1) {
        std::ostringstream oss;
        oss << "cmd length to long, limit " << sizeof(inpc_msg.value) - 1 << " bytes";
        return oss.str();
    }
    memcpy(inpc_msg.value, cmd.c_str(), cmd.length());
    inpc_msg.value[cmd.length()] = '\0';
    inpc_msg.size = cmd.length() + 1;
    if (writeToWorkerProcess(&inpc_msg) != 0) {
        return "failed";
    }
    memset(&inpc_msg, 0, sizeof(inpc_msg));
    if (readFromWorkerProcess(&inpc_msg) != 0) {
        return "failed";
    }
    if (inpc_msg.type != InProcessMsg::Type::kShellAck) {
        std::cout << "type not kShellAck" << std::endl;
        return "failed";
    }
    return inpc_msg.value;
}

int Worker::openSocketPair(void)
{
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_socket_pair) == -1) {
        return -1;
    }
    if (fcntl(m_socket_pair[0], F_SETFD, FD_CLOEXEC) == -1) {
        closeSocketPair();
        return -1;
    }
    if (fcntl(m_socket_pair[1], F_SETFD, FD_CLOEXEC) == -1) {
        closeSocketPair();
        return -1;
    }
    return 0;
}

void Worker::closeSocketPair(void)
{
    if (m_socket_pair[0] != -1) {
        close(m_socket_pair[0]);
        m_socket_pair[0] = -1;
    }
    if (m_socket_pair[1] != -1) {
        close(m_socket_pair[1]);
        m_socket_pair[1] = -1;
    }
}

Worker::~Worker()
{
    closeSocketPair();
}

int Worker::writeToMainProcess(InProcessMsg* inpc_msg)
{
    return writeSocketPair(inpc_msg, m_socket_pair[1]);
}

int Worker::readFromWorkerProcess(InProcessMsg* inpc_msg)
{
    return readSocketPair(inpc_msg, m_socket_pair[0]);
}

int Worker::writeToWorkerProcess(InProcessMsg *inpc_msg)
{
    return writeSocketPair(inpc_msg, m_socket_pair[0]);
}

int Worker::readFromMainProcess(InProcessMsg* inpc_msg)
{
    return readSocketPair(inpc_msg, m_socket_pair[1]);
}

int Worker::readSocketPair(InProcessMsg* inpc_msg, int socket_pair_fd)
{
    struct iovec        iov[1];
    struct msghdr       msg;

    union {
        struct cmsghdr  cm;
        char            space[CMSG_SPACE(sizeof(int))];
    } cmsg;

    iov[0].iov_base = (char *) inpc_msg;
    iov[0].iov_len = sizeof(InProcessMsg);

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    msg.msg_control = (caddr_t) &cmsg;
    msg.msg_controllen = sizeof(cmsg);

    auto n = recvmsg(socket_pair_fd, &msg, 0);
    if (n == -1) {
        std::cout << "read socket_pair " << socket_pair_fd 
            << " failed, errno=" << errno << std::endl;
        return -1;
    }
    if ((size_t) n < sizeof(InProcessMsg)) {
        std::cout << "read socket_pair " << socket_pair_fd
            << " failed, returned not enough data: " << n 
            << std::endl;
        return -1;
    }

    // needn't inpc_msg->fd so ignore it

    if (msg.msg_flags & (MSG_TRUNC|MSG_CTRUNC)) {
        std::cout << "read socket_pair " << socket_pair_fd
            <<", data truncated" << std::endl;
    }

    return 0;
}

int Worker::writeSocketPair(InProcessMsg* inpc_msg, int socket_pair_fd)
{
    struct iovec        iov[1];
    struct msghdr       msg;

    union {
        struct cmsghdr  cm;
        char            space[CMSG_SPACE(sizeof(int))];
    } cmsg;

    if (inpc_msg->fd == -1) {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;

    } else {
        msg.msg_control = (caddr_t) &cmsg;
        msg.msg_controllen = sizeof(cmsg);

        memset(&cmsg, 0, sizeof(cmsg));

        cmsg.cm.cmsg_len = CMSG_LEN(sizeof(int));
        cmsg.cm.cmsg_level = SOL_SOCKET;
        cmsg.cm.cmsg_type = SCM_RIGHTS;
        memcpy(CMSG_DATA(&cmsg.cm), &inpc_msg->fd, sizeof(int));
    }

    iov[0].iov_base = (char *) inpc_msg;
    iov[0].iov_len = sizeof(InProcessMsg);

    msg.msg_flags = 0;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    auto n = sendmsg(socket_pair_fd, &msg, 0);
    if (n == -1) {
        std::cout << "write socket_pair " << socket_pair_fd 
            << " failed, errno=" << errno << std::endl;
        return -1;
    }

    return 0;
}
