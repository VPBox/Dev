# NetD benchmarks

These are benchmarks for libc **connect** and **gethostbyname** functions as hooked by netd.

## Infrastructure

The benchmarks are powered by [google-benchmark](https://github.com/google/benchmark), which records
four statistics:

<table>
  <tr>
    <th>Statistic</th>
    <th>Meaning</th>
  </tr>
  <tr>
    <th>real_time</th>
    <td>Mean wall-clock time taken for each iteration of the function under test to complete. This
        includes any time spent in other processes.
        <p>
        Some of the **connect** benchmarks override this by implementing their own timings, but
        the semantics are roughly the same. See connect_benchmark.cpp for details.</td>
  </tr>
  <tr>
    <th>cpu_time</th>
    <td>Mean time spent in the current thread during each iteration of the function under test. This
        does not include any time spent in other processes or blocking on I/O.
        <p>
        Since almost all of the functionality we are testing here is in the netd server, not the
        client, these timings are not relevant and should be ignored for most purposes.</td>
  </tr>
  <tr>
    <th>iterations</th>
    <td>The number of runs of the test function that were executed, across all threads, in order for
        the test to reach the minimum amount of time run. Most tests here define 'minimum time run'
        in terms of real time or 'manual time', rather than CPU time, for the reasons above.
        <p>
        This should be roughly inversely proportional to **real_time** for single-threaded
        benchmarks and also somewhat proportional to the number of threads used.</td>
  </tr>
  <tr>
    <th>label</th>
    <td>Field that the benchmark can fill with an arbitrary string.
        <p>
        For graphing purposes, a few of the benchmarks in this directory store the 90th-percentile
        latency in this field for comparison against the mean latency
        (it's only possible to plot one other value apart from mean).
        <p>
        If the graphing ecosystem ever becomes advanced enough to plot multiple outputs from one
        label, it might be more useful to log every decile to get a better picture of how spiky
        things are.</td>
</table>

# Methods currently being benchmarked

## connect()

- Documented in [connect\_benchmark.cpp](connect_benchmark.cpp)

## getaddrinfo()

- Documented in [dns\_benchmark.cpp](dns_benchmark.cpp)


<style type="text/css">
  tr:nth-child(2n+1) {
    background: lightgrey;
  }
  td,th {
    text-align: left;
    vertical-align: top;
  }
</style>
