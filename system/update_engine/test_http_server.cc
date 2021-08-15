//
// Copyright (C) 2012 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// This file implements a simple HTTP server. It can exhibit odd behavior
// that's useful for testing. For example, it's useful to test that
// the updater can continue a connection if it's dropped, or that it
// handles very slow data transfers.

// To use this, simply make an HTTP connection to localhost:port and
// GET a url.

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include <base/logging.h>
#include <base/posix/eintr_wrapper.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/http_common.h"

// HTTP end-of-line delimiter; sorry, this needs to be a macro.
#define EOL "\r\n"

using std::string;
using std::vector;

namespace chromeos_update_engine {

static const char* kListeningMsgPrefix = "listening on port ";

enum {
  RC_OK = 0,
  RC_BAD_ARGS,
  RC_ERR_READ,
  RC_ERR_SETSOCKOPT,
  RC_ERR_BIND,
  RC_ERR_LISTEN,
  RC_ERR_GETSOCKNAME,
  RC_ERR_REPORT,
};

struct HttpRequest {
  string raw_headers;
  string host;
  string url;
  off_t start_offset{0};
  off_t end_offset{0};  // non-inclusive, zero indicates unspecified.
  HttpResponseCode return_code{kHttpResponseOk};
};

bool ParseRequest(int fd, HttpRequest* request) {
  string headers;
  do {
    char buf[1024];
    ssize_t r = read(fd, buf, sizeof(buf));
    if (r < 0) {
      perror("read");
      exit(RC_ERR_READ);
    }
    headers.append(buf, r);
  } while (!base::EndsWith(headers, EOL EOL, base::CompareCase::SENSITIVE));

  LOG(INFO) << "got headers:\n--8<------8<------8<------8<----\n"
            << headers << "\n--8<------8<------8<------8<----";
  request->raw_headers = headers;

  // Break header into lines.
  vector<string> lines = base::SplitStringUsingSubstr(
      headers.substr(0, headers.length() - strlen(EOL EOL)),
      EOL,
      base::TRIM_WHITESPACE,
      base::SPLIT_WANT_ALL);

  // Decode URL line.
  vector<string> terms = base::SplitString(lines[0],
                                           base::kWhitespaceASCII,
                                           base::KEEP_WHITESPACE,
                                           base::SPLIT_WANT_NONEMPTY);
  CHECK_EQ(terms.size(), static_cast<vector<string>::size_type>(3));
  CHECK_EQ(terms[0], "GET");
  request->url = terms[1];
  LOG(INFO) << "URL: " << request->url;

  // Decode remaining lines.
  size_t i;
  for (i = 1; i < lines.size(); i++) {
    terms = base::SplitString(lines[i],
                              base::kWhitespaceASCII,
                              base::KEEP_WHITESPACE,
                              base::SPLIT_WANT_NONEMPTY);

    if (terms[0] == "Range:") {
      CHECK_EQ(terms.size(), static_cast<vector<string>::size_type>(2));
      string& range = terms[1];
      LOG(INFO) << "range attribute: " << range;
      CHECK(base::StartsWith(range, "bytes=", base::CompareCase::SENSITIVE) &&
            range.find('-') != string::npos);
      request->start_offset = atoll(range.c_str() + strlen("bytes="));
      // Decode end offset and increment it by one (so it is non-inclusive).
      if (range.find('-') < range.length() - 1)
        request->end_offset = atoll(range.c_str() + range.find('-') + 1) + 1;
      request->return_code = kHttpResponsePartialContent;
      string tmp_str = base::StringPrintf(
          "decoded range offsets: "
          "start=%jd end=",
          (intmax_t)request->start_offset);
      if (request->end_offset > 0)
        base::StringAppendF(
            &tmp_str, "%jd (non-inclusive)", (intmax_t)request->end_offset);
      else
        base::StringAppendF(&tmp_str, "unspecified");
      LOG(INFO) << tmp_str;
    } else if (terms[0] == "Host:") {
      CHECK_EQ(terms.size(), static_cast<vector<string>::size_type>(2));
      request->host = terms[1];
      LOG(INFO) << "host attribute: " << request->host;
    } else {
      LOG(WARNING) << "ignoring HTTP attribute: `" << lines[i] << "'";
    }
  }

  return true;
}

string Itoa(off_t num) {
  char buf[100] = {0};
  snprintf(buf, sizeof(buf), "%" PRIi64, num);
  return buf;
}

// Writes a string into a file. Returns total number of bytes written or -1 if a
// write error occurred.
ssize_t WriteString(int fd, const string& str) {
  const size_t total_size = str.size();
  size_t remaining_size = total_size;
  char const* data = str.data();

  while (remaining_size) {
    ssize_t written = write(fd, data, remaining_size);
    if (written < 0) {
      perror("write");
      LOG(INFO) << "write failed";
      return -1;
    }
    data += written;
    remaining_size -= written;
  }

  return total_size;
}

// Writes the headers of an HTTP response into a file.
ssize_t WriteHeaders(int fd,
                     const off_t start_offset,
                     const off_t end_offset,
                     HttpResponseCode return_code) {
  ssize_t written = 0, ret;

  ret = WriteString(fd,
                    string("HTTP/1.1 ") + Itoa(return_code) + " " +
                        GetHttpResponseDescription(return_code) +
                        EOL "Content-Type: application/octet-stream" EOL);
  if (ret < 0)
    return -1;
  written += ret;

  // Compute content legnth.
  const off_t content_length = end_offset - start_offset;

  // A start offset that equals the end offset indicates that the response
  // should contain the full range of bytes in the requested resource.
  if (start_offset || start_offset == end_offset) {
    ret = WriteString(
        fd,
        string("Accept-Ranges: bytes" EOL "Content-Range: bytes ") +
            Itoa(start_offset == end_offset ? 0 : start_offset) + "-" +
            Itoa(end_offset - 1) + "/" + Itoa(end_offset) + EOL);
    if (ret < 0)
      return -1;
    written += ret;
  }

  ret = WriteString(
      fd, string("Content-Length: ") + Itoa(content_length) + EOL EOL);
  if (ret < 0)
    return -1;
  written += ret;

  return written;
}

// Writes a predetermined payload of lines of ascending bytes to a file. The
// first byte of output is appropriately offset with respect to the request line
// length.  Returns the number of successfully written bytes.
size_t WritePayload(int fd,
                    const off_t start_offset,
                    const off_t end_offset,
                    const char first_byte,
                    const size_t line_len) {
  CHECK_LE(start_offset, end_offset);
  CHECK_GT(line_len, static_cast<size_t>(0));

  LOG(INFO) << "writing payload: " << line_len << "-byte lines starting with `"
            << first_byte << "', offset range " << start_offset << " -> "
            << end_offset;

  // Populate line of ascending characters.
  string line;
  line.reserve(line_len);
  char byte = first_byte;
  size_t i;
  for (i = 0; i < line_len; i++)
    line += byte++;

  const size_t total_len = end_offset - start_offset;
  size_t remaining_len = total_len;
  bool success = true;

  // If start offset is not aligned with line boundary, output partial line up
  // to the first line boundary.
  size_t start_modulo = start_offset % line_len;
  if (start_modulo) {
    string partial = line.substr(start_modulo, remaining_len);
    ssize_t ret = WriteString(fd, partial);
    if ((success = (ret >= 0 && (size_t)ret == partial.length())))
      remaining_len -= partial.length();
  }

  // Output full lines up to the maximal line boundary below the end offset.
  while (success && remaining_len >= line_len) {
    ssize_t ret = WriteString(fd, line);
    if ((success = (ret >= 0 && (size_t)ret == line_len)))
      remaining_len -= line_len;
  }

  // Output a partial line up to the end offset.
  if (success && remaining_len) {
    string partial = line.substr(0, remaining_len);
    ssize_t ret = WriteString(fd, partial);
    if ((success = (ret >= 0 && (size_t)ret == partial.length())))
      remaining_len -= partial.length();
  }

  return (total_len - remaining_len);
}

// Write default payload lines of the form 'abcdefghij'.
inline size_t WritePayload(int fd,
                           const off_t start_offset,
                           const off_t end_offset) {
  return WritePayload(fd, start_offset, end_offset, 'a', 10);
}

// Send an empty response, then kill the server.
void HandleQuit(int fd) {
  WriteHeaders(fd, 0, 0, kHttpResponseOk);
  LOG(INFO) << "pid(" << getpid() << "): HTTP server exiting ...";
  exit(RC_OK);
}

// Generates an HTTP response with payload corresponding to requested offsets
// and length.  Optionally, truncate the payload at a given length and add a
// pause midway through the transfer.  Returns the total number of bytes
// delivered or -1 for error.
ssize_t HandleGet(int fd,
                  const HttpRequest& request,
                  const size_t total_length,
                  const size_t truncate_length,
                  const int sleep_every,
                  const int sleep_secs) {
  ssize_t ret;
  size_t written = 0;

  // Obtain start offset, make sure it is within total payload length.
  const size_t start_offset = request.start_offset;
  if (start_offset >= total_length) {
    LOG(WARNING) << "start offset (" << start_offset
                 << ") exceeds total length (" << total_length
                 << "), generating error response ("
                 << kHttpResponseReqRangeNotSat << ")";
    return WriteHeaders(
        fd, total_length, total_length, kHttpResponseReqRangeNotSat);
  }

  // Obtain end offset, adjust to fit in total payload length and ensure it does
  // not preceded the start offset.
  size_t end_offset =
      (request.end_offset > 0 ? request.end_offset : total_length);
  if (end_offset < start_offset) {
    LOG(WARNING) << "end offset (" << end_offset << ") precedes start offset ("
                 << start_offset << "), generating error response";
    return WriteHeaders(fd, 0, 0, kHttpResponseBadRequest);
  }
  if (end_offset > total_length) {
    LOG(INFO) << "requested end offset (" << end_offset
              << ") exceeds total length (" << total_length << "), adjusting";
    end_offset = total_length;
  }

  // Generate headers
  LOG(INFO) << "generating response header: range=" << start_offset << "-"
            << (end_offset - 1) << "/" << (end_offset - start_offset)
            << ", return code=" << request.return_code;
  if ((ret = WriteHeaders(fd, start_offset, end_offset, request.return_code)) <
      0)
    return -1;
  LOG(INFO) << ret << " header bytes written";
  written += ret;

  // Compute payload length, truncate as necessary.
  size_t payload_length = end_offset - start_offset;
  if (truncate_length > 0 && truncate_length < payload_length) {
    LOG(INFO) << "truncating request payload length (" << payload_length
              << ") at " << truncate_length;
    payload_length = truncate_length;
    end_offset = start_offset + payload_length;
  }

  LOG(INFO) << "generating response payload: range=" << start_offset << "-"
            << (end_offset - 1) << "/" << (end_offset - start_offset);

  // Decide about optional midway delay.
  if (truncate_length > 0 && sleep_every > 0 && sleep_secs >= 0 &&
      start_offset % (truncate_length * sleep_every) == 0) {
    const off_t midway_offset = start_offset + payload_length / 2;

    if ((ret = WritePayload(fd, start_offset, midway_offset)) < 0)
      return -1;
    LOG(INFO) << ret << " payload bytes written (first chunk)";
    written += ret;

    LOG(INFO) << "sleeping for " << sleep_secs << " seconds...";
    sleep(sleep_secs);

    if ((ret = WritePayload(fd, midway_offset, end_offset)) < 0)
      return -1;
    LOG(INFO) << ret << " payload bytes written (second chunk)";
    written += ret;
  } else {
    if ((ret = WritePayload(fd, start_offset, end_offset)) < 0)
      return -1;
    LOG(INFO) << ret << " payload bytes written";
    written += ret;
  }

  LOG(INFO) << "response generation complete, " << written
            << " total bytes written";
  return written;
}

ssize_t HandleGet(int fd,
                  const HttpRequest& request,
                  const size_t total_length) {
  return HandleGet(fd, request, total_length, 0, 0, 0);
}

// Handles /redirect/<code>/<url> requests by returning the specified
// redirect <code> with a location pointing to /<url>.
void HandleRedirect(int fd, const HttpRequest& request) {
  LOG(INFO) << "Redirecting...";
  string url = request.url;
  CHECK_EQ(static_cast<size_t>(0), url.find("/redirect/"));
  url.erase(0, strlen("/redirect/"));
  string::size_type url_start = url.find('/');
  CHECK_NE(url_start, string::npos);
  HttpResponseCode code = StringToHttpResponseCode(url.c_str());
  url.erase(0, url_start);
  url = "http://" + request.host + url;
  const char* status = GetHttpResponseDescription(code);
  if (!status)
    CHECK(false) << "Unrecognized redirection code: " << code;
  LOG(INFO) << "Code: " << code << " " << status;
  LOG(INFO) << "New URL: " << url;

  ssize_t ret;
  if ((ret = WriteString(fd, "HTTP/1.1 " + Itoa(code) + " " + status + EOL)) <
      0)
    return;
  WriteString(fd, "Location: " + url + EOL);
}

// Generate a page not found error response with actual text payload. Return
// number of bytes written or -1 for error.
ssize_t HandleError(int fd, const HttpRequest& request) {
  LOG(INFO) << "Generating error HTTP response";

  ssize_t ret;
  size_t written = 0;

  const string data("This is an error page.");

  if ((ret = WriteHeaders(fd, 0, data.size(), kHttpResponseNotFound)) < 0)
    return -1;
  written += ret;

  if ((ret = WriteString(fd, data)) < 0)
    return -1;
  written += ret;

  return written;
}

// Generate an error response if the requested offset is nonzero, up to a given
// maximal number of successive failures.  The error generated is an "Internal
// Server Error" (500).
ssize_t HandleErrorIfOffset(int fd,
                            const HttpRequest& request,
                            size_t end_offset,
                            int max_fails) {
  static int num_fails = 0;

  if (request.start_offset > 0 && num_fails < max_fails) {
    LOG(INFO) << "Generating error HTTP response";

    ssize_t ret;
    size_t written = 0;

    const string data("This is an error page.");

    if ((ret = WriteHeaders(
             fd, 0, data.size(), kHttpResponseInternalServerError)) < 0)
      return -1;
    written += ret;

    if ((ret = WriteString(fd, data)) < 0)
      return -1;
    written += ret;

    num_fails++;
    return written;
  } else {
    num_fails = 0;
    return HandleGet(fd, request, end_offset);
  }
}

// Returns a valid response echoing in the body of the response all the headers
// sent by the client.
void HandleEchoHeaders(int fd, const HttpRequest& request) {
  WriteHeaders(fd, 0, request.raw_headers.size(), kHttpResponseOk);
  WriteString(fd, request.raw_headers);
}

void HandleHang(int fd) {
  LOG(INFO) << "Hanging until the other side of the connection is closed.";
  char c;
  while (HANDLE_EINTR(read(fd, &c, 1)) > 0) {
  }
}

void HandleDefault(int fd, const HttpRequest& request) {
  const off_t start_offset = request.start_offset;
  const string data("unhandled path");
  const size_t size = data.size();
  ssize_t ret;

  if ((ret = WriteHeaders(fd, start_offset, size, request.return_code)) < 0)
    return;
  WriteString(
      fd,
      (start_offset < static_cast<off_t>(size) ? data.substr(start_offset)
                                               : ""));
}

// Break a URL into terms delimited by slashes.
class UrlTerms {
 public:
  UrlTerms(const string& url, size_t num_terms) {
    // URL must be non-empty and start with a slash.
    CHECK_GT(url.size(), static_cast<size_t>(0));
    CHECK_EQ(url[0], '/');

    // Split it into terms delimited by slashes, omitting the preceding slash.
    terms = base::SplitString(
        url.substr(1), "/", base::KEEP_WHITESPACE, base::SPLIT_WANT_ALL);

    // Ensure expected length.
    CHECK_EQ(terms.size(), num_terms);
  }

