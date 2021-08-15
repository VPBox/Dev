# DNS-over-TLS query forwarder design

## Overview

The DNS-over-TLS query forwarder consists of five classes:
 * `DnsTlsDispatcher`
 * `DnsTlsTransport`
 * `DnsTlsQueryMap`
 * `DnsTlsSessionCache`
 * `DnsTlsSocket`

`DnsTlsDispatcher` is a singleton class whose `query` method is the DnsTls's
only public interface.  `DnsTlsDispatcher` is just a table holding the
`DnsTlsTransport` for each server (represented by a `DnsTlsServer` struct) and
network.  `DnsTlsDispatcher` also blocks each query thread, waiting on a
`std::future` returned by `DnsTlsTransport` that represents the response.

`DnsTlsTransport` sends each query over a `DnsTlsSocket`, opening a
new one if necessary.  It also has to listen for responses from the
`DnsTlsSocket`, which happen on a different thread.
`IDnsTlsSocketObserver` is an interface defining how `DnsTlsSocket` returns
responses to `DnsTlsTransport`.

`DnsTlsQueryMap` and `DnsTlsSessionCache` are helper classes owned by `DnsTlsTransport`.
`DnsTlsQueryMap` handles ID renumbering and query-response pairing.
`DnsTlsSessionCache` allows TLS session resumption.

`DnsTlsSocket` interleaves all queries onto a single socket, and reports all
responses to `DnsTlsTransport` (through the `IDnsTlsObserver` interface).  It doesn't
know anything about which queries correspond to which responses, and does not retain
state to indicate whether there is an outstanding query.

## Threading

### Overall patterns

For clarity, each of the five classes in this design is thread-safe and holds one lock.
Classes that spawn a helper thread call `thread::join()` in their destructor to ensure
that it is cleaned up appropriately.

All the classes here make full use of Clang thread annotations (and also null-pointer
annotations) to minimize the likelihood of a latent threading bug.  The unit tests are
also heavily threaded to exercise this functionality.

This code creates O(1) threads per socket, and does not create a new thread for each
query or response.  However, DnsProxyListener does create a thread for each query.

### Threading in `DnsTlsSocket`

`DnsTlsSocket` can receive queries on any thread, and send them over a
"reliable datagram pipe" (`socketpair()` in `SOCK_SEQPACKET` mode).
The query method writes a struct (containing a pointer to the query) to the pipe
from its thread, and the loop thread (which owns the SSL socket)
reads off the other end of the pipe.  The pipe doesn't actually have a queue "inside";
instead, any queueing happens by blocking the query thread until the
socket thread can read the datagram off the other end.

We need to pass messages between threads using a pipe, and not a condition variable
or a thread-safe queue, because the socket thread has to be blocked
in `poll()` waiting for data from the server, but also has to be woken
up on inputs from the query threads.  Therefore, inputs from the query
threads have to arrive on a socket, so that `poll()` can listen for them.
(There can only be a single thread because [you can't use different threads
to read and write in OpenSSL](https://www.openssl.org/blog/blog/2017/02/21/threads/)).

## ID renumbering

`DnsTlsDispatcher` accepts queries that have colliding ID numbers and still sends them on
a single socket.  To avoid confusion at the server, `DnsTlsQueryMap` assigns each
query a new ID for transmission, records the mapping from input IDs to sent IDs, and
applies the inverse mapping to responses before returning them to the caller.

`DnsTlsQueryMap` assigns each new query the ID number one greater than the largest
ID number of an outstanding query.  This means that ID numbers are initially sequential
and usually small.  If the largest possible ID number is already in use,
`DnsTlsQueryMap` will scan the ID space to find an available ID, or fail the query
if there are no available IDs.  Queries will not block waiting for an ID number to
become available.

## Time constants

`DnsTlsSocket` imposes a 20-second inactivity timeout.  A socket that has been idle for
20 seconds will be closed.  This sets the limit of tolerance for slow replies,
which could happen as a result of malfunctioning authoritative DNS servers.
If there are any pending queries, `DnsTlsTransport` will retry them.

`DnsTlsQueryMap` imposes a retry limit of 3.  `DnsTlsTransport` will retry the query up
to 3 times before reporting failure to `DnsTlsDispatcher`.
This limit helps to ensure proper functioning in the case of a recursive resolver that
is malfunctioning or is flooded with requests that are stalled due to malfunctioning
authoritative servers.

`DnsTlsDispatcher` maintains a 5-minute timeout.  Any `DnsTlsTransport` that has had no
outstanding queries for 5 minutes will be destroyed at the next query on a different
transport.
This sets the limit on how long session tickets will be preserved during idle periods,
because each `DnsTlsTransport` owns a `DnsTlsSessionCache`.  Imposing this timeout
increases latency on the first query after an idle period, but also helps to avoid
unbounded memory usage.

`DnsTlsSessionCache` sets a limit of 5 sessions in each cache, expiring the oldest one
when the limit is reached.  However, because the client code does not currently
reuse sessions more than once, it should not be possible to hit this limit.

## Testing

Unit tests are in `dns_tls_test.cpp`. They cover all the classes except
`DnsTlsSocket` (which requires `CAP_NET_ADMIN` because it uses `setsockopt(SO_MARK)`) and
`DnsTlsSessionCache` (which requires integration with libssl).  These classes are
exercised by the integration tests in `../tests/resolv_test.cpp`.

### Dependency Injection

For unit testing, we would like to be able to mock out `DnsTlsSocket`.  This is
particularly required for unit testing of `DnsTlsDispatcher` and `DnsTlsTransport`.
To make these unit tests possible, this code uses a dependency injection pattern:
`DnsTlsSocket` is produced by a `DnsTlsSocketFactory`, and both of these have a
defined interface.

`DnsTlsDispatcher`'s constructor takes an `IDnsTlsSocketFactory`,
which in production is a `DnsTlsSocketFactory`.  However, in unit tests, we can
substitute a test factory that returns a fake socket, so that the unit tests can
run without actually connecting over TLS to a test server.  (The integration tests
do actual TLS.)

## Logging

This code uses `ALOGV` throughout for low-priority logging, and does not use
`ALOGD`.  `ALOGV` is disabled by default, unless activated by `#define LOG_NDEBUG 0`.
(`ALOGD` is not disabled by default, requiring extra measures to avoid spamming the
system log in production builds.)

## Reference
 * [BoringSSL API docs](https://commondatastorage.googleapis.com/chromium-boringssl-docs/headers.html)
