#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <vector>
#include <tuple>

#include <unistd.h>
#include <sys/wait.h>

using namespace std;
using namespace android;

enum BinderWorkerServiceCode {
    BINDER_NOP = IBinder::FIRST_CALL_TRANSACTION,
};

#define ASSERT_TRUE(cond) \
do { \
    if (!(cond)) {\
       cerr << __func__ << ":" << __LINE__ << " condition:" << #cond << " failed\n" << endl; \
       exit(EXIT_FAILURE); \
    } \
} while (0)

class BinderWorkerService : public BBinder
{
public:
    BinderWorkerService() {}
    ~BinderWorkerService() {}
    virtual status_t onTransact(uint32_t code,
                                const Parcel& data, Parcel* reply,
                                uint32_t flags = 0) {
        (void)flags;
        (void)data;
        (void)reply;
        switch (code) {
        case BINDER_NOP:
            return NO_ERROR;
        default:
            return UNKNOWN_TRANSACTION;
        };
    }
};

class Pipe {
    int m_readFd;
    int m_writeFd;
    Pipe(int readFd, int writeFd) : m_readFd{readFd}, m_writeFd{writeFd} {}
    Pipe(const Pipe &) = delete;
    Pipe& operator=(const Pipe &) = delete;
    Pipe& operator=(const Pipe &&) = delete;
public:
    Pipe(Pipe&& rval) noexcept {
        m_readFd = rval.m_readFd;
        m_writeFd = rval.m_writeFd;
        rval.m_readFd = 0;
        rval.m_writeFd = 0;
    }
    ~Pipe() {
        if (m_readFd)
            close(m_readFd);
        if (m_writeFd)
            close(m_writeFd);
    }
    void signal() {
        bool val = true;
        int error = write(m_writeFd, &val, sizeof(val));
        ASSERT_TRUE(error >= 0);
    };
    void wait() {
        bool val = false;
        int error = read(m_readFd, &val, sizeof(val));
        ASSERT_TRUE(error >= 0);
    }
    template <typename T> void send(const T& v) {
        int error = write(m_writeFd, &v, sizeof(T));
        ASSERT_TRUE(error >= 0);
    }
    template <typename T> void recv(T& v) {
        int error = read(m_readFd, &v, sizeof(T));
        ASSERT_TRUE(error >= 0);
    }
    static tuple<Pipe, Pipe> createPipePair() {
        int a[2];
        int b[2];

        int error1 = pipe(a);
        int error2 = pipe(b);
        ASSERT_TRUE(error1 >= 0);
        ASSERT_TRUE(error2 >= 0);

        return make_tuple(Pipe(a[0], b[1]), Pipe(b[0], a[1]));
    }
};

static const uint32_t num_buckets = 128;
static uint64_t max_time_bucket = 50ull * 1000000;
static uint64_t time_per_bucket = max_time_bucket / num_buckets;

struct ProcResults {
    uint64_t m_worst = 0;
    uint32_t m_buckets[num_buckets] = {0};
    uint64_t m_transactions = 0;
    uint64_t m_long_transactions = 0;
    uint64_t m_total_time = 0;
    uint64_t m_best = max_time_bucket;

