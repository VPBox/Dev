#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>

using namespace std;
using namespace android;

enum BinderWorkerServiceCode {
  BINDER_NOP = IBinder::FIRST_CALL_TRANSACTION,
};

#define ASSERT(cond)                                                \
  do {                                                              \
    if (!(cond)) {                                                  \
      cerr << __func__ << ":" << __LINE__ << " condition:" << #cond \
           << " failed\n"                                           \
           << endl;                                                 \
      exit(EXIT_FAILURE);                                           \
    }                                                               \
  } while (0)

vector<sp<IBinder> > workers;

// the ratio that the service is synced on the same cpu beyond
// GOOD_SYNC_MIN is considered as good
#define GOOD_SYNC_MIN (0.6)

#define DUMP_PRESICION 2

string trace_path = "/sys/kernel/debug/tracing";

// the default value
int no_process = 2;
int iterations = 100;
int payload_size = 16;
int no_inherent = 0;
int no_sync = 0;
int verbose = 0;
int trace;

bool traceIsOn() {
  fstream file;
  file.open(trace_path + "/tracing_on", ios::in);
  char on;
  file >> on;
  file.close();
  return on == '1';
}

void traceStop() {
  ofstream file;
  file.open(trace_path + "/tracing_on", ios::out | ios::trunc);
  file << '0' << endl;
  file.close();
}

// the deadline latency that we are interested in
uint64_t deadline_us = 2500;

int thread_pri() {
  struct sched_param param;
  int policy;
  ASSERT(!pthread_getschedparam(pthread_self(), &policy, &param));
  return param.sched_priority;
}

void thread_dump(const char* prefix) {
  struct sched_param param;
  int policy;
  if (!verbose) return;
  cout << "--------------------------------------------------" << endl;
  cout << setw(12) << left << prefix << " pid: " << getpid()
       << " tid: " << gettid() << " cpu: " << sched_getcpu() << endl;
  ASSERT(!pthread_getschedparam(pthread_self(), &policy, &param));
  string s = (policy == SCHED_OTHER)
                 ? "SCHED_OTHER"
                 : (policy == SCHED_FIFO)
                       ? "SCHED_FIFO"
                       : (policy == SCHED_RR) ? "SCHED_RR" : "???";
  cout << setw(12) << left << s << param.sched_priority << endl;
  return;
}

class BinderWorkerService : public BBinder {
 public:
  BinderWorkerService() {
  }
  ~BinderWorkerService() {
  }
  virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                              uint32_t flags = 0) {
    (void)flags;
    (void)data;
    (void)reply;
    switch (code) {
      // The transaction format is like
      //
      // data[in]:  int32: caller priority
      //            int32: caller cpu
      //
      // reply[out]: int32: 1 if caller's priority != callee's priority
      //             int32: 1 if caller's cpu != callee's cpu
      //
      // note the caller cpu read here is not always correct
      // there're still chances that the caller got switched out
      // right after it read the cpu number and still before the transaction.
      case BINDER_NOP: {
        thread_dump("binder");
        int priority = thread_pri();
        int priority_caller = data.readInt32();
        int h = 0, s = 0;
        if (priority_caller != priority) {
          h++;
          if (verbose) {
            cout << "err priority_caller:" << priority_caller
                 << ", priority:" << priority << endl;
          }
        }
        if (priority == sched_get_priority_max(SCHED_FIFO)) {
          int cpu = sched_getcpu();
          int cpu_caller = data.readInt32();
          if (cpu != cpu_caller) {
            s++;
          }
        }
        reply->writeInt32(h);
        reply->writeInt32(s);
        return NO_ERROR;
      }
      default:
        return UNKNOWN_TRANSACTION;
    };
  }
};

class Pipe {
  int m_readFd;
  int m_writeFd;
  Pipe(int readFd, int writeFd) : m_readFd{readFd}, m_writeFd{writeFd} {
  }
  Pipe(const Pipe&) = delete;
  Pipe& operator=(const Pipe&) = delete;
  Pipe& operator=(const Pipe&&) = delete;

 public:
  Pipe(Pipe&& rval) noexcept {
    m_readFd = rval.m_readFd;
    m_writeFd = rval.m_writeFd;
    rval.m_readFd = 0;
    rval.m_writeFd = 0;
  }
  ~Pipe() {
    if (m_readFd) close(m_readFd);
    if (m_writeFd) close(m_writeFd);
  }
  void signal() {
    bool val = true;
    int error = write(m_writeFd, &val, sizeof(val));
    ASSERT(error >= 0);
  };
  void wait() {
    bool val = false;
    int error = read(m_readFd, &val, sizeof(val));
    ASSERT(error >= 0);
  }
  template <typename T>
  void send(const T& v) {
    int error = write(m_writeFd, &v, sizeof(T));
    ASSERT(error >= 0);
  }
  template <typename T>
  void recv(T& v) {
    int error = read(m_readFd, &v, sizeof(T));
    ASSERT(error >= 0);
  }
  static tuple<Pipe, Pipe> createPipePair() {
    int a[2];
    int b[2];

    int error1 = pipe(a);
    int error2 = pipe(b);
    ASSERT(error1 >= 0);
    ASSERT(error2 >= 0);

    return make_tuple(Pipe(a[0], b[1]), Pipe(b[0], a[1]));
  }
};

