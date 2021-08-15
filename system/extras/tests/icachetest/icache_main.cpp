#include <stdio.h>
#include <sys/time.h>
#include <getopt.h>

#include <thread>
#include <iostream>
#include <iomanip>

#include <sched.h>

#include "Profiler.h"

extern "C" void icache_test(long count, long step);

static constexpr size_t MAX_CODE_SIZE = 128*1024;
static constexpr size_t CACHE_LINE_SIZE = 64;
static constexpr size_t MAX_ITERATIONS_COUNT = MAX_CODE_SIZE / CACHE_LINE_SIZE;
static constexpr size_t REPETITIONS = 0x800000L;


using namespace utils;

static cpu_set_t g_cpu_set;

static void printUsage(char* name) {
    std::string exec_name(name);
    std::string usage(
            "ICACHE is a command-line tool for testing the L1 instruction cache performance.\n"
            "(Make sure security.perf_harden is set to 0)\n\n"
            "Usages:\n"
            "    ICACHE [options]\n"
            "\n"
            "Options:\n"
            "   --help, -h\n"
            "       print this message\n\n"
            "   --affinity=N, -a N\n"
            "       Specify which CPU the test should run on.\n\n"
    );
    const std::string from("ICACHE");
    for (size_t pos = usage.find(from); pos != std::string::npos; pos = usage.find(from, pos)) {
         usage.replace(pos, from.length(), exec_name);
    }
    printf("%s", usage.c_str());
}

static int handleCommandLineArgments(int argc, char* argv[]) {
    static constexpr const char* OPTSTR = "ha:";
    static const struct option OPTIONS[] = {
            { "help",                 no_argument, 0, 'h' },
            { "affinity",       required_argument, 0, 'a' },
            { 0, 0, 0, 0 }  // termination of the option list
    };
    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, OPTSTR, OPTIONS, &option_index)) >= 0) {
        std::string arg(optarg ? optarg : "");
        switch (opt) {
            default:
            case 'h':
                printUsage(argv[0]);
                exit(0);
                break;
            case 'a':
                size_t cpu = std::stoi(arg);
                if (cpu < std::thread::hardware_concurrency()) {
                    CPU_SET(cpu, &g_cpu_set);
                } else {
                    std::cerr << "N must be < " << std::thread::hardware_concurrency() << std::endl;
                    exit(0);
                }
                break;
        }
    }
    return optind;
}

int main(int argc, char* argv[]) {
    CPU_ZERO(&g_cpu_set);

    [[maybe_unused]] int option_index = handleCommandLineArgments(argc, argv);
    [[maybe_unused]] int num_args = argc - option_index;

    if (CPU_COUNT(&g_cpu_set)) {
        sched_setaffinity(gettid(), sizeof(g_cpu_set), &g_cpu_set);
    }

    Profiler& profiler = Profiler::get();
    profiler.resetEvents(Profiler::EV_CPU_CYCLES | Profiler::EV_L1I_RATES);

    if (!profiler.isValid()) {
        fprintf(stderr, "performance counters not enabled. try \"setprop security.perf_harden 0\"\n");
        exit(0);
    }

    size_t const stepInBytes = 1024;    // 1 KiB steps
    size_t const step = stepInBytes / CACHE_LINE_SIZE;

    std::cout << std::fixed << std::setprecision(2);

    printf("[KiB]\t[cyc]\t[refs]\t[MPKI]\t[ns]\n");

    Profiler::Counters counters;

    for (size_t i=step ; i <= MAX_ITERATIONS_COUNT ; i += step) {
        profiler.reset();

        auto now = std::chrono::steady_clock::now();
        profiler.start();
        icache_test(REPETITIONS, i);
        profiler.stop();
        auto duration = std::chrono::steady_clock::now() - now;

        profiler.readCounters(&counters);

        std::cout << ((i*CACHE_LINE_SIZE)/1024) << "\t"
            << counters.getCpuCycles()/double(REPETITIONS) << "\t"
            << counters.getL1IReferences()/double(REPETITIONS) << "\t"
            << counters.getMPKI(counters.getL1IMisses()) << "\t"
            << duration.count()/double(REPETITIONS) << "\t"
            << std::endl;
    }

    return 0;
}