    void add_time(uint64_t time) {
        if (time > max_time_bucket) {
            m_long_transactions++;
        }
        m_buckets[min(time, max_time_bucket-1) / time_per_bucket] += 1;
        m_best = min(time, m_best);
        m_worst = max(time, m_worst);
        m_transactions += 1;
        m_total_time += time;
    }
    static ProcResults combine(const ProcResults& a, const ProcResults& b) {
        ProcResults ret;
        for (int i = 0; i < num_buckets; i++) {
            ret.m_buckets[i] = a.m_buckets[i] + b.m_buckets[i];
        }
        ret.m_worst = max(a.m_worst, b.m_worst);
        ret.m_best = min(a.m_best, b.m_best);
        ret.m_transactions = a.m_transactions + b.m_transactions;
        ret.m_long_transactions = a.m_long_transactions + b.m_long_transactions;
        ret.m_total_time = a.m_total_time + b.m_total_time;
        return ret;
    }
    void dump() {
        if (m_long_transactions > 0) {
            cout << (double)m_long_transactions / m_transactions << "% of transactions took longer "
                "than estimated max latency. Consider setting -m to be higher than "
                 << m_worst / 1000 << " microseconds" << endl;
        }

        double best = (double)m_best / 1.0E6;
        double worst = (double)m_worst / 1.0E6;
        double average = (double)m_total_time / m_transactions / 1.0E6;
        cout << "average:" << average << "ms worst:" << worst << "ms best:" << best << "ms" << endl;

        uint64_t cur_total = 0;
        float time_per_bucket_ms = time_per_bucket / 1.0E6;
        for (int i = 0; i < num_buckets; i++) {
            float cur_time = time_per_bucket_ms * i + 0.5f * time_per_bucket_ms;
            if ((cur_total < 0.5f * m_transactions) && (cur_total + m_buckets[i] >= 0.5f * m_transactions)) {
                cout << "50%: " << cur_time << " ";
            }
            if ((cur_total < 0.9f * m_transactions) && (cur_total + m_buckets[i] >= 0.9f * m_transactions)) {
                cout << "90%: " << cur_time << " ";
            }
            if ((cur_total < 0.95f * m_transactions) && (cur_total + m_buckets[i] >= 0.95f * m_transactions)) {
                cout << "95%: " << cur_time << " ";
            }
            if ((cur_total < 0.99f * m_transactions) && (cur_total + m_buckets[i] >= 0.99f * m_transactions)) {
                cout << "99%: " << cur_time << " ";
            }
            cur_total += m_buckets[i];
        }
        cout << endl;
    }
};

String16 generateServiceName(int num)
{
    char num_str[32];
    snprintf(num_str, sizeof(num_str), "%d", num);
    String16 serviceName = String16("binderWorker") + String16(num_str);
    return serviceName;
}