typedef chrono::time_point<chrono::high_resolution_clock> Tick;

static inline Tick tickNow() {
  return chrono::high_resolution_clock::now();
}

static inline uint64_t tickNano(Tick& sta, Tick& end) {
  return uint64_t(chrono::duration_cast<chrono::nanoseconds>(end - sta).count());
}

struct Results {
  uint64_t m_best = 0xffffffffffffffffULL;
  uint64_t m_worst = 0;
  uint64_t m_transactions = 0;
  uint64_t m_total_time = 0;
  uint64_t m_miss = 0;
  bool tracing;
  explicit Results(bool _tracing) : tracing(_tracing) {
  }
  inline bool miss_deadline(uint64_t nano) {
    return nano > deadline_us * 1000;
  }
  void add_time(uint64_t nano) {
    m_best = min(nano, m_best);
    m_worst = max(nano, m_worst);
    m_transactions += 1;
    m_total_time += nano;
    if (miss_deadline(nano)) m_miss++;
    if (miss_deadline(nano) && tracing) {
      // There might be multiple process pair running the test concurrently
      // each may execute following statements and only the first one actually
      // stop the trace and any traceStop() afterthen has no effect.
      traceStop();
      cout << endl;
      cout << "deadline triggered: halt & stop trace" << endl;
      cout << "log:" + trace_path + "/trace" << endl;
      cout << endl;
      exit(1);
    }
  }
  void dump() {
    double best = (double)m_best / 1.0E6;
    double worst = (double)m_worst / 1.0E6;
    double average = (double)m_total_time / m_transactions / 1.0E6;
    // FIXME: libjson?
    int W = DUMP_PRESICION + 2;
    cout << setprecision(DUMP_PRESICION) << "{ \"avg\":" << setw(W) << left
         << average << ",\"wst\":" << setw(W) << left << worst
         << ",\"bst\":" << setw(W) << left << best << ",\"miss\":" << left
         << m_miss << ",\"meetR\":" << left << setprecision(DUMP_PRESICION + 3)
         << (1.0 - (double)m_miss / m_transactions) << "}";
  }
};

String16 generateServiceName(int num) {
  char num_str[32];
  snprintf(num_str, sizeof(num_str), "%d", num);
  String16 serviceName = String16("binderWorker") + String16(num_str);
  return serviceName;
}

static void parcel_fill(Parcel& data, int sz, int priority, int cpu) {
  ASSERT(sz >= (int)sizeof(uint32_t) * 2);
  data.writeInt32(priority);
  data.writeInt32(cpu);
  sz -= sizeof(uint32_t);
  while (sz > (int)sizeof(uint32_t)) {
    data.writeInt32(0);
    sz -= sizeof(uint32_t);
  }
}

typedef struct {
  void* result;
  int target;
} thread_priv_t;

static void* thread_start(void* p) {
  thread_priv_t* priv = (thread_priv_t*)p;
  int target = priv->target;
  Results* results_fifo = (Results*)priv->result;
  Parcel data, reply;
  Tick sta, end;

  parcel_fill(data, payload_size, thread_pri(), sched_getcpu());
  thread_dump("fifo-caller");

  sta = tickNow();
  status_t ret = workers[target]->transact(BINDER_NOP, data, &reply);
  end = tickNow();
  results_fifo->add_time(tickNano(sta, end));

  no_inherent += reply.readInt32();
  no_sync += reply.readInt32();
  return nullptr;
}

// create a fifo thread to transact and wait it to finished
static void thread_transaction(int target, Results* results_fifo) {
  thread_priv_t thread_priv;
  void* dummy;
  pthread_t thread;
  pthread_attr_t attr;
  struct sched_param param;
  thread_priv.target = target;
  thread_priv.result = results_fifo;
  ASSERT(!pthread_attr_init(&attr));
  ASSERT(!pthread_attr_setschedpolicy(&attr, SCHED_FIFO));
  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  ASSERT(!pthread_attr_setschedparam(&attr, &param));
  ASSERT(!pthread_create(&thread, &attr, &thread_start, &thread_priv));
  ASSERT(!pthread_join(thread, &dummy));
}

#define is_client(_num) ((_num) >= (no_process / 2))

