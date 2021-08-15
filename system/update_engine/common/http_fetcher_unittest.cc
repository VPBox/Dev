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

#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/message_loop/message_loop.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>
#include <brillo/message_loops/base_message_loop.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <brillo/process.h>
#include <brillo/streams/file_stream.h>
#include <brillo/streams/stream.h>
#include <gtest/gtest.h>

#include "update_engine/common/fake_hardware.h"
#include "update_engine/common/file_fetcher.h"
#include "update_engine/common/http_common.h"
#include "update_engine/common/mock_http_fetcher.h"
#include "update_engine/common/mock_proxy_resolver.h"
#include "update_engine/common/multi_range_http_fetcher.h"
#include "update_engine/common/proxy_resolver.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/libcurl_http_fetcher.h"

using brillo::MessageLoop;
using std::make_pair;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;
using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SaveArg;

namespace {

const int kBigLength = 100000;
const int kMediumLength = 1000;
const int kFlakyTruncateLength = 29000;
const int kFlakySleepEvery = 3;
const int kFlakySleepSecs = 10;

}  // namespace

namespace chromeos_update_engine {

static const char* kUnusedUrl = "unused://unused";

static inline string LocalServerUrlForPath(in_port_t port, const string& path) {
  string port_str = (port ? base::StringPrintf(":%hu", port) : "");
  return base::StringPrintf(
      "http://127.0.0.1%s%s", port_str.c_str(), path.c_str());
}

//
// Class hierarchy for HTTP server implementations.
//

class HttpServer {
 public:
  // This makes it an abstract class (dirty but works).
  virtual ~HttpServer() = 0;

  virtual in_port_t GetPort() const { return 0; }

  bool started_;
};

HttpServer::~HttpServer() {}

class NullHttpServer : public HttpServer {
 public:
  NullHttpServer() { started_ = true; }
};

class PythonHttpServer : public HttpServer {
 public:
  PythonHttpServer() : port_(0) {
    started_ = false;

    // Spawn the server process.
    unique_ptr<brillo::Process> http_server(new brillo::ProcessImpl());
    http_server->AddArg(test_utils::GetBuildArtifactsPath("test_http_server"));
    http_server->RedirectUsingPipe(STDOUT_FILENO, false);

    if (!http_server->Start()) {
      ADD_FAILURE() << "failed to spawn http server process";
      return;
    }
    LOG(INFO) << "started http server with pid " << http_server->pid();

    // Wait for server to begin accepting connections, obtain its port.
    brillo::StreamPtr stdout = brillo::FileStream::FromFileDescriptor(
        http_server->GetPipe(STDOUT_FILENO), false /* own */, nullptr);
    if (!stdout)
      return;

    vector<char> buf(128);
    string line;
    while (line.find('\n') == string::npos) {
      size_t read;
      if (!stdout->ReadBlocking(buf.data(), buf.size(), &read, nullptr)) {
        ADD_FAILURE() << "error reading http server stdout";
        return;
      }
      line.append(buf.data(), read);
      if (read == 0)
        break;
    }
    // Parse the port from the output line.
    const size_t listening_msg_prefix_len = strlen(kServerListeningMsgPrefix);
    if (line.size() < listening_msg_prefix_len) {
      ADD_FAILURE() << "server output too short";
      return;
    }

    EXPECT_EQ(kServerListeningMsgPrefix,
              line.substr(0, listening_msg_prefix_len));
    string port_str = line.substr(listening_msg_prefix_len);
    port_str.resize(port_str.find('\n'));
    EXPECT_TRUE(base::StringToUint(port_str, &port_));

    started_ = true;
    LOG(INFO) << "server running, listening on port " << port_;

    // Any failure before this point will SIGKILL the test server if started
    // when the |http_server| goes out of scope.
    http_server_ = std::move(http_server);
  }

  ~PythonHttpServer() {
    // If there's no process, do nothing.
    if (!http_server_)
      return;
    // Wait up to 10 seconds for the process to finish. Destroying the process
    // will kill it with a SIGKILL otherwise.
    http_server_->Kill(SIGTERM, 10);
  }

  in_port_t GetPort() const override { return port_; }

 private:
  static const char* kServerListeningMsgPrefix;

  unique_ptr<brillo::Process> http_server_;
  unsigned int port_;
};

const char* PythonHttpServer::kServerListeningMsgPrefix = "listening on port ";

//
// Class hierarchy for HTTP fetcher test wrappers.
//

class AnyHttpFetcherTest {
 public:
  AnyHttpFetcherTest() {}
  virtual ~AnyHttpFetcherTest() {}

  virtual HttpFetcher* NewLargeFetcher(ProxyResolver* proxy_resolver) = 0;
  HttpFetcher* NewLargeFetcher(size_t num_proxies) {
    proxy_resolver_.set_num_proxies(num_proxies);
    return NewLargeFetcher(&proxy_resolver_);
  }
  HttpFetcher* NewLargeFetcher() { return NewLargeFetcher(1); }

  virtual HttpFetcher* NewSmallFetcher(ProxyResolver* proxy_resolver) = 0;
  HttpFetcher* NewSmallFetcher() {
    proxy_resolver_.set_num_proxies(1);
    return NewSmallFetcher(&proxy_resolver_);
  }

  virtual string BigUrl(in_port_t port) const { return kUnusedUrl; }
  virtual string SmallUrl(in_port_t port) const { return kUnusedUrl; }
  virtual string ErrorUrl(in_port_t port) const { return kUnusedUrl; }

  virtual bool IsMock() const = 0;
  virtual bool IsMulti() const = 0;
  virtual bool IsHttpSupported() const = 0;
  virtual bool IsFileFetcher() const = 0;

  virtual void IgnoreServerAborting(HttpServer* server) const {}

  virtual HttpServer* CreateServer() = 0;

  FakeHardware* fake_hardware() { return &fake_hardware_; }

