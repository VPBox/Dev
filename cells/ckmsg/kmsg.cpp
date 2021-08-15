// Copyright 2006-2014 The Android Open Source Project

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <log/log.h>
#include <log/log_read.h>
#include <log/logprint.h>
#include <sys/klog.h>

#define DEFAULT_LOG_ROTATE_SIZE_KBYTES 16
#define DEFAULT_MAX_ROTATED_LOGS 4
static AndroidLogFormat * g_logformat;

namespace android {

/* Global Variables */

static const char * g_outputFileName = NULL;
static int g_logRotateSizeKBytes = 0;                   // 0 means "no log rotation"
static int g_maxRotatedLogs = DEFAULT_MAX_ROTATED_LOGS; // 0 means "unbounded"
static int g_outFD = -1;
static off_t g_outByteCount = 0;
static int g_printBinary = 0;
static int g_devCount = 0;

static int openLogFile (const char *pathname)
{
    return open(pathname, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
}

static void rotateLogs()
{
    int err;
    // Can't rotate logs if we're not outputting to a file
    if (g_outputFileName == NULL) {
        return;
    }

    close(g_outFD);

    for (int i = g_maxRotatedLogs ; i > 0 ; i--) {
        char *file0, *file1;

        asprintf(&file1, "%s.%d", g_outputFileName, i);

        if (i - 1 == 0) {
            asprintf(&file0, "%s", g_outputFileName);
        } else {
            asprintf(&file0, "%s.%d", g_outputFileName, i - 1);
        }

        err = rename (file0, file1);

        if (err < 0 && errno != ENOENT) {
            perror("while rotating log files");
        }

        free(file1);
        free(file0);
    }

    g_outFD = openLogFile (g_outputFileName);

    if (g_outFD < 0) {
        perror ("couldn't open output file");
        exit(-1);
    }

    g_outByteCount = 0;

}

static void setupOutput()
{

    if (g_outputFileName == NULL) {
        g_outFD = STDOUT_FILENO;

    } else {
        struct stat statbuf;

        g_outFD = openLogFile (g_outputFileName);

        if (g_outFD < 0) {
            perror ("couldn't open output file");
            exit(-1);
        }

        fstat(g_outFD, &statbuf);
        g_outByteCount = statbuf.st_size;
    }
}
static void show_help(const char *cmd)
{
    fprintf(stderr,"Usage: %s [options] [filterspecs]\n", cmd);

    fprintf(stderr, "options include:\n"
                    "  -f <filename>   Log to file. Default to stdout\n"
                    "  -r [<kbytes>]   Rotate log every kbytes. (16 if unspecified). Requires -f\n"
                    "  -n <count>      Sets max number of rotated logs to <count>, default 4\n"
                    "  -v <format>     Sets the log print format, where <format> is one of:\n\n"
                    "                  brief process tag thread raw time threadtime long\n\n"
                    "  -K              enable dmesg in logcat\n"
                    );
}

}
 /* namespace android */
static int setLogFormat(const char * formatString)
{
    static AndroidLogPrintFormat format;

    format = android_log_formatFromString(formatString);

    if (format == FORMAT_OFF) {
        // FORMAT_OFF means invalid string
        return -1;
    }

    android_log_setPrintFormat(g_logformat, format);

    return 0;
}
static bool g_kernelLogEnable = false;
#define KLOG_FILE "/proc/kmsg"
static int fd = -1;
static void klog_enable (bool enable)
{
    g_kernelLogEnable = enable;
}
static int klog_fd (void)
{
    if (fd <= 0)
        fd = open(KLOG_FILE, O_RDONLY|O_NONBLOCK);
    return fd;
}
static void klog_print (AndroidLogFormat *fmt, int out_fd)
{
    int n = 0;
    char buf[1024];
    struct timespec ts;
    AndroidLogEntry ale;
    memset(buf, 0, 1024);
    while ((n=read(klog_fd(), buf, 1024)) > 0) {
        clock_gettime(CLOCK_REALTIME, &ts);
        ale.priority   = ANDROID_LOG_WARN;
        ale.tv_sec     = ts.tv_sec;
        ale.tv_nsec    = ts.tv_nsec;
        ale.pid        = 0;
        ale.tid        = 0;
        ale.tag        = "Kernel-Dmesg";
        ale.message    = buf;
        ale.messageLen = n;
        android_log_printLogLine(fmt, out_fd,  &ale);
        android::g_outByteCount += n;
        if (android::g_logRotateSizeKBytes > 0 &&
                (android::g_outByteCount/1024) >= android::g_logRotateSizeKBytes)
            android::rotateLogs();
        memset(buf, 0, 1024);
    }
    return;
}
static void readKernelLogLines() {
    int max = 0;
    int result;
    fd_set readset;
    bool sleep = false;
    int kfd = klog_fd();
    if (g_kernelLogEnable && (kfd > max)){
        max = kfd;
        do {
            timeval timeout = { 0, 5000 /* 5ms */ }; // If we oversleep it's ok, i.e. ignore EINTR.
            FD_ZERO(&readset);
            FD_SET(kfd, &readset);
            result = select(max + 1, &readset, NULL, NULL, sleep ? NULL : &timeout);
        } while (result == -1 && errno == EINTR);
        if (result >= 0) {
            if (FD_ISSET(kfd, &readset))
                klog_print(g_logformat, android::g_outFD);
        }
    }
}
int main(int argc, char **argv)
{
    int err;
    int hasSetLogFormat = 0;
    const char *forceFilters = NULL;
    g_logformat = android_log_format_new();
    if (argc == 2 && 0 == strcmp(argv[1], "--help")) {
        android::show_help(argv[0]);
        exit(0);
    }
    umask(022);
    for (;;) {
        int ret;
        ret = getopt(argc, argv, "f:r:n:v:K");
        if (ret < 0) {
            break;
        }
        switch(ret) {
            case 'f':
                android::g_outputFileName = optarg;
            break;
            case 'r':
                if (optarg == NULL) {
                    android::g_logRotateSizeKBytes
                                = DEFAULT_LOG_ROTATE_SIZE_KBYTES;
                } else {
                    if (!isdigit(optarg[0])) {
                        fprintf(stderr,"Invalid parameter to -r\n");
                        android::show_help(argv[0]);
                        exit(-1);
                    }
                    android::g_logRotateSizeKBytes = atoi(optarg);
                }
            break;
            case 'n':
                if (!isdigit(optarg[0])) {
                    fprintf(stderr,"Invalid parameter to -r\n");
                    android::show_help(argv[0]);
                    exit(-1);
                }
                android::g_maxRotatedLogs = atoi(optarg);
            break;
            case 'v':
                err = setLogFormat (optarg);
                if (err < 0) {
                    fprintf(stderr,"Invalid parameter to -v\n");
                    android::show_help(argv[0]);
                    exit(-1);
                }

                hasSetLogFormat = 1;
            break;
            case 'K':
                klog_enable(true);
                break;
            default:
                fprintf(stderr,"Unrecognized Option\n");
                android::show_help(argv[0]);
                exit(-1);
            break;
        }
    }
    android::setupOutput();

    while (1) {
        readKernelLogLines();
    }
    if(fd >= 0){
        close(fd);
    }
    return 0;
}
