//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/payload_generator/xz.h"

#include <elf.h>
#include <endian.h>

#include <algorithm>

#include <7zCrc.h>
#include <Xz.h>
#include <XzEnc.h>
#include <base/logging.h>

namespace {

bool xz_initialized = false;

// An ISeqInStream implementation that reads all the data from the passed Blob.
struct BlobReaderStream : public ISeqInStream {
  explicit BlobReaderStream(const brillo::Blob& data) : data_(data) {
    Read = &BlobReaderStream::ReadStatic;
  }

  static SRes ReadStatic(const ISeqInStream* p, void* buf, size_t* size) {
    auto* self = static_cast<BlobReaderStream*>(const_cast<ISeqInStream*>(p));
    *size = std::min(*size, self->data_.size() - self->pos_);
    memcpy(buf, self->data_.data() + self->pos_, *size);
    self->pos_ += *size;
    return SZ_OK;
  }

  const brillo::Blob& data_;

  // The current reader position.
  size_t pos_ = 0;
};

// An ISeqOutStream implementation that writes all the data to the passed Blob.
struct BlobWriterStream : public ISeqOutStream {
  explicit BlobWriterStream(brillo::Blob* data) : data_(data) {
    Write = &BlobWriterStream::WriteStatic;
  }

  static size_t WriteStatic(const ISeqOutStream* p,
                            const void* buf,
                            size_t size) {
    auto* self = static_cast<const BlobWriterStream*>(p);
    const uint8_t* buffer = reinterpret_cast<const uint8_t*>(buf);
    self->data_->reserve(self->data_->size() + size);
    self->data_->insert(self->data_->end(), buffer, buffer + size);
    return size;
  }

  brillo::Blob* data_;
};

// Returns the filter id to be used to compress |data|.
// Only BCJ filter for x86 and ARM ELF file are supported, returns 0 otherwise.
int GetFilterID(const brillo::Blob& data) {
  if (data.size() < sizeof(Elf32_Ehdr) ||
      memcmp(data.data(), ELFMAG, SELFMAG) != 0)
    return 0;

  const Elf32_Ehdr* header = reinterpret_cast<const Elf32_Ehdr*>(data.data());

  // Only little-endian is supported.
  if (header->e_ident[EI_DATA] != ELFDATA2LSB)
    return 0;

  switch (le16toh(header->e_machine)) {
    case EM_386:
    case EM_X86_64:
      return XZ_ID_X86;
    case EM_ARM:
      // Both ARM and ARM Thumb instructions could be found in the same ARM ELF
      // file. We choose to use the ARM Thumb filter here because testing shows
      // that it usually works better than the ARM filter.
      return XZ_ID_ARMT;
#ifdef EM_AARCH64
    case EM_AARCH64:
      // Neither the ARM nor the ARM Thumb filter works well with AArch64.
      return 0;
#endif
  }
  return 0;
}

}  // namespace

namespace chromeos_update_engine {

void XzCompressInit() {
  if (xz_initialized)
    return;
  xz_initialized = true;
  // Although we don't include a CRC32 for the stream, the xz file header has
  // a CRC32 of the header itself, which required the CRC table to be
  // initialized.
  CrcGenerateTable();
}

bool XzCompress(const brillo::Blob& in, brillo::Blob* out) {
  CHECK(xz_initialized) << "Initialize XzCompress first";
  out->clear();
  if (in.empty())
    return true;

  // Xz compression properties.
  CXzProps props;
  XzProps_Init(&props);
  // No checksum in the xz stream. xz-embedded (used by the decompressor) only
  // supports CRC32, but we already check the sha-1 of the whole blob during
  // payload application.
  props.checkId = XZ_CHECK_NO;

  // LZMA2 compression properties.
  CLzma2EncProps lzma2Props;
  Lzma2EncProps_Init(&lzma2Props);
  // LZMA compression "level 6" requires 9 MB of RAM to decompress in the worst
  // case.
  lzma2Props.lzmaProps.level = 6;
  lzma2Props.lzmaProps.numThreads = 1;
  // The input size data is used to reduce the dictionary size if possible.
  lzma2Props.lzmaProps.reduceSize = in.size();
  Lzma2EncProps_Normalize(&lzma2Props);
  props.lzma2Props = lzma2Props;

  props.filterProps.id = GetFilterID(in);

  BlobWriterStream out_writer(out);
  BlobReaderStream in_reader(in);
  SRes res = Xz_Encode(&out_writer, &in_reader, &props, nullptr /* progress */);
  return res == SZ_OK;
}

}  // namespace chromeos_update_engine
