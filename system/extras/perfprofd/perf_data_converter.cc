
#include "perf_data_converter.h"

#include <algorithm>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/strings.h>
#include <perf_data_utils.h>
#include <perf_parser.h>
#include <perf_protobuf_io.h>

#include "perfprofd_record.pb.h"
#include "perf_data.pb.h"

#include "map_utils.h"
#include "quipper_helper.h"
#include "symbolizer.h"

using std::map;

namespace android {
namespace perfprofd {

namespace {

void AddSymbolInfo(PerfprofdRecord* record,
                   ::quipper::PerfParser& perf_parser,
                   ::perfprofd::Symbolizer* symbolizer,
                   bool symbolize_everything) {
  std::unordered_set<std::string> filenames_w_build_id;
  if (!symbolize_everything) {
    for (auto& perf_build_id : record->build_ids()) {
      filenames_w_build_id.insert(perf_build_id.filename());
    }
  }

  std::unordered_set<std::string> files_wo_build_id;
  {
    quipper::MmapEventIterator it(*record);
    for (; it != it.end(); ++it) {
      const ::quipper::PerfDataProto_MMapEvent* mmap_event = &it->mmap_event();
      if (!mmap_event->has_filename() || !mmap_event->has_start() || !mmap_event->has_len()) {
        // Don't care.
        continue;
      }
      if (filenames_w_build_id.count(mmap_event->filename()) == 0) {
        files_wo_build_id.insert(mmap_event->filename());
      }
    }
  }
  if (files_wo_build_id.empty()) {
    return;
  }

  struct Dso {
    uint64_t min_vaddr;
    RangeMap<std::string, uint64_t> symbols;
    explicit Dso(uint64_t min_vaddr_in) : min_vaddr(min_vaddr_in) {
    }
  };
  std::unordered_map<std::string, Dso> files;

  auto it = record->events().begin();
  auto end = record->events().end();
  auto parsed_it = perf_parser.parsed_events().begin();
  auto parsed_end = perf_parser.parsed_events().end();
  for (; it != end; ++it, ++parsed_it) {
    CHECK(parsed_it != parsed_end);
    if (!it->has_sample_event()) {
      continue;
    }

    const ::quipper::PerfDataProto_SampleEvent& sample_event = it->sample_event();

    if (android::base::kEnableDChecks) {
      // Check that the parsed_event and sample_event are consistent.
      CHECK_EQ(parsed_it->callchain.size(), sample_event.callchain_size());
    }

    auto check_address = [&](const std::string& dso_name, uint64_t offset) {
      if (files_wo_build_id.count(dso_name) == 0) {
        return;
      }

      // OK, that's a hit in the mmap segment (w/o build id).

      Dso* dso_data;
      {
        auto dso_it = files.find(dso_name);
        constexpr uint64_t kNoMinAddr = std::numeric_limits<uint64_t>::max();
        if (dso_it == files.end()) {
          uint64_t min_vaddr;
          bool has_min_vaddr = symbolizer->GetMinExecutableVAddr(dso_name, &min_vaddr);
          if (!has_min_vaddr) {
            min_vaddr = kNoMinAddr;
          }
          auto it = files.emplace(dso_name, Dso(min_vaddr));
          dso_data = &it.first->second;
        } else {
          dso_data = &dso_it->second;
        }
        if (dso_data->min_vaddr == kNoMinAddr) {
          return;
        }
      }

      // TODO: Is min_vaddr necessary here?
      const uint64_t file_addr = offset;

      std::string symbol = symbolizer->Decode(dso_name, file_addr);
      if (symbol.empty()) {
        return;
      }

      dso_data->symbols.Insert(symbol, file_addr);
    };
    if (sample_event.has_ip() && parsed_it->dso_and_offset.dso_info_ != nullptr) {
      check_address(parsed_it->dso_and_offset.dso_info_->name, parsed_it->dso_and_offset.offset_);
    }
    if (sample_event.callchain_size() > 0) {
      for (auto& callchain_data: parsed_it->callchain) {
        if (callchain_data.dso_info_ == nullptr) {
          continue;
        }
        check_address(callchain_data.dso_info_->name, callchain_data.offset_);
      }
    }
  }

  if (!files.empty()) {
    // We have extra symbol info, create proto messages now.
    size_t symbol_info_index = 0;
    for (auto& file_data : files) {
      const std::string& filename = file_data.first;
      const Dso& dso = file_data.second;
      if (dso.symbols.empty()) {
        continue;
      }

      auto* symbol_info = record->AddExtension(::quipper::symbol_info);
      symbol_info->set_filename(filename);
      symbol_info->set_filename_md5_prefix(::quipper::Md5Prefix(filename));
      symbol_info->set_min_vaddr(dso.min_vaddr);
      for (auto& aggr_sym : dso.symbols) {
        auto* symbol = symbol_info->add_symbols();
        symbol->set_addr(*aggr_sym.second.offsets.begin());
        symbol->set_size(*aggr_sym.second.offsets.rbegin() - *aggr_sym.second.offsets.begin() + 1);
        symbol->set_name(aggr_sym.second.symbol);
        symbol->set_name_md5_prefix(::quipper::Md5Prefix(aggr_sym.second.symbol));
      }

      ++symbol_info_index;
    }
  }
}

}  // namespace

PerfprofdRecord*
RawPerfDataToAndroidPerfProfile(const string &perf_file,
                                ::perfprofd::Symbolizer* symbolizer,
                                 bool symbolize_everything) {
  std::unique_ptr<PerfprofdRecord> ret(new PerfprofdRecord());
  ret->SetExtension(::quipper::id, 0);  // TODO.

  ::quipper::PerfParserOptions options = {};
  options.do_remap = true;
  options.discard_unused_events = true;
  options.read_missing_buildids = true;

  ::quipper::PerfReader reader;
  if (!reader.ReadFile(perf_file)) return nullptr;

  ::quipper::PerfParser parser(&reader, options);
  if (!parser.ParseRawEvents()) return nullptr;

  if (!reader.Serialize(ret.get())) return nullptr;

  // Append parser stats to protobuf.
  ::quipper::PerfSerializer::SerializeParserStats(parser.stats(), ret.get());

  // TODO: Symbolization.
  if (symbolizer != nullptr) {
    AddSymbolInfo(ret.get(), parser, symbolizer, symbolize_everything);
  }

  return ret.release();
}

}  // namespace perfprofd
}  // namespace android
