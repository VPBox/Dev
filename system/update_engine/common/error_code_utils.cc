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

#include "update_engine/common/error_code_utils.h"

#include <base/strings/string_number_conversions.h>

using std::string;

namespace chromeos_update_engine {
namespace utils {

string ErrorCodeToString(ErrorCode code) {
  // If the given code has both parts (i.e. the error code part and the flags
  // part) then strip off the flags part since the switch statement below
  // has case statements only for the base error code or a single flag but
  // doesn't support any combinations of those.
  if ((static_cast<int>(code) & static_cast<int>(ErrorCode::kSpecialFlags)) &&
      (static_cast<int>(code) & ~static_cast<int>(ErrorCode::kSpecialFlags)))
    code = static_cast<ErrorCode>(static_cast<int>(code) &
                                  ~static_cast<int>(ErrorCode::kSpecialFlags));
  switch (code) {
    case ErrorCode::kSuccess:
      return "ErrorCode::kSuccess";
    case ErrorCode::kError:
      return "ErrorCode::kError";
    case ErrorCode::kOmahaRequestError:
      return "ErrorCode::kOmahaRequestError";
    case ErrorCode::kOmahaResponseHandlerError:
      return "ErrorCode::kOmahaResponseHandlerError";
    case ErrorCode::kFilesystemCopierError:
      return "ErrorCode::kFilesystemCopierError";
    case ErrorCode::kPostinstallRunnerError:
      return "ErrorCode::kPostinstallRunnerError";
    case ErrorCode::kPayloadMismatchedType:
      return "ErrorCode::kPayloadMismatchedType";
    case ErrorCode::kInstallDeviceOpenError:
      return "ErrorCode::kInstallDeviceOpenError";
    case ErrorCode::kKernelDeviceOpenError:
      return "ErrorCode::kKernelDeviceOpenError";
    case ErrorCode::kDownloadTransferError:
      return "ErrorCode::kDownloadTransferError";
    case ErrorCode::kPayloadHashMismatchError:
      return "ErrorCode::kPayloadHashMismatchError";
    case ErrorCode::kPayloadSizeMismatchError:
      return "ErrorCode::kPayloadSizeMismatchError";
    case ErrorCode::kDownloadPayloadVerificationError:
      return "ErrorCode::kDownloadPayloadVerificationError";
    case ErrorCode::kDownloadNewPartitionInfoError:
      return "ErrorCode::kDownloadNewPartitionInfoError";
    case ErrorCode::kDownloadWriteError:
      return "ErrorCode::kDownloadWriteError";
    case ErrorCode::kNewRootfsVerificationError:
      return "ErrorCode::kNewRootfsVerificationError";
    case ErrorCode::kNewKernelVerificationError:
      return "ErrorCode::kNewKernelVerificationError";
    case ErrorCode::kSignedDeltaPayloadExpectedError:
      return "ErrorCode::kSignedDeltaPayloadExpectedError";
    case ErrorCode::kDownloadPayloadPubKeyVerificationError:
      return "ErrorCode::kDownloadPayloadPubKeyVerificationError";
    case ErrorCode::kPostinstallBootedFromFirmwareB:
      return "ErrorCode::kPostinstallBootedFromFirmwareB";
    case ErrorCode::kDownloadStateInitializationError:
      return "ErrorCode::kDownloadStateInitializationError";
    case ErrorCode::kDownloadInvalidMetadataMagicString:
      return "ErrorCode::kDownloadInvalidMetadataMagicString";
    case ErrorCode::kDownloadSignatureMissingInManifest:
      return "ErrorCode::kDownloadSignatureMissingInManifest";
    case ErrorCode::kDownloadManifestParseError:
      return "ErrorCode::kDownloadManifestParseError";
    case ErrorCode::kDownloadMetadataSignatureError:
      return "ErrorCode::kDownloadMetadataSignatureError";
    case ErrorCode::kDownloadMetadataSignatureVerificationError:
      return "ErrorCode::kDownloadMetadataSignatureVerificationError";
    case ErrorCode::kDownloadMetadataSignatureMismatch:
      return "ErrorCode::kDownloadMetadataSignatureMismatch";
    case ErrorCode::kDownloadOperationHashVerificationError:
      return "ErrorCode::kDownloadOperationHashVerificationError";
    case ErrorCode::kDownloadOperationExecutionError:
      return "ErrorCode::kDownloadOperationExecutionError";
    case ErrorCode::kDownloadOperationHashMismatch:
      return "ErrorCode::kDownloadOperationHashMismatch";
    case ErrorCode::kOmahaRequestEmptyResponseError:
      return "ErrorCode::kOmahaRequestEmptyResponseError";
    case ErrorCode::kOmahaRequestXMLParseError:
      return "ErrorCode::kOmahaRequestXMLParseError";
    case ErrorCode::kDownloadInvalidMetadataSize:
      return "ErrorCode::kDownloadInvalidMetadataSize";
    case ErrorCode::kDownloadInvalidMetadataSignature:
      return "ErrorCode::kDownloadInvalidMetadataSignature";
    case ErrorCode::kOmahaResponseInvalid:
      return "ErrorCode::kOmahaResponseInvalid";
    case ErrorCode::kOmahaUpdateIgnoredPerPolicy:
      return "ErrorCode::kOmahaUpdateIgnoredPerPolicy";
    case ErrorCode::kOmahaUpdateDeferredPerPolicy:
      return "ErrorCode::kOmahaUpdateDeferredPerPolicy";
    case ErrorCode::kOmahaErrorInHTTPResponse:
      return "ErrorCode::kOmahaErrorInHTTPResponse";
    case ErrorCode::kDownloadOperationHashMissingError:
      return "ErrorCode::kDownloadOperationHashMissingError";
    case ErrorCode::kDownloadMetadataSignatureMissingError:
      return "ErrorCode::kDownloadMetadataSignatureMissingError";
    case ErrorCode::kOmahaUpdateDeferredForBackoff:
      return "ErrorCode::kOmahaUpdateDeferredForBackoff";
    case ErrorCode::kPostinstallPowerwashError:
      return "ErrorCode::kPostinstallPowerwashError";
    case ErrorCode::kUpdateCanceledByChannelChange:
      return "ErrorCode::kUpdateCanceledByChannelChange";
    case ErrorCode::kUmaReportedMax:
      return "ErrorCode::kUmaReportedMax";
    case ErrorCode::kOmahaRequestHTTPResponseBase:
      return "ErrorCode::kOmahaRequestHTTPResponseBase";
    case ErrorCode::kResumedFlag:
      return "Resumed";
    case ErrorCode::kDevModeFlag:
      return "DevMode";
    case ErrorCode::kTestImageFlag:
      return "TestImage";
    case ErrorCode::kTestOmahaUrlFlag:
      return "TestOmahaUrl";
    case ErrorCode::kSpecialFlags:
      return "ErrorCode::kSpecialFlags";
    case ErrorCode::kPostinstallFirmwareRONotUpdatable:
      return "ErrorCode::kPostinstallFirmwareRONotUpdatable";
    case ErrorCode::kUnsupportedMajorPayloadVersion:
      return "ErrorCode::kUnsupportedMajorPayloadVersion";
    case ErrorCode::kUnsupportedMinorPayloadVersion:
      return "ErrorCode::kUnsupportedMinorPayloadVersion";
    case ErrorCode::kOmahaRequestXMLHasEntityDecl:
      return "ErrorCode::kOmahaRequestXMLHasEntityDecl";
    case ErrorCode::kFilesystemVerifierError:
      return "ErrorCode::kFilesystemVerifierError";
    case ErrorCode::kUserCanceled:
      return "ErrorCode::kUserCanceled";
    case ErrorCode::kNonCriticalUpdateInOOBE:
      return "ErrorCode::kNonCriticalUpdateInOOBE";
    case ErrorCode::kOmahaUpdateIgnoredOverCellular:
      return "ErrorCode::kOmahaUpdateIgnoredOverCellular";
    case ErrorCode::kPayloadTimestampError:
      return "ErrorCode::kPayloadTimestampError";
    case ErrorCode::kUpdatedButNotActive:
      return "ErrorCode::kUpdatedButNotActive";
    case ErrorCode::kNoUpdate:
      return "ErrorCode::kNoUpdate";
    case ErrorCode::kRollbackNotPossible:
      return "ErrorCode::kRollbackNotPossible";
    case ErrorCode::kFirstActiveOmahaPingSentPersistenceError:
      return "ErrorCode::kFirstActiveOmahaPingSentPersistenceError";
    case ErrorCode::kVerityCalculationError:
      return "ErrorCode::kVerityCalculationError";
      // Don't add a default case to let the compiler warn about newly added
      // error codes which should be added here.
  }

  return "Unknown error: " + base::UintToString(static_cast<unsigned>(code));
}

}  // namespace utils
}  // namespace chromeos_update_engine