 protected:
  DirectProxyResolver proxy_resolver_;
  FakeHardware fake_hardware_;
};

class MockHttpFetcherTest : public AnyHttpFetcherTest {
 public:
  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewLargeFetcher;
  HttpFetcher* NewLargeFetcher(ProxyResolver* proxy_resolver) override {
    brillo::Blob big_data(1000000);
    return new MockHttpFetcher(
        big_data.data(), big_data.size(), proxy_resolver);
  }

  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewSmallFetcher;
  HttpFetcher* NewSmallFetcher(ProxyResolver* proxy_resolver) override {
    return new MockHttpFetcher("x", 1, proxy_resolver);
  }

  bool IsMock() const override { return true; }
  bool IsMulti() const override { return false; }
  bool IsHttpSupported() const override { return true; }
  bool IsFileFetcher() const override { return false; }

  HttpServer* CreateServer() override { return new NullHttpServer; }
};

class LibcurlHttpFetcherTest : public AnyHttpFetcherTest {
 public:
  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewLargeFetcher;
  HttpFetcher* NewLargeFetcher(ProxyResolver* proxy_resolver) override {
    LibcurlHttpFetcher* ret =
        new LibcurlHttpFetcher(proxy_resolver, &fake_hardware_);
    // Speed up test execution.
    ret->set_idle_seconds(1);
    ret->set_retry_seconds(1);
    fake_hardware_.SetIsOfficialBuild(false);
    return ret;
  }

  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewSmallFetcher;
  HttpFetcher* NewSmallFetcher(ProxyResolver* proxy_resolver) override {
    return NewLargeFetcher(proxy_resolver);
  }

  string BigUrl(in_port_t port) const override {
    return LocalServerUrlForPath(
        port, base::StringPrintf("/download/%d", kBigLength));
  }
  string SmallUrl(in_port_t port) const override {
    return LocalServerUrlForPath(port, "/foo");
  }
  string ErrorUrl(in_port_t port) const override {
    return LocalServerUrlForPath(port, "/error");
  }

  bool IsMock() const override { return false; }
  bool IsMulti() const override { return false; }
  bool IsHttpSupported() const override { return true; }
  bool IsFileFetcher() const override { return false; }

  void IgnoreServerAborting(HttpServer* server) const override {
    // Nothing to do.
  }

  HttpServer* CreateServer() override { return new PythonHttpServer; }
};

class MultiRangeHttpFetcherTest : public LibcurlHttpFetcherTest {
 public:
  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewLargeFetcher;
  HttpFetcher* NewLargeFetcher(ProxyResolver* proxy_resolver) override {
    MultiRangeHttpFetcher* ret = new MultiRangeHttpFetcher(
        new LibcurlHttpFetcher(proxy_resolver, &fake_hardware_));
    ret->ClearRanges();
    ret->AddRange(0);
    // Speed up test execution.
    ret->set_idle_seconds(1);
    ret->set_retry_seconds(1);
    fake_hardware_.SetIsOfficialBuild(false);
    return ret;
  }

  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewSmallFetcher;
  HttpFetcher* NewSmallFetcher(ProxyResolver* proxy_resolver) override {
    return NewLargeFetcher(proxy_resolver);
  }

  bool IsMulti() const override { return true; }
};

class FileFetcherTest : public AnyHttpFetcherTest {
 public:
  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewLargeFetcher;
  HttpFetcher* NewLargeFetcher(ProxyResolver* /* proxy_resolver */) override {
    return new FileFetcher();
  }

  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewSmallFetcher;
  HttpFetcher* NewSmallFetcher(ProxyResolver* proxy_resolver) override {
    return NewLargeFetcher(proxy_resolver);
  }

  string BigUrl(in_port_t port) const override {
    static string big_contents = []() {
      string buf;
      buf.reserve(kBigLength);
      constexpr const char* kBigUrlContent = "abcdefghij";
      for (size_t i = 0; i < kBigLength; i += strlen(kBigUrlContent)) {
        buf.append(kBigUrlContent,
                   std::min(kBigLength - i, strlen(kBigUrlContent)));
      }
      return buf;
    }();
    test_utils::WriteFileString(temp_file_.path(), big_contents);
    return "file://" + temp_file_.path();
  }
  string SmallUrl(in_port_t port) const override {
    test_utils::WriteFileString(temp_file_.path(), "small contents");
    return "file://" + temp_file_.path();
  }
  string ErrorUrl(in_port_t port) const override {
    return "file:///path/to/non-existing-file";
  }

  bool IsMock() const override { return false; }
  bool IsMulti() const override { return false; }
  bool IsHttpSupported() const override { return false; }
  bool IsFileFetcher() const override { return true; }

  void IgnoreServerAborting(HttpServer* server) const override {}

  HttpServer* CreateServer() override { return new NullHttpServer; }

 private:
  test_utils::ScopedTempFile temp_file_{"ue_file_fetcher.XXXXXX"};
};

class MultiRangeHttpFetcherOverFileFetcherTest : public FileFetcherTest {
 public:
  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewLargeFetcher;
  HttpFetcher* NewLargeFetcher(ProxyResolver* /* proxy_resolver */) override {
    MultiRangeHttpFetcher* ret = new MultiRangeHttpFetcher(new FileFetcher());
    ret->ClearRanges();
    // FileFetcher doesn't support range with unspecified length.
    ret->AddRange(0, 1);
    // Speed up test execution.
    ret->set_idle_seconds(1);
    ret->set_retry_seconds(1);
    fake_hardware_.SetIsOfficialBuild(false);
    return ret;
  }

  // Necessary to unhide the definition in the base class.
  using AnyHttpFetcherTest::NewSmallFetcher;
  HttpFetcher* NewSmallFetcher(ProxyResolver* proxy_resolver) override {
    return NewLargeFetcher(proxy_resolver);
  }

