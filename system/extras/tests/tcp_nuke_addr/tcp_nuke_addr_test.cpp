#include <arpa/inet.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <mutex>
#include <thread>

#include "utils/RWLock.h"

// Defined only in ifc_utils.c, in the kernel, and in the NDK.
#ifndef SIOCKILLADDR
#define SIOCKILLADDR 0x8939
#endif

#ifndef TCP_LINGER2
#define TCP_LINGER2 8
#endif

#define KILL_INTERVAL_MS 10
#define CONNECT_THREADS 1

#define PERROR_EXIT(msg) { do { perror((msg)); exit(1); } while (0); };


// Ensures that sockets don't stay in TIME_WAIT state.
void setSoLinger(int s) {
    const struct linger l = {
        0,  // off
        0,  // 0 seconds
    };
    if (setsockopt(s, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) == -1) {
        PERROR_EXIT("SO_LINGER");
    }
    const int nolinger = -1;
    if (setsockopt(s, SOL_TCP, TCP_LINGER2, &nolinger, sizeof(nolinger)) == -1) {
        PERROR_EXIT("TCP_LINGER2");
    }
}


// Binds to a random port on a random loopback address. We don't just use 127.0.0.1 because we don't
// want this test to kill unrelated connections on loopback.
int bindRandomAddr() {
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    while (sin.sin_addr.s_addr == htonl(INADDR_LOOPBACK)) {
        arc4random_buf(
             ((uint8_t *) &sin.sin_addr.s_addr) + 1,
             sizeof(sin.sin_addr.s_addr) - 1);
    }

    int listensock;
    if ((listensock = socket(AF_INET, SOCK_STREAM, 0)) == -1) PERROR_EXIT("listensock");
    if (bind(listensock, (sockaddr *) &sin, sizeof(sin)) == -1) PERROR_EXIT("bind");
    if (listen(listensock, 10) == -1) PERROR_EXIT("listen");

    return listensock;
}


// Thread that calls SIOCKILLADDR in a loop.
void killSockets(sockaddr_in listenaddr, int intervalMs, android::RWLock *lock) {
    ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    listenaddr.sin_port = 0;
    strncpy(ifr.ifr_name, "lo", strlen("lo"));
    memcpy(&ifr.ifr_addr, &listenaddr, sizeof(listenaddr));

    int ioctlsock = socket(AF_INET, SOCK_DGRAM, 0);
    if (ioctlsock == -1) PERROR_EXIT("ioctlsock");
    while(true) {
        lock->writeLock();
        if (ioctl(ioctlsock, SIOCKILLADDR, &ifr) != 0) {
            PERROR_EXIT("SIOCKILLADDR failed, did you run 32-bit userspace on a 64-bit kernel?");
        }
        lock->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}


// Thread that calls connect() in a loop.
void connectLoop(sockaddr_in listenaddr, int listensock,
        android::RWLock *lock, std::atomic<unsigned int> *attempts) {
    while(true) {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        setSoLinger(s);

        // Don't call SIOCKILLADDR while connect() is running, or we'll end up with lots of
        // connections in state FIN_WAITx or TIME_WAIT, which will then slow down future
        // due to SYN retransmits.
        lock->readLock();
        if (connect(s, (sockaddr *) &listenaddr, sizeof(listenaddr)) == -1) PERROR_EXIT("connect");
        lock->unlock();

        send(s, "foo", 3, 0);
        int acceptedsock = accept(listensock, NULL, 0);
        if (close(acceptedsock) == -1) PERROR_EXIT("close");
        if (close(s) == -1) PERROR_EXIT("close");

        attempts->fetch_add(1);
    }
}


// Thread that prints progress every second.
void progressThread(std::atomic<unsigned int> *attempts) {
    uint32_t elapsed = 0;
    uint32_t total, previous = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        elapsed++;
        total = attempts->load();
        printf("%ds: %u cps, total %u\n", elapsed, total-previous, total);
        fflush(stdout);
        previous = total;
    }
}


int main() {
    int listensock = bindRandomAddr();
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(listensock, (sockaddr *) &sin, &len) == -1) PERROR_EXIT("getsockname");

    printf("Using address %s:%d\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

    android::RWLock lock;
    std::atomic<unsigned int> attempts;
    attempts.store(0);

    std::thread t0(killSockets, sin, KILL_INTERVAL_MS, &lock);
    for (size_t i = 0; i < CONNECT_THREADS; i++) {
        std::thread(connectLoop, sin, listensock, &lock, &attempts).detach();
    }

    progressThread(&attempts);
    return 0;
}
