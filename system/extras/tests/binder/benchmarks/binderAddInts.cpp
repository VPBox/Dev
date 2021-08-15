/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 * Binder add integers benchmark (Using google-benchmark library)
 *
 */

#include <cerrno>
#include <grp.h>
#include <iostream>
#include <iomanip>
#include <libgen.h>
#include <time.h>
#include <unistd.h>

#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <benchmark/benchmark.h>

#include <utils/Log.h>
#include <testUtil.h>

using namespace android;
using namespace std;

const int unbound = -1; // Indicator for a thread not bound to a specific CPU

String16 serviceName("test.binderAddInts");

struct options {
    int serverCPU;
    int clientCPU;
    float        iterDelay; // End of iteration delay in seconds
} options = { // Set defaults
    unbound, // Server CPU
    unbound, // Client CPU
    0.0,    // End of iteration delay
};

class AddIntsService : public BBinder
{
  public:
    explicit AddIntsService(int cpu = unbound);
    virtual ~AddIntsService() {}

    enum command {
        ADD_INTS = 0x120,
    };

    virtual status_t onTransact(uint32_t code,
                                const Parcel& data, Parcel* reply,
                                uint32_t flags = 0);

  private:
    int cpu_;
};

// File scope function prototypes
static bool server(void);
static void BM_addInts(benchmark::State& state);
static void bindCPU(unsigned int cpu);
static ostream &operator<<(ostream &stream, const String16& str);
static ostream &operator<<(ostream &stream, const cpu_set_t& set);

static bool server(void)
{
    int rv;

    // Add the service
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    if ((rv = sm->addService(serviceName,
        new AddIntsService(options.serverCPU))) != 0) {
        cerr << "addService " << serviceName << " failed, rv: " << rv
            << " errno: " << errno << endl;
        return false;
    }

    // Start threads to handle server work
    proc->startThreadPool();
    return true;
}

static void BM_addInts(benchmark::State& state)
{
    int rv;
    sp<IServiceManager> sm = defaultServiceManager();

    // If needed bind to client CPU
    if (options.clientCPU != unbound) { bindCPU(options.clientCPU); }

    // Attach to service
    sp<IBinder> binder;
    for (int i = 0; i < 3; i++) {
        binder = sm->getService(serviceName);
        if (binder != 0) break;
        cout << serviceName << " not published, waiting..." << endl;
        usleep(500000); // 0.5 s
    }

    if (binder == 0) {
        cout << serviceName << " failed to publish, aborting" << endl;
        return;
    }

    unsigned int iter = 0;
    // Perform the IPC operations in the benchmark
    while (state.KeepRunning()) {
        Parcel send, reply;

        // Create parcel to be sent.  Will use the iteration cound
        // and the iteration count + 3 as the two integer values
        // to be sent.
        state.PauseTiming();
        int val1 = iter;
        int val2 = iter + 3;
        int expected = val1 + val2;  // Expect to get the sum back
        send.writeInt32(val1);
        send.writeInt32(val2);
        state.ResumeTiming();
        // Send the parcel, while timing how long it takes for
        // the answer to return.
        if ((rv = binder->transact(AddIntsService::ADD_INTS,
            send, &reply)) != 0) {
            cerr << "binder->transact failed, rv: " << rv
                << " errno: " << errno << endl;
            exit(10);
        }

        state.PauseTiming();
        int result = reply.readInt32();
        if (result != (int) (iter + iter + 3)) {
            cerr << "Unexpected result for iteration " << iter << endl;
            cerr << "  result: " << result << endl;
            cerr << "expected: " << expected << endl;
        }

        if (options.iterDelay > 0.0) { testDelaySpin(options.iterDelay); }
        state.ResumeTiming();
    }
}
BENCHMARK(BM_addInts);


AddIntsService::AddIntsService(int cpu): cpu_(cpu) {
    if (cpu != unbound) { bindCPU(cpu); }
}

