#ifndef WIRELESS_ANDROID_LOGGING_AWP_PERF_DATA_CONVERTER_H_
#define WIRELESS_ANDROID_LOGGING_AWP_PERF_DATA_CONVERTER_H_

#include <string>

#include "perfprofd_record-fwd.h"

namespace perfprofd {
struct Symbolizer;
}  // namespace perfprofd

namespace android {
namespace perfprofd {

PerfprofdRecord*
RawPerfDataToAndroidPerfProfile(const std::string &perf_file,
                                ::perfprofd::Symbolizer* symbolizer,
                                bool symbolize_everything);

}  // namespace perfprofd
}  // namespace android

#endif  // WIRELESS_ANDROID_LOGGING_AWP_PERF_DATA_CONVERTER_H_