  bool IsMulti() const override { return true; }
};

//
// Infrastructure for type tests of HTTP fetcher.
// See: http://code.google.com/p/googletest/wiki/AdvancedGuide#Typed_Tests
//

// Fixture class template. We use an explicit constraint to guarantee that it
// can only be instantiated with an AnyHttpFetcherTest type, see:
// http://www2.research.att.com/~bs/bs_faq2.html#constraints
template <typename T>
class HttpFetcherTest : public ::testing::Test {
 public:
  base::MessageLoopForIO base_loop_;
  brillo::BaseMessageLoop loop_{&base_loop_};

  T test_;

 protected:
  HttpFetcherTest() { loop_.SetAsCurrent(); }

  void TearDown() override {
    EXPECT_EQ(0, brillo::MessageLoopRunMaxIterations(&loop_, 1));
  }

 private:
  static void TypeConstraint(T* a) {
    AnyHttpFetcherTest* b = a;
    if (b == 0)  // Silence compiler warning of unused variable.
      *b = a;
  }
};

// Test case types list.
typedef ::testing::Types<LibcurlHttpFetcherTest,
                         MockHttpFetcherTest,
                         MultiRangeHttpFetcherTest,
                         FileFetcherTest,
                         MultiRangeHttpFetcherOverFileFetcherTest>
    HttpFetcherTestTypes;
TYPED_TEST_CASE(HttpFetcherTest, HttpFetcherTestTypes);

namespace {
class HttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  HttpFetcherTestDelegate() = default;

  bool ReceivedBytes(HttpFetcher* /* fetcher */,
                     const void* bytes,
                     size_t length) override {
    data.append(reinterpret_cast<const char*>(bytes), length);
    // Update counters
    times_received_bytes_called_++;
    return true;
  }

  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    if (is_expect_error_)
      EXPECT_EQ(kHttpResponseNotFound, fetcher->http_response_code());
    else
      EXPECT_EQ(kHttpResponseOk, fetcher->http_response_code());
    MessageLoop::current()->BreakLoop();

    // Update counter
    times_transfer_complete_called_++;
  }

  void TransferTerminated(HttpFetcher* fetcher) override {
    times_transfer_terminated_called_++;
    MessageLoop::current()->BreakLoop();
  }

  // Are we expecting an error response? (default: no)
  bool is_expect_error_{false};

  // Counters for callback invocations.
  int times_transfer_complete_called_{0};
  int times_transfer_terminated_called_{0};
  int times_received_bytes_called_{0};

  // The received data bytes.
  string data;
};

void StartTransfer(HttpFetcher* http_fetcher, const string& url) {
  http_fetcher->BeginTransfer(url);
}
}  // namespace

TYPED_TEST(HttpFetcherTest, SimpleTest) {
  HttpFetcherTestDelegate delegate;
  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  this->loop_.PostTask(FROM_HERE,
                       base::Bind(StartTransfer,
                                  fetcher.get(),
                                  this->test_.SmallUrl(server->GetPort())));
  this->loop_.Run();
  EXPECT_EQ(0, delegate.times_transfer_terminated_called_);
}

TYPED_TEST(HttpFetcherTest, SimpleBigTest) {
  HttpFetcherTestDelegate delegate;
  unique_ptr<HttpFetcher> fetcher(this->test_.NewLargeFetcher());
  fetcher->set_delegate(&delegate);

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  this->loop_.PostTask(
      FROM_HERE,
      base::Bind(
          StartTransfer, fetcher.get(), this->test_.BigUrl(server->GetPort())));
  this->loop_.Run();
  EXPECT_EQ(0, delegate.times_transfer_terminated_called_);
}

// Issue #9648: when server returns an error HTTP response, the fetcher needs to
// terminate transfer prematurely, rather than try to process the error payload.
TYPED_TEST(HttpFetcherTest, ErrorTest) {
  if (this->test_.IsMock() || this->test_.IsMulti())
    return;
  HttpFetcherTestDelegate delegate;

  // Delegate should expect an error response.
  delegate.is_expect_error_ = true;

  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  this->loop_.PostTask(FROM_HERE,
                       base::Bind(StartTransfer,
                                  fetcher.get(),
                                  this->test_.ErrorUrl(server->GetPort())));
  this->loop_.Run();

  // Make sure that no bytes were received.
  EXPECT_EQ(0, delegate.times_received_bytes_called_);
  EXPECT_EQ(0U, fetcher->GetBytesDownloaded());

  // Make sure that transfer completion was signaled once, and no termination
  // was signaled.
  EXPECT_EQ(1, delegate.times_transfer_complete_called_);
  EXPECT_EQ(0, delegate.times_transfer_terminated_called_);
}

TYPED_TEST(HttpFetcherTest, ExtraHeadersInRequestTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;

  HttpFetcherTestDelegate delegate;
  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);
  fetcher->SetHeader("User-Agent", "MyTest");
  fetcher->SetHeader("user-agent", "Override that header");
  fetcher->SetHeader("Authorization", "Basic user:passwd");

  // Invalid headers.
  fetcher->SetHeader("X-Foo", "Invalid\nHeader\nIgnored");
  fetcher->SetHeader("X-Bar: ", "I do not know how to parse");

  // Hide Accept header normally added by default.
  fetcher->SetHeader("Accept", "");

  PythonHttpServer server;
  int port = server.GetPort();
  ASSERT_TRUE(server.started_);

  this->loop_.PostTask(
      FROM_HERE,
      base::Bind(StartTransfer,
                 fetcher.get(),
                 LocalServerUrlForPath(port, "/echo-headers")));
  this->loop_.Run();

  EXPECT_NE(string::npos,
            delegate.data.find("user-agent: Override that header\r\n"));
  EXPECT_NE(string::npos,
            delegate.data.find("Authorization: Basic user:passwd\r\n"));

  EXPECT_EQ(string::npos, delegate.data.find("\nAccept:"));
  EXPECT_EQ(string::npos, delegate.data.find("X-Foo: Invalid"));
  EXPECT_EQ(string::npos, delegate.data.find("X-Bar: I do not"));
}