void worker_fx(int num,
               int worker_count,
               int iterations,
               int payload_size,
               bool cs_pair,
               Pipe p)
{
    // Create BinderWorkerService and for go.
    ProcessState::self()->startThreadPool();
    sp<IServiceManager> serviceMgr = defaultServiceManager();
    sp<BinderWorkerService> service = new BinderWorkerService;
    serviceMgr->addService(generateServiceName(num), service);

    srand(num);
    p.signal();
    p.wait();

    // If client/server pairs, then half the workers are
    // servers and half are clients
    int server_count = cs_pair ? worker_count / 2 : worker_count;

    // Get references to other binder services.
    cout << "Created BinderWorker" << num << endl;
    (void)worker_count;
    vector<sp<IBinder> > workers;
    for (int i = 0; i < server_count; i++) {
        if (num == i)
            continue;
        workers.push_back(serviceMgr->getService(generateServiceName(i)));
    }

    // Run the benchmark if client
    ProcResults results;
    chrono::time_point<chrono::high_resolution_clock> start, end;
    for (int i = 0; (!cs_pair || num >= server_count) && i < iterations; i++) {
        Parcel data, reply;
        int target = cs_pair ? num % server_count : rand() % workers.size();
        int sz = payload_size;

        while (sz >= sizeof(uint32_t)) {
            data.writeInt32(0);
            sz -= sizeof(uint32_t);
        }
        start = chrono::high_resolution_clock::now();
        status_t ret = workers[target]->transact(BINDER_NOP, data, &reply);
        end = chrono::high_resolution_clock::now();

        uint64_t cur_time = uint64_t(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
        results.add_time(cur_time);

        if (ret != NO_ERROR) {
           cout << "thread " << num << " failed " << ret << "i : " << i << endl;
           exit(EXIT_FAILURE);
        }
    }

    // Signal completion to master and wait.
    p.signal();
    p.wait();

    // Send results to master and wait for go to exit.
    p.send(results);
    p.wait();

    exit(EXIT_SUCCESS);
}

Pipe make_worker(int num, int iterations, int worker_count, int payload_size, bool cs_pair)
{
    auto pipe_pair = Pipe::createPipePair();
    pid_t pid = fork();
    if (pid) {
        /* parent */
        return move(get<0>(pipe_pair));
    } else {
        /* child */
        worker_fx(num, worker_count, iterations, payload_size, cs_pair, move(get<1>(pipe_pair)));
        /* never get here */
        return move(get<0>(pipe_pair));
    }

}

void wait_all(vector<Pipe>& v)
{
    for (int i = 0; i < v.size(); i++) {
        v[i].wait();
    }
}

void signal_all(vector<Pipe>& v)
{
    for (int i = 0; i < v.size(); i++) {
        v[i].signal();
    }
}

void run_main(int iterations,
              int workers,
              int payload_size,
              int cs_pair,
              bool training_round=false)
{
    vector<Pipe> pipes;
    // Create all the workers and wait for them to spawn.
    for (int i = 0; i < workers; i++) {
        pipes.push_back(make_worker(i, iterations, workers, payload_size, cs_pair));
    }
    wait_all(pipes);

    // Run the workers and wait for completion.
    chrono::time_point<chrono::high_resolution_clock> start, end;
    cout << "waiting for workers to complete" << endl;
    start = chrono::high_resolution_clock::now();
    signal_all(pipes);
    wait_all(pipes);
    end = chrono::high_resolution_clock::now();

    // Calculate overall throughput.
    double iterations_per_sec = double(iterations * workers) / (chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1.0E9);
    cout << "iterations per sec: " << iterations_per_sec << endl;

    // Collect all results from the workers.
    cout << "collecting results" << endl;
    signal_all(pipes);
    ProcResults tot_results;
    for (int i = 0; i < workers; i++) {
        ProcResults tmp_results;
        pipes[i].recv(tmp_results);
        tot_results = ProcResults::combine(tot_results, tmp_results);
    }

    // Kill all the workers.
    cout << "killing workers" << endl;
    signal_all(pipes);
    for (int i = 0; i < workers; i++) {
        int status;
        wait(&status);
        if (status != 0) {
            cout << "nonzero child status" << status << endl;
        }
    }
    if (training_round) {
        // sets max_time_bucket to 2 * m_worst from the training round.
        // Also needs to adjust time_per_bucket accordingly.
        max_time_bucket = 2 * tot_results.m_worst;
        time_per_bucket = max_time_bucket / num_buckets;
        cout << "Max latency during training: " << tot_results.m_worst / 1.0E6 << "ms" << endl;
    } else {
            tot_results.dump();
    }
}

int main(int argc, char *argv[])
{
    int workers = 2;
    int iterations = 10000;
    int payload_size = 0;
    bool cs_pair = false;
    bool training_round = false;
    (void)argc;
    (void)argv;

    // Parse arguments.
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--help") {
            cout << "Usage: binderThroughputTest [OPTIONS]" << endl;
            cout << "\t-i N    : Specify number of iterations." << endl;
            cout << "\t-m N    : Specify expected max latency in microseconds." << endl;
            cout << "\t-p      : Split workers into client/server pairs." << endl;
            cout << "\t-s N    : Specify payload size." << endl;
            cout << "\t-t N    : Run training round." << endl;
            cout << "\t-w N    : Specify total number of workers." << endl;
            return 0;
        }
        if (string(argv[i]) == "-w") {
            workers = atoi(argv[i+1]);
            i++;
            continue;
        }
        if (string(argv[i]) == "-i") {
            iterations = atoi(argv[i+1]);
            i++;
            continue;
        }
        if (string(argv[i]) == "-s") {
            payload_size = atoi(argv[i+1]);
            i++;
        }
        if (string(argv[i]) == "-p") {
            // client/server pairs instead of spreading
            // requests to all workers. If true, half
            // the workers become clients and half servers
            cs_pair = true;
        }
        if (string(argv[i]) == "-t") {
            // Run one training round before actually collecting data
            // to get an approximation of max latency.
            training_round = true;
        }
        if (string(argv[i]) == "-m") {
            // Caller specified the max latency in microseconds.
            // No need to run training round in this case.
            if (atoi(argv[i+1]) > 0) {
                max_time_bucket = strtoull(argv[i+1], (char **)nullptr, 10) * 1000;
                time_per_bucket = max_time_bucket / num_buckets;
                i++;
            } else {
                cout << "Max latency -m must be positive." << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    if (training_round) {
        cout << "Start training round" << endl;
        run_main(iterations, workers, payload_size, cs_pair, training_round=true);
        cout << "Completed training round" << endl << endl;
    }

    run_main(iterations, workers, payload_size, cs_pair);
    return 0;
}