  inline const string& Get(const off_t index) const { return terms[index]; }
  inline const char* GetCStr(const off_t index) const {
    return Get(index).c_str();
  }
  inline int GetInt(const off_t index) const { return atoi(GetCStr(index)); }
  inline size_t GetSizeT(const off_t index) const {
    return static_cast<size_t>(atol(GetCStr(index)));
  }

 private:
  vector<string> terms;
};

void HandleConnection(int fd) {
  HttpRequest request;
  ParseRequest(fd, &request);

  string& url = request.url;
  LOG(INFO) << "pid(" << getpid() << "): handling url " << url;
  if (url == "/quitquitquit") {
    HandleQuit(fd);
  } else if (base::StartsWith(
                 url, "/download/", base::CompareCase::SENSITIVE)) {
    const UrlTerms terms(url, 2);
    HandleGet(fd, request, terms.GetSizeT(1));
  } else if (base::StartsWith(url, "/flaky/", base::CompareCase::SENSITIVE)) {
    const UrlTerms terms(url, 5);
    HandleGet(fd,
              request,
              terms.GetSizeT(1),
              terms.GetSizeT(2),
              terms.GetInt(3),
              terms.GetInt(4));
  } else if (url.find("/redirect/") == 0) {
    HandleRedirect(fd, request);
  } else if (url == "/error") {
    HandleError(fd, request);
  } else if (base::StartsWith(
                 url, "/error-if-offset/", base::CompareCase::SENSITIVE)) {
    const UrlTerms terms(url, 3);
    HandleErrorIfOffset(fd, request, terms.GetSizeT(1), terms.GetInt(2));
  } else if (url == "/echo-headers") {
    HandleEchoHeaders(fd, request);
  } else if (url == "/hang") {
    HandleHang(fd);
  } else {
    HandleDefault(fd, request);
  }

  close(fd);
}

}  // namespace chromeos_update_engine