namespace {
class PausingHttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* /* bytes */,
                     size_t /* length */) override {
    CHECK(!paused_);
    paused_ = true;
    fetcher->Pause();
    return true;
  }
  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    MessageLoop::current()->BreakLoop();
  }
  void TransferTerminated(HttpFetcher* fetcher) override { ADD_FAILURE(); }
  void Unpause() {
    CHECK(paused_);
    paused_ = false;
    fetcher_->Unpause();
  }
  bool paused_;
  HttpFetcher* fetcher_;
};

void UnpausingTimeoutCallback(PausingHttpFetcherTestDelegate* delegate,
                              MessageLoop::TaskId* my_id) {
  if (delegate->paused_)
    delegate->Unpause();
  // Update the task id with the new scheduled callback.
  *my_id = MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&UnpausingTimeoutCallback, delegate, my_id),
      base::TimeDelta::FromMilliseconds(200));
}
}  // namespace

TYPED_TEST(HttpFetcherTest, PauseTest) {
  PausingHttpFetcherTestDelegate delegate;
  unique_ptr<HttpFetcher> fetcher(this->test_.NewLargeFetcher());
  delegate.paused_ = false;
  delegate.fetcher_ = fetcher.get();
  fetcher->set_delegate(&delegate);

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  MessageLoop::TaskId callback_id;
  callback_id = this->loop_.PostDelayedTask(
      FROM_HERE,
      base::Bind(&UnpausingTimeoutCallback, &delegate, &callback_id),
      base::TimeDelta::FromMilliseconds(200));
  fetcher->BeginTransfer(this->test_.BigUrl(server->GetPort()));

  this->loop_.Run();
  EXPECT_TRUE(this->loop_.CancelTask(callback_id));
}

// This test will pause the fetcher while the download is not yet started
// because it is waiting for the proxy to be resolved.
TYPED_TEST(HttpFetcherTest, PauseWhileResolvingProxyTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;
  MockProxyResolver mock_resolver;
  unique_ptr<HttpFetcher> fetcher(this->test_.NewLargeFetcher(&mock_resolver));

  // Saved arguments from the proxy call.
  ProxiesResolvedFn proxy_callback;
  EXPECT_CALL(mock_resolver, GetProxiesForUrl("http://fake_url", _))
      .WillOnce(DoAll(SaveArg<1>(&proxy_callback), Return(true)));
  fetcher->BeginTransfer("http://fake_url");
  testing::Mock::VerifyAndClearExpectations(&mock_resolver);

  // Pausing and unpausing while resolving the proxy should not affect anything.
  fetcher->Pause();
  fetcher->Unpause();
  fetcher->Pause();
  // Proxy resolver comes back after we paused the fetcher.
  ASSERT_FALSE(proxy_callback.is_null());
  proxy_callback.Run({1, kNoProxy});
}

namespace {
class AbortingHttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    return true;
  }
  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    ADD_FAILURE();  // We should never get here
    MessageLoop::current()->BreakLoop();
  }
  void TransferTerminated(HttpFetcher* fetcher) override {
    EXPECT_EQ(fetcher, fetcher_.get());
    EXPECT_FALSE(once_);
    EXPECT_TRUE(callback_once_);
    callback_once_ = false;
    // The fetcher could have a callback scheduled on the ProxyResolver that
    // can fire after this callback. We wait until the end of the test to
    // delete the fetcher.
  }
  void TerminateTransfer() {
    CHECK(once_);
    once_ = false;
    fetcher_->TerminateTransfer();
  }
  void EndLoop() { MessageLoop::current()->BreakLoop(); }
  bool once_;
  bool callback_once_;
  unique_ptr<HttpFetcher> fetcher_;
};

void AbortingTimeoutCallback(AbortingHttpFetcherTestDelegate* delegate,
                             MessageLoop::TaskId* my_id) {
  if (delegate->once_) {
    delegate->TerminateTransfer();
    *my_id = MessageLoop::current()->PostTask(
        FROM_HERE, base::Bind(AbortingTimeoutCallback, delegate, my_id));
  } else {
    delegate->EndLoop();
    *my_id = MessageLoop::kTaskIdNull;
  }
}
}  // namespace

TYPED_TEST(HttpFetcherTest, AbortTest) {
  AbortingHttpFetcherTestDelegate delegate;
  delegate.fetcher_.reset(this->test_.NewLargeFetcher());
  delegate.once_ = true;
  delegate.callback_once_ = true;
  delegate.fetcher_->set_delegate(&delegate);

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  this->test_.IgnoreServerAborting(server.get());
  ASSERT_TRUE(server->started_);

  MessageLoop::TaskId task_id = MessageLoop::kTaskIdNull;

  task_id = this->loop_.PostTask(
      FROM_HERE, base::Bind(AbortingTimeoutCallback, &delegate, &task_id));
  delegate.fetcher_->BeginTransfer(this->test_.BigUrl(server->GetPort()));

  this->loop_.Run();
  CHECK(!delegate.once_);
  CHECK(!delegate.callback_once_);
  this->loop_.CancelTask(task_id);
}

TYPED_TEST(HttpFetcherTest, TerminateTransferWhileResolvingProxyTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;
  MockProxyResolver mock_resolver;
  unique_ptr<HttpFetcher> fetcher(this->test_.NewLargeFetcher(&mock_resolver));

  HttpFetcherTestDelegate delegate;
  fetcher->set_delegate(&delegate);

  EXPECT_CALL(mock_resolver, GetProxiesForUrl(_, _)).WillOnce(Return(123));
  fetcher->BeginTransfer("http://fake_url");
  // Run the message loop until idle. This must call the MockProxyResolver with
  // the request.
  while (this->loop_.RunOnce(false)) {
  }
  testing::Mock::VerifyAndClearExpectations(&mock_resolver);

  EXPECT_CALL(mock_resolver, CancelProxyRequest(123)).WillOnce(Return(true));

  // Terminate the transfer right before the proxy resolution response.
  fetcher->TerminateTransfer();
  EXPECT_EQ(0, delegate.times_received_bytes_called_);
  EXPECT_EQ(0, delegate.times_transfer_complete_called_);
  EXPECT_EQ(1, delegate.times_transfer_terminated_called_);
}

