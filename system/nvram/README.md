# Access-controlled NVRAM implementation

This repository contains various pieces related to the Access-controlled NVRAM
HAL. In a nutshell, the Access-controlled NVRAM HAL allows creation of NVRAM
spaces that can hold arbitrary data blobs of limited size. Access restrictions
can be configured on each NVRAM space to prevent the contents from being
accessed or modified, up to the point of requiring full hardware reset to clear
a locked NVRAM space. This can be used for various security features that
require a trusted storage location for critical data that an attacker can't
tamper with. For details of the NVRAM HAL API, see
[hardware/libhardware/include/hardware/nvram.h](https://android.googlesource.com/platform/hardware/libhardware/+/master/include/hardware/nvram.h).

## [client](/client)

Contains a simple command-line application to interact with the NVRAM HAL
implementation. This is useful for experimentation and use in scripts.
`nvram-client` receives commands and parameters on the command line and
translates them to NVRAM HAL calls. Results are printed on stdout.

## [core](/core)

A reference implementation of the NVRAM functionality. This reference
implementation can be used to create NVRAM HAL implementations that run in a
trusted execution environment. It is also the basis for the testing NVRAM HAL
module, which implements the entire NVRAM HAL API surface in userspace for the
sake of illustration (but obviously doesn't meet the persistence and
tamper-evidence requirements).

Note that the reference implementation leaves persistent storage to be handled
by the embedding code, which needs to provide an implementation of the storage
interface defined in
[system/nvram/core/include/nvram/core/storage.h](core/include/nvram/core/storage.h).

## [hal](/hal)

The [hal](/hal) directory contains glue code that simplifies creation of NVRAM
HAL modules. The code implements the API surface specified by the nvram.h HAL
header and translates calls into the request/response message format defined in
the [messages](/messages) directory. Thus, to create a working NVRAM HAL module,
it is sufficient to provide an implementation that understands `nvram::Request`
and `nvram::Response` objects, the glue code will adapt it to the full NVRAM API
surface.

## [messages](/messages)

Defines an IPC message format that can be used to serialize NVRAM HAL calls and
their parameters in preparation for sending them elsewhere (e.g., a TEE) for
execution. There is a request and a response struct corresponding to each NVRAM
HAL function. The `nvram::Request` and `nvram::Response` wrappers keep track of
the actual request or response type, respectively, as well as the request or
response parameters specific to the type.