// Server function that handles parcels received from the client
status_t AddIntsService::onTransact(uint32_t code, const Parcel &data,
                                    Parcel* reply, uint32_t /* flags */) {
    int val1, val2;
    status_t rv(0);
    int cpu;

    // If server bound to a particular CPU, check that
    // were executing on that CPU.
    if (cpu_ != unbound) {
        cpu = sched_getcpu();
        if (cpu != cpu_) {
            cerr << "server onTransact on CPU " << cpu << " expected CPU "
                  << cpu_ << endl;
            exit(20);
        }
    }

    // Perform the requested operation
    switch (code) {
    case ADD_INTS:
        val1 = data.readInt32();
        val2 = data.readInt32();
        reply->writeInt32(val1 + val2);
        break;

    default:
      cerr << "server onTransact unknown code, code: " << code << endl;
      exit(21);
    }

    return rv;
}

static void bindCPU(unsigned int cpu)
{
    int rv;
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    rv = sched_setaffinity(0, sizeof(cpuset), &cpuset);

    if (rv != 0) {
        cerr << "bindCPU failed, rv: " << rv << " errno: " << errno << endl;
        perror(NULL);
        exit(30);
    }
}

static ostream &operator<<(ostream &stream, const String16& str)
{
    for (unsigned int n1 = 0; n1 < str.size(); n1++) {
        if ((str[n1] > 0x20) && (str[n1] < 0x80)) {
            stream << (char) str[n1];
        } else {
            stream << '~';
        }
    }

    return stream;
}

static ostream &operator<<(ostream &stream, const cpu_set_t& set)
{
    for (unsigned int n1 = 0; n1 < CPU_SETSIZE; n1++) {
        if (CPU_ISSET(n1, &set)) {
            if (n1 != 0) { stream << ' '; }
            stream << n1;
        }
    }

    return stream;
}

int main(int argc, char *argv[])
{
    int rv;
    ::benchmark::Initialize(&argc, argv);
    // Determine CPUs available for use.
    // This testcase limits its self to using CPUs that were
    // available at the start of the benchmark.
    cpu_set_t availCPUs;
    if ((rv = sched_getaffinity(0, sizeof(availCPUs), &availCPUs)) != 0) {
        cerr << "sched_getaffinity failure, rv: " << rv
            << " errno: " << errno << endl;
        exit(1);
    }

    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "s:c:d:?")) != -1) {
        char *chptr; // character pointer for command-line parsing

        switch (opt) {
        case 'c': // client CPU
        case 's': { // server CPU
            // Parse the CPU number
            int cpu = strtoul(optarg, &chptr, 10);
            if (*chptr != '\0') {
                cerr << "Invalid cpu specified for -" << (char) opt
                    << " option of: " << optarg << endl;
                exit(2);
            }

            // Is the CPU available?
            if (!CPU_ISSET(cpu, &availCPUs)) {
                cerr << "CPU " << optarg << " not currently available" << endl;
                cerr << "  Available CPUs: " << availCPUs << endl;
                exit(3);
            }

            // Record the choice
            *((opt == 'c') ? &options.clientCPU : &options.serverCPU) = cpu;
            break;
        }

        case 'd': // delay between each iteration
            options.iterDelay = strtod(optarg, &chptr);
            if ((*chptr != '\0') || (options.iterDelay < 0.0)) {
                cerr << "Invalid delay specified of: " << optarg << endl;
                exit(6);
            }
            break;

        case '?':
        default:
            cerr << basename(argv[0]) << " [options]" << endl;
            cerr << "  options:" << endl;
            cerr << "    -s cpu - server CPU number" << endl;
            cerr << "    -c cpu - client CPU number" << endl;
            cerr << "    -d time - delay after operation in seconds" << endl;
            exit(((optopt == 0) || (optopt == '?')) ? 0 : 7);
        }
    }

    fflush(stdout);
    switch (pid_t pid = fork()) {
    case 0: // Child
        ::benchmark::RunSpecifiedBenchmarks();
        return 0;

    default: // Parent
        if (!server()) { break; }

        // Wait for all children to end
        do {
            int stat;
            rv = wait(&stat);
            if ((rv == -1) && (errno == ECHILD)) { break; }
            if (rv == -1) {
                cerr << "wait failed, rv: " << rv << " errno: "
                    << errno << endl;
                perror(NULL);
                exit(8);
            }
        } while (1);
        return 0;

    case -1: // Error
        exit(9);
    }
}