namespace {
class FlakyHttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    data.append(reinterpret_cast<const char*>(bytes), length);
    return true;
  }
  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    EXPECT_TRUE(successful);
    EXPECT_EQ(kHttpResponsePartialContent, fetcher->http_response_code());
    MessageLoop::current()->BreakLoop();
  }
  void TransferTerminated(HttpFetcher* fetcher) override { ADD_FAILURE(); }
  string data;
};
}  // namespace

TYPED_TEST(HttpFetcherTest, FlakyTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;
  {
    FlakyHttpFetcherTestDelegate delegate;
    unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
    fetcher->set_delegate(&delegate);

    unique_ptr<HttpServer> server(this->test_.CreateServer());
    ASSERT_TRUE(server->started_);

    this->loop_.PostTask(FROM_HERE,
                         base::Bind(&StartTransfer,
                                    fetcher.get(),
                                    LocalServerUrlForPath(
                                        server->GetPort(),
                                        base::StringPrintf("/flaky/%d/%d/%d/%d",
                                                           kBigLength,
                                                           kFlakyTruncateLength,
                                                           kFlakySleepEvery,
                                                           kFlakySleepSecs))));
    this->loop_.Run();

    // verify the data we get back
    ASSERT_EQ(kBigLength, static_cast<int>(delegate.data.size()));
    for (int i = 0; i < kBigLength; i += 10) {
      // Assert so that we don't flood the screen w/ EXPECT errors on failure.
      ASSERT_EQ(delegate.data.substr(i, 10), "abcdefghij");
    }
  }
}

namespace {
// This delegate kills the server attached to it after receiving any bytes.
// This can be used for testing what happens when you try to fetch data and
// the server dies.
class FailureHttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  explicit FailureHttpFetcherTestDelegate(PythonHttpServer* server)
      : server_(server) {}

  ~FailureHttpFetcherTestDelegate() override {
    if (server_) {
      LOG(INFO) << "Stopping server in destructor";
      server_.reset();
      LOG(INFO) << "server stopped";
    }
  }

  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    if (server_) {
      LOG(INFO) << "Stopping server in ReceivedBytes";
      server_.reset();
      LOG(INFO) << "server stopped";
    }
    return true;
  }
  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    EXPECT_FALSE(successful);
    EXPECT_EQ(0, fetcher->http_response_code());
    times_transfer_complete_called_++;
    MessageLoop::current()->BreakLoop();
  }
  void TransferTerminated(HttpFetcher* fetcher) override {
    times_transfer_terminated_called_++;
    MessageLoop::current()->BreakLoop();
  }
  unique_ptr<PythonHttpServer> server_;
  int times_transfer_terminated_called_{0};
  int times_transfer_complete_called_{0};
};
}  // namespace

TYPED_TEST(HttpFetcherTest, FailureTest) {
  // This test ensures that a fetcher responds correctly when a server isn't
  // available at all.
  if (this->test_.IsMock())
    return;
  FailureHttpFetcherTestDelegate delegate(nullptr);
  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);

  this->loop_.PostTask(
      FROM_HERE,
      base::Bind(
          StartTransfer, fetcher.get(), "http://host_doesnt_exist99999999"));
  this->loop_.Run();
  EXPECT_EQ(1, delegate.times_transfer_complete_called_);
  EXPECT_EQ(0, delegate.times_transfer_terminated_called_);

  // Exiting and testing happens in the delegate
}

TYPED_TEST(HttpFetcherTest, NoResponseTest) {
  // This test starts a new http server but the server doesn't respond and just
  // closes the connection.
  if (this->test_.IsMock())
    return;

  PythonHttpServer* server = new PythonHttpServer();
  int port = server->GetPort();
  ASSERT_TRUE(server->started_);

  // Handles destruction and claims ownership.
  FailureHttpFetcherTestDelegate delegate(server);
  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);
  // The server will not reply at all, so we can limit the execution time of the
  // test by reducing the low-speed timeout to something small. The test will
  // finish once the TimeoutCallback() triggers (every second) and the timeout
  // expired.
  fetcher->set_low_speed_limit(kDownloadLowSpeedLimitBps, 1);

  this->loop_.PostTask(
      FROM_HERE,
      base::Bind(
          StartTransfer, fetcher.get(), LocalServerUrlForPath(port, "/hang")));
  this->loop_.Run();
  EXPECT_EQ(1, delegate.times_transfer_complete_called_);
  EXPECT_EQ(0, delegate.times_transfer_terminated_called_);

  // Check that no other callback runs in the next two seconds. That would
  // indicate a leaked callback.
  bool timeout = false;
  auto callback = base::Bind([](bool* timeout) { *timeout = true; },
                             base::Unretained(&timeout));
  this->loop_.PostDelayedTask(
      FROM_HERE, callback, base::TimeDelta::FromSeconds(2));
  EXPECT_TRUE(this->loop_.RunOnce(true));
  EXPECT_TRUE(timeout);
}