void worker_fx(int num, int no_process, int iterations, int payload_size,
               Pipe p) {
  int dummy;
  Results results_other(false), results_fifo(trace);

  // Create BinderWorkerService and for go.
  ProcessState::self()->startThreadPool();
  sp<IServiceManager> serviceMgr = defaultServiceManager();
  sp<BinderWorkerService> service = new BinderWorkerService;
  serviceMgr->addService(generateServiceName(num), service);
  // init done
  p.signal();
  // wait for kick-off
  p.wait();

  // If client/server pairs, then half the workers are
  // servers and half are clients
  int server_count = no_process / 2;

  for (int i = 0; i < server_count; i++) {
    // self service is in-process so just skip
    if (num == i) continue;
    workers.push_back(serviceMgr->getService(generateServiceName(i)));
  }

  // Client for each pair iterates here
  // each iterations contains exatcly 2 transactions
  for (int i = 0; is_client(num) && i < iterations; i++) {
    Parcel data, reply;
    Tick sta, end;
    // the target is paired to make it easier to diagnose
    int target = num % server_count;

    // 1. transaction by fifo thread
    thread_transaction(target, &results_fifo);
    parcel_fill(data, payload_size, thread_pri(), sched_getcpu());
    thread_dump("other-caller");

    // 2. transaction by other thread
    sta = tickNow();
    ASSERT(NO_ERROR == workers[target]->transact(BINDER_NOP, data, &reply));
    end = tickNow();
    results_other.add_time(tickNano(sta, end));

    no_inherent += reply.readInt32();
    no_sync += reply.readInt32();
  }
  // Signal completion to master and wait.
  p.signal();
  p.wait();

  p.send(&dummy);
  // wait for kill
  p.wait();
  // Client for each pair dump here
  if (is_client(num)) {
    int no_trans = iterations * 2;
    double sync_ratio = (1.0 - (double)no_sync / no_trans);
    // FIXME: libjson?
    cout << "\"P" << (num - server_count) << "\":{\"SYNC\":\""
         << ((sync_ratio > GOOD_SYNC_MIN) ? "GOOD" : "POOR") << "\","
         << "\"S\":" << (no_trans - no_sync) << ",\"I\":" << no_trans << ","
         << "\"R\":" << sync_ratio << "," << endl;

    cout << "  \"other_ms\":";
    results_other.dump();
    cout << "," << endl;
    cout << "  \"fifo_ms\": ";
    results_fifo.dump();
    cout << endl;
    cout << "}," << endl;
  }
  exit(no_inherent);
}

Pipe make_process(int num, int iterations, int no_process, int payload_size) {
  auto pipe_pair = Pipe::createPipePair();
  pid_t pid = fork();
  if (pid) {
    // parent
    return move(get<0>(pipe_pair));
  } else {
    // child
    thread_dump(is_client(num) ? "client" : "server");
    worker_fx(num, no_process, iterations, payload_size,
              move(get<1>(pipe_pair)));
    // never get here
    return move(get<0>(pipe_pair));
  }
}

void wait_all(vector<Pipe>& v) {
  for (size_t i = 0; i < v.size(); i++) {
    v[i].wait();
  }
}

void signal_all(vector<Pipe>& v) {
  for (size_t i = 0; i < v.size(); i++) {
    v[i].signal();
  }
}

// This test is modified from binderThroughputTest.cpp
int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-i") {
      iterations = atoi(argv[i + 1]);
      i++;
      continue;
    }
    if (string(argv[i]) == "-pair") {
      no_process = 2 * atoi(argv[i + 1]);
      i++;
      continue;
    }
    if (string(argv[i]) == "-deadline_us") {
      deadline_us = atoi(argv[i + 1]);
      i++;
      continue;
    }
    if (string(argv[i]) == "-v") {
      verbose = 1;
    }
    // The -trace argument is used like that:
    //
    // First start trace with atrace command as usual
    // >atrace --async_start sched freq
    //
    // then use schd-dbg with -trace arguments
    //./schd-dbg -trace -deadline_us 2500
    //
    // This makes schd-dbg to stop trace once it detects a transaction
    // duration over the deadline. By writing '0' to
    // /sys/kernel/debug/tracing and halt the process. The tracelog is
    // then available on /sys/kernel/debug/trace
    if (string(argv[i]) == "-trace") {
      trace = 1;
    }
  }
  if (trace && !traceIsOn()) {
    cout << "trace is not running" << endl;
    cout << "check " << trace_path + "/tracing_on" << endl;
    cout << "use atrace --async_start first" << endl;
    exit(-1);
  }
  vector<Pipe> pipes;
  thread_dump("main");
  // FIXME: libjson?
  cout << "{" << endl;
  cout << "\"cfg\":{\"pair\":" << (no_process / 2)
       << ",\"iterations\":" << iterations << ",\"deadline_us\":" << deadline_us
       << "}," << endl;

  // the main process fork 2 processes for each pairs
  // 1 server + 1 client
  // each has a pipe to communicate with
  for (int i = 0; i < no_process; i++) {
    pipes.push_back(make_process(i, iterations, no_process, payload_size));
  }
  // wait for init done
  wait_all(pipes);
  // kick-off iterations
  signal_all(pipes);
  // wait for completion
  wait_all(pipes);
  // start to send result
  signal_all(pipes);
  for (int i = 0; i < no_process; i++) {
    int status;
    // kill
    pipes[i].signal();
    wait(&status);
    // the exit status is number of transactions without priority inheritance
    // detected in the child process
    no_inherent += status;
  }
  // FIXME: libjson?
  cout << "\"inheritance\": " << (no_inherent == 0 ? "\"PASS\"" : "\"FAIL\"")
       << endl;
  cout << "}" << endl;
  return -no_inherent;
}