using namespace chromeos_update_engine;  // NOLINT(build/namespaces)

void usage(const char* prog_arg) {
  fprintf(stderr,
          "Usage: %s [ FILE ]\n"
          "Once accepting connections, the following is written to FILE (or "
          "stdout):\n"
          "\"%sN\" (where N is an integer port number)\n",
          basename(prog_arg),
          kListeningMsgPrefix);
}

int main(int argc, char** argv) {
  // Check invocation.
  if (argc > 2)
    errx(RC_BAD_ARGS, "unexpected number of arguments (use -h for usage)");

  // Parse (optional) argument.
  int report_fd = STDOUT_FILENO;
  if (argc == 2) {
    if (!strcmp(argv[1], "-h")) {
      usage(argv[0]);
      exit(RC_OK);
    }

    report_fd = open(argv[1], O_WRONLY | O_CREAT, 00644);
  }

  // Ignore SIGPIPE on write() to sockets.
  signal(SIGPIPE, SIG_IGN);

  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0)
    LOG(FATAL) << "socket() failed";

  struct sockaddr_in server_addr = sockaddr_in();
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = 0;

  {
    // Get rid of "Address in use" error
    int tr = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) ==
        -1) {
      perror("setsockopt");
      exit(RC_ERR_SETSOCKOPT);
    }
  }

  // Bind the socket and set for listening.
  if (bind(listen_fd,
           reinterpret_cast<struct sockaddr*>(&server_addr),
           sizeof(server_addr)) < 0) {
    perror("bind");
    exit(RC_ERR_BIND);
  }
  if (listen(listen_fd, 5) < 0) {
    perror("listen");
    exit(RC_ERR_LISTEN);
  }

  // Check the actual port.
  struct sockaddr_in bound_addr = sockaddr_in();
  socklen_t bound_addr_len = sizeof(bound_addr);
  if (getsockname(listen_fd,
                  reinterpret_cast<struct sockaddr*>(&bound_addr),
                  &bound_addr_len) < 0) {
    perror("getsockname");
    exit(RC_ERR_GETSOCKNAME);
  }
  in_port_t port = ntohs(bound_addr.sin_port);

  // Output the listening port, indicating that the server is processing
  // requests. IMPORTANT! (a) the format of this message is as expected by some
  // unit tests, avoid unilateral changes; (b) it is necessary to flush/sync the
  // file to prevent the spawning process from waiting indefinitely for this
  // message.
  string listening_msg = base::StringPrintf("%s%hu", kListeningMsgPrefix, port);
  LOG(INFO) << listening_msg;
  CHECK_EQ(write(report_fd, listening_msg.c_str(), listening_msg.length()),
           static_cast<int>(listening_msg.length()));
  CHECK_EQ(write(report_fd, "\n", 1), 1);
  if (report_fd == STDOUT_FILENO)
    fsync(report_fd);
  else
    close(report_fd);

  while (1) {
    LOG(INFO) << "pid(" << getpid() << "): waiting to accept new connection";
    int client_fd = accept(listen_fd, nullptr, nullptr);
    LOG(INFO) << "got past accept";
    if (client_fd < 0)
      LOG(FATAL) << "ERROR on accept";
    HandleConnection(client_fd);
  }
  return 0;
}