TYPED_TEST(HttpFetcherTest, ServerDiesTest) {
  // This test starts a new http server and kills it after receiving its first
  // set of bytes. It test whether or not our fetcher eventually gives up on
  // retries and aborts correctly.
  if (this->test_.IsMock())
    return;
  PythonHttpServer* server = new PythonHttpServer();
  int port = server->GetPort();
  ASSERT_TRUE(server->started_);

  // Handles destruction and claims ownership.
  FailureHttpFetcherTestDelegate delegate(server);
  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);

  this->loop_.PostTask(
      FROM_HERE,
      base::Bind(StartTransfer,
                 fetcher.get(),
                 LocalServerUrlForPath(port,
                                       base::StringPrintf("/flaky/%d/%d/%d/%d",
                                                          kBigLength,
                                                          kFlakyTruncateLength,
                                                          kFlakySleepEvery,
                                                          kFlakySleepSecs))));
  this->loop_.Run();
  EXPECT_EQ(1, delegate.times_transfer_complete_called_);
  EXPECT_EQ(0, delegate.times_transfer_terminated_called_);

  // Exiting and testing happens in the delegate
}

// Test that we can cancel a transfer while it is still trying to connect to the
// server. This test kills the server after a few bytes are received.
TYPED_TEST(HttpFetcherTest, TerminateTransferWhenServerDiedTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;

  PythonHttpServer* server = new PythonHttpServer();
  int port = server->GetPort();
  ASSERT_TRUE(server->started_);

  // Handles destruction and claims ownership.
  FailureHttpFetcherTestDelegate delegate(server);
  unique_ptr<HttpFetcher> fetcher(this->test_.NewSmallFetcher());
  fetcher->set_delegate(&delegate);

  this->loop_.PostTask(
      FROM_HERE,
      base::Bind(StartTransfer,
                 fetcher.get(),
                 LocalServerUrlForPath(port,
                                       base::StringPrintf("/flaky/%d/%d/%d/%d",
                                                          kBigLength,
                                                          kFlakyTruncateLength,
                                                          kFlakySleepEvery,
                                                          kFlakySleepSecs))));
  // Terminating the transfer after 3 seconds gives it a chance to contact the
  // server and enter the retry loop.
  this->loop_.PostDelayedTask(FROM_HERE,
                              base::Bind(&HttpFetcher::TerminateTransfer,
                                         base::Unretained(fetcher.get())),
                              base::TimeDelta::FromSeconds(3));

  // Exiting and testing happens in the delegate.
  this->loop_.Run();
  EXPECT_EQ(0, delegate.times_transfer_complete_called_);
  EXPECT_EQ(1, delegate.times_transfer_terminated_called_);

  // Check that no other callback runs in the next two seconds. That would
  // indicate a leaked callback.
  bool timeout = false;
  auto callback = base::Bind([](bool* timeout) { *timeout = true; },
                             base::Unretained(&timeout));
  this->loop_.PostDelayedTask(
      FROM_HERE, callback, base::TimeDelta::FromSeconds(2));
  EXPECT_TRUE(this->loop_.RunOnce(true));
  EXPECT_TRUE(timeout);
}

namespace {
const HttpResponseCode kRedirectCodes[] = {kHttpResponseMovedPermanently,
                                           kHttpResponseFound,
                                           kHttpResponseSeeOther,
                                           kHttpResponseTempRedirect};

class RedirectHttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  explicit RedirectHttpFetcherTestDelegate(bool expected_successful)
      : expected_successful_(expected_successful) {}
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    data.append(reinterpret_cast<const char*>(bytes), length);
    return true;
  }
  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    EXPECT_EQ(expected_successful_, successful);
    if (expected_successful_) {
      EXPECT_EQ(kHttpResponseOk, fetcher->http_response_code());
    } else {
      EXPECT_GE(fetcher->http_response_code(), kHttpResponseMovedPermanently);
      EXPECT_LE(fetcher->http_response_code(), kHttpResponseTempRedirect);
    }
    MessageLoop::current()->BreakLoop();
  }
  void TransferTerminated(HttpFetcher* fetcher) override { ADD_FAILURE(); }
  bool expected_successful_;
  string data;
};

// RedirectTest takes ownership of |http_fetcher|.
void RedirectTest(const HttpServer* server,
                  bool expected_successful,
                  const string& url,
                  HttpFetcher* http_fetcher) {
  RedirectHttpFetcherTestDelegate delegate(expected_successful);
  unique_ptr<HttpFetcher> fetcher(http_fetcher);
  fetcher->set_delegate(&delegate);

  MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(StartTransfer,
                 fetcher.get(),
                 LocalServerUrlForPath(server->GetPort(), url)));
  MessageLoop::current()->Run();
  if (expected_successful) {
    // verify the data we get back
    ASSERT_EQ(static_cast<size_t>(kMediumLength), delegate.data.size());
    for (int i = 0; i < kMediumLength; i += 10) {
      // Assert so that we don't flood the screen w/ EXPECT errors on failure.
      ASSERT_EQ(delegate.data.substr(i, 10), "abcdefghij");
    }
  }
}
}  // namespace

TYPED_TEST(HttpFetcherTest, SimpleRedirectTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  for (size_t c = 0; c < arraysize(kRedirectCodes); ++c) {
    const string url = base::StringPrintf(
        "/redirect/%d/download/%d", kRedirectCodes[c], kMediumLength);
    RedirectTest(server.get(), true, url, this->test_.NewLargeFetcher());
  }
}

TYPED_TEST(HttpFetcherTest, MaxRedirectTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  string url;
  for (int r = 0; r < kDownloadMaxRedirects; r++) {
    url += base::StringPrintf("/redirect/%d",
                              kRedirectCodes[r % arraysize(kRedirectCodes)]);
  }
  url += base::StringPrintf("/download/%d", kMediumLength);
  RedirectTest(server.get(), true, url, this->test_.NewLargeFetcher());
}

TYPED_TEST(HttpFetcherTest, BeyondMaxRedirectTest) {
  if (this->test_.IsMock() || !this->test_.IsHttpSupported())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  string url;
  for (int r = 0; r < kDownloadMaxRedirects + 1; r++) {
    url += base::StringPrintf("/redirect/%d",
                              kRedirectCodes[r % arraysize(kRedirectCodes)]);
  }
  url += base::StringPrintf("/download/%d", kMediumLength);
  RedirectTest(server.get(), false, url, this->test_.NewLargeFetcher());
}

