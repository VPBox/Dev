//
// Copyright (C) 2011 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_CERTIFICATE_CHECKER_H_
#define UPDATE_ENGINE_CERTIFICATE_CHECKER_H_

#include <curl/curl.h>
#include <openssl/ssl.h>

#include <string>

#include <base/macros.h>
#include <gtest/gtest_prod.h>  // for FRIEND_TEST

namespace chromeos_update_engine {

class PrefsInterface;

// Wrapper for openssl operations with the certificates.
class OpenSSLWrapper {
 public:
  OpenSSLWrapper() = default;
  virtual ~OpenSSLWrapper() = default;

  // Takes an openssl X509_STORE_CTX, extracts the corresponding certificate
  // from it and calculates its fingerprint (SHA256 digest). Returns true on
  // success and false otherwise.
  //
  // |x509_ctx| is the pointer to the openssl object that holds the certificate.
  // |out_depth| is the depth of the current certificate, in the certificate
  // chain.
  // |out_digest_length| is the length of the generated digest.
  // |out_digest| is the byte array where the digest itself will be written.
  // It should be big enough to hold a SHA1 digest (e.g. EVP_MAX_MD_SIZE).
  virtual bool GetCertificateDigest(X509_STORE_CTX* x509_ctx,
                                    int* out_depth,
                                    unsigned int* out_digest_length,
                                    uint8_t* out_digest) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(OpenSSLWrapper);
};

// The values in this enum are replicated in the metrics server. See metrics.h
// for instructions on how to update these values in the server side.
enum class CertificateCheckResult {
  // The certificate is valid and the same as seen before or the first time we
  // see a certificate.
  kValid,
  // The certificate is valid, but is different than a previously seen
  // certificate for the selected server.
  kValidChanged,
  // The certificate validation failed.
  kFailed,

  // This value must be the last entry.
  kNumConstants
};

// These values are used to generate the keys of files persisted via prefs.
// This means that changing these will cause loss of information on metrics
// reporting, during the transition. These values are also mapped to a metric
// name in metrics.cc, so adding values here requires to assign a new metric
// name in that file.
enum class ServerToCheck {
  kUpdate = 0,
  kDownload,

  // Ignore this server.
  kNone,
};

// Responsible for checking whether update server certificates change, and
// reporting to UMA when this happens. Since all state information is persisted,
// and openssl forces us to use a static callback with no data pointer, this
// class is entirely static.
class CertificateChecker {
 public:
  class Observer {
   public:
    virtual ~Observer() = default;

    // Called whenever a certificate is checked for the server |server_to_check|
    // passing the result of said certificate check.
    virtual void CertificateChecked(ServerToCheck server_to_check,
                                    CertificateCheckResult result) = 0;
  };

  CertificateChecker(PrefsInterface* prefs, OpenSSLWrapper* openssl_wrapper);
  ~CertificateChecker();

  // This callback is called by libcurl just before the initialization of an
  // SSL connection after having processed all other SSL related options. Used
  // to check if server certificates change. |cert_checker| is expected to be a
  // pointer to the CertificateChecker instance.
  static CURLcode ProcessSSLContext(CURL* curl_handle,
                                    SSL_CTX* ssl_ctx,
                                    void* cert_checker);

  // Initialize this class as the singleton instance. Only one instance can be
  // initialized at a time and it should be initialized before other methods
  // can be used.
  void Init();

  // Set the certificate observer to the passed instance. To remove the
  // observer, pass a nullptr. The |observer| instance must be valid while this
  // CertificateChecker verifies certificates.
  void SetObserver(Observer* observer) { observer_ = observer; }

 private:
  FRIEND_TEST(CertificateCheckerTest, NewCertificate);
  FRIEND_TEST(CertificateCheckerTest, SameCertificate);
  FRIEND_TEST(CertificateCheckerTest, ChangedCertificate);
  FRIEND_TEST(CertificateCheckerTest, FailedCertificate);

  // These callbacks are asynchronously called by openssl after initial SSL
  // verification. They are used to perform any additional security verification
  // on the connection, but we use them here to get hold of the server
  // certificate, in order to determine if it has changed since the last
  // connection. Since openssl forces us to do this statically, we define two
  // different callbacks for the two different official update servers, and only
  // assign the correspondent one. The assigned callback is then called once per
  // each certificate on the server and returns 1 for success and 0 for failure.
  static int VerifySSLCallbackDownload(int preverify_ok,
                                       X509_STORE_CTX* x509_ctx);
  static int VerifySSLCallbackUpdate(int preverify_ok,
                                     X509_STORE_CTX* x509_ctx);
  static int VerifySSLCallback(int preverify_ok,
                               X509_STORE_CTX* x509_ctx,
                               ServerToCheck server_to_check);

  // Checks if server certificate stored in |x509_ctx| has changed since last
  // connection to that same server, specified by |server_to_check|.
  // This is called by the callbacks defined above. The result of the
  // certificate check is passed to the observer, if any. Returns true on
  // success and false otherwise.
  bool CheckCertificateChange(int preverify_ok,
                              X509_STORE_CTX* x509_ctx,
                              ServerToCheck server_to_check);

  // Notifies the observer, if any, of a certificate checking.
  void NotifyCertificateChecked(ServerToCheck server_to_check,
                                CertificateCheckResult result);

  // The CertificateChecker singleton instance.
  static CertificateChecker* cert_checker_singleton_;

  // Prefs instance used to store the certificates seen in the past.
  PrefsInterface* prefs_;

  // The wrapper for openssl operations.
  OpenSSLWrapper* openssl_wrapper_;

  // The observer called whenever a certificate is checked, if not null.
  Observer* observer_{nullptr};

  DISALLOW_COPY_AND_ASSIGN(CertificateChecker);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_CERTIFICATE_CHECKER_H_