namespace {
class MultiHttpFetcherTestDelegate : public HttpFetcherDelegate {
 public:
  explicit MultiHttpFetcherTestDelegate(int expected_response_code)
      : expected_response_code_(expected_response_code) {}

  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    EXPECT_EQ(fetcher, fetcher_.get());
    data.append(reinterpret_cast<const char*>(bytes), length);
    return true;
  }

  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    EXPECT_EQ(fetcher, fetcher_.get());
    EXPECT_EQ(expected_response_code_ != kHttpResponseUndefined, successful);
    if (expected_response_code_ != 0)
      EXPECT_EQ(expected_response_code_, fetcher->http_response_code());
    // Destroy the fetcher (because we're allowed to).
    fetcher_.reset(nullptr);
    MessageLoop::current()->BreakLoop();
  }

  void TransferTerminated(HttpFetcher* fetcher) override { ADD_FAILURE(); }

  unique_ptr<HttpFetcher> fetcher_;
  int expected_response_code_;
  string data;
};

void MultiTest(HttpFetcher* fetcher_in,
               FakeHardware* fake_hardware,
               const string& url,
               const vector<pair<off_t, off_t>>& ranges,
               const string& expected_prefix,
               size_t expected_size,
               HttpResponseCode expected_response_code) {
  MultiHttpFetcherTestDelegate delegate(expected_response_code);
  delegate.fetcher_.reset(fetcher_in);

  MultiRangeHttpFetcher* multi_fetcher =
      static_cast<MultiRangeHttpFetcher*>(fetcher_in);
  ASSERT_TRUE(multi_fetcher);
  multi_fetcher->ClearRanges();
  for (vector<pair<off_t, off_t>>::const_iterator it = ranges.begin(),
                                                  e = ranges.end();
       it != e;
       ++it) {
    string tmp_str = base::StringPrintf("%jd+", it->first);
    if (it->second > 0) {
      base::StringAppendF(&tmp_str, "%jd", it->second);
      multi_fetcher->AddRange(it->first, it->second);
    } else {
      base::StringAppendF(&tmp_str, "?");
      multi_fetcher->AddRange(it->first);
    }
    LOG(INFO) << "added range: " << tmp_str;
  }
  fake_hardware->SetIsOfficialBuild(false);
  multi_fetcher->set_delegate(&delegate);

  MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(StartTransfer, multi_fetcher, url));
  MessageLoop::current()->Run();

  EXPECT_EQ(expected_size, delegate.data.size());
  EXPECT_EQ(expected_prefix,
            string(delegate.data.data(), expected_prefix.size()));
}
}  // namespace

TYPED_TEST(HttpFetcherTest, MultiHttpFetcherSimpleTest) {
  if (!this->test_.IsMulti())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(0, 25));
  ranges.push_back(make_pair(99, 17));
  MultiTest(this->test_.NewLargeFetcher(),
            this->test_.fake_hardware(),
            this->test_.BigUrl(server->GetPort()),
            ranges,
            "abcdefghijabcdefghijabcdejabcdefghijabcdef",
            25 + 17,
            this->test_.IsFileFetcher() ? kHttpResponseOk
                                        : kHttpResponsePartialContent);
}

TYPED_TEST(HttpFetcherTest, MultiHttpFetcherUnspecifiedEndTest) {
  if (!this->test_.IsMulti() || this->test_.IsFileFetcher())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(0, 25));
  ranges.push_back(make_pair(99, 0));
  MultiTest(this->test_.NewLargeFetcher(),
            this->test_.fake_hardware(),
            this->test_.BigUrl(server->GetPort()),
            ranges,
            "abcdefghijabcdefghijabcdejabcdefghijabcdef",
            kBigLength - (99 - 25),
            kHttpResponsePartialContent);
}

TYPED_TEST(HttpFetcherTest, MultiHttpFetcherLengthLimitTest) {
  if (!this->test_.IsMulti())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(0, 24));
  MultiTest(this->test_.NewLargeFetcher(),
            this->test_.fake_hardware(),
            this->test_.BigUrl(server->GetPort()),
            ranges,
            "abcdefghijabcdefghijabcd",
            24,
            this->test_.IsFileFetcher() ? kHttpResponseOk
                                        : kHttpResponsePartialContent);
}

TYPED_TEST(HttpFetcherTest, MultiHttpFetcherMultiEndTest) {
  if (!this->test_.IsMulti() || this->test_.IsFileFetcher())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(kBigLength - 2, 0));
  ranges.push_back(make_pair(kBigLength - 3, 0));
  MultiTest(this->test_.NewLargeFetcher(),
            this->test_.fake_hardware(),
            this->test_.BigUrl(server->GetPort()),
            ranges,
            "ijhij",
            5,
            kHttpResponsePartialContent);
}

TYPED_TEST(HttpFetcherTest, MultiHttpFetcherInsufficientTest) {
  if (!this->test_.IsMulti())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(kBigLength - 2, 4));
  for (int i = 0; i < 2; ++i) {
    LOG(INFO) << "i = " << i;
    MultiTest(this->test_.NewLargeFetcher(),
              this->test_.fake_hardware(),
              this->test_.BigUrl(server->GetPort()),
              ranges,
              "ij",
              2,
              kHttpResponseUndefined);
    ranges.push_back(make_pair(0, 5));
  }
}

// Issue #18143: when a fetch of a secondary chunk out of a chain, then it
// should retry with other proxies listed before giving up.
//
// (1) successful recovery: The offset fetch will fail twice but succeed with
// the third proxy.
TYPED_TEST(HttpFetcherTest, MultiHttpFetcherErrorIfOffsetRecoverableTest) {
  if (!this->test_.IsMulti() || this->test_.IsFileFetcher())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(0, 25));
  ranges.push_back(make_pair(99, 0));
  MultiTest(this->test_.NewLargeFetcher(3),
            this->test_.fake_hardware(),
            LocalServerUrlForPath(
                server->GetPort(),
                base::StringPrintf("/error-if-offset/%d/2", kBigLength)),
            ranges,
            "abcdefghijabcdefghijabcdejabcdefghijabcdef",
            kBigLength - (99 - 25),
            kHttpResponsePartialContent);
}

// (2) unsuccessful recovery: The offset fetch will fail repeatedly.  The
// fetcher will signal a (failed) completed transfer to the delegate.
TYPED_TEST(HttpFetcherTest, MultiHttpFetcherErrorIfOffsetUnrecoverableTest) {
  if (!this->test_.IsMulti() || this->test_.IsFileFetcher())
    return;

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  vector<pair<off_t, off_t>> ranges;
  ranges.push_back(make_pair(0, 25));
  ranges.push_back(make_pair(99, 0));
  MultiTest(this->test_.NewLargeFetcher(2),
            this->test_.fake_hardware(),
            LocalServerUrlForPath(
                server->GetPort(),
                base::StringPrintf("/error-if-offset/%d/3", kBigLength)),
            ranges,
            "abcdefghijabcdefghijabcde",  // only received the first chunk
            25,
            kHttpResponseUndefined);
}

namespace {
// This HttpFetcherDelegate calls TerminateTransfer at a configurable point.
class MultiHttpFetcherTerminateTestDelegate : public HttpFetcherDelegate {
 public:
  explicit MultiHttpFetcherTerminateTestDelegate(size_t terminate_trigger_bytes)
      : terminate_trigger_bytes_(terminate_trigger_bytes) {}

  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    LOG(INFO) << "ReceivedBytes, " << length << " bytes.";
    EXPECT_EQ(fetcher, fetcher_.get());
    bool should_terminate = false;
    if (bytes_downloaded_ < terminate_trigger_bytes_ &&
        bytes_downloaded_ + length >= terminate_trigger_bytes_) {
      MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&HttpFetcher::TerminateTransfer,
                     base::Unretained(fetcher_.get())));
      should_terminate = true;
    }
    bytes_downloaded_ += length;
    return !should_terminate;
  }

  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    ADD_FAILURE() << "TransferComplete called but expected a failure";
    // Destroy the fetcher (because we're allowed to).
    fetcher_.reset(nullptr);
    MessageLoop::current()->BreakLoop();
  }

  void TransferTerminated(HttpFetcher* fetcher) override {
    // Destroy the fetcher (because we're allowed to).
    fetcher_.reset(nullptr);
    MessageLoop::current()->BreakLoop();
  }

  unique_ptr<HttpFetcher> fetcher_;
  size_t bytes_downloaded_{0};
  size_t terminate_trigger_bytes_;
};
}  // namespace

TYPED_TEST(HttpFetcherTest, MultiHttpFetcherTerminateBetweenRangesTest) {
  if (!this->test_.IsMulti())
    return;
  const size_t kRangeTrigger = 1000;
  MultiHttpFetcherTerminateTestDelegate delegate(kRangeTrigger);

  unique_ptr<HttpServer> server(this->test_.CreateServer());
  ASSERT_TRUE(server->started_);

  MultiRangeHttpFetcher* multi_fetcher =
      static_cast<MultiRangeHttpFetcher*>(this->test_.NewLargeFetcher());
  ASSERT_TRUE(multi_fetcher);
  // Transfer ownership of the fetcher to the delegate.
  delegate.fetcher_.reset(multi_fetcher);
  multi_fetcher->set_delegate(&delegate);

  multi_fetcher->ClearRanges();
  multi_fetcher->AddRange(45, kRangeTrigger);
  multi_fetcher->AddRange(2000, 100);

  this->test_.fake_hardware()->SetIsOfficialBuild(false);

  StartTransfer(multi_fetcher, this->test_.BigUrl(server->GetPort()));
  MessageLoop::current()->Run();

  // Check that the delegate made it to the trigger point.
  EXPECT_EQ(kRangeTrigger, delegate.bytes_downloaded_);
}

namespace {
class BlockedTransferTestDelegate : public HttpFetcherDelegate {
 public:
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override {
    ADD_FAILURE();
    return true;
  }
  void TransferComplete(HttpFetcher* fetcher, bool successful) override {
    EXPECT_FALSE(successful);
    MessageLoop::current()->BreakLoop();
  }
  void TransferTerminated(HttpFetcher* fetcher) override { ADD_FAILURE(); }
};

void BlockedTransferTestHelper(AnyHttpFetcherTest* fetcher_test,
                               bool is_official_build) {
  if (fetcher_test->IsMock() || fetcher_test->IsMulti())
    return;

  unique_ptr<HttpServer> server(fetcher_test->CreateServer());
  ASSERT_TRUE(server->started_);

  BlockedTransferTestDelegate delegate;
  unique_ptr<HttpFetcher> fetcher(fetcher_test->NewLargeFetcher());
  LOG(INFO) << "is_official_build: " << is_official_build;
  // NewLargeFetcher creates the HttpFetcher* with a FakeSystemState.
  fetcher_test->fake_hardware()->SetIsOfficialBuild(is_official_build);
  fetcher->set_delegate(&delegate);

  MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(
          StartTransfer,
          fetcher.get(),
          LocalServerUrlForPath(server->GetPort(),
                                fetcher_test->SmallUrl(server->GetPort()))));
  MessageLoop::current()->Run();
}
}  // namespace

TYPED_TEST(HttpFetcherTest, BlockedTransferTest) {
  BlockedTransferTestHelper(&this->test_, false);
}

TYPED_TEST(HttpFetcherTest, BlockedTransferOfficialBuildTest) {
  BlockedTransferTestHelper(&this->test_, true);
}

}  // namespace chromeos_update_engine
