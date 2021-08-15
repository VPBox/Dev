/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <stdlib.h>
#include <android-base/strings.h>
#include "fec_private.h"

/* converts a hex nibble into an int */
static inline int hextobin(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        errno = EINVAL;
        return -1;
    }
}

/* converts a hex string `src' of `size' characters to binary and copies the
   the result into `dst' */
static int parse_hex(uint8_t *dst, uint32_t size, const char *src)
{
    int l, h;

    check(dst);
    check(src);
    check(2 * size == strlen(src));

    while (size) {
        h = hextobin(tolower(*src++));
        l = hextobin(tolower(*src++));

        check(l >= 0);
        check(h >= 0);

        *dst++ = (h << 4) | l;
        --size;
    }

    return 0;
}

/* parses a 64-bit unsigned integer from string `src' into `dst' and if
   `maxval' is >0, checks that `dst' <= `maxval' */
static int parse_uint64(const char *src, uint64_t maxval, uint64_t *dst)
{
    char *end;
    unsigned long long int value;

    check(src);
    check(dst);

    errno = 0;
    value = strtoull(src, &end, 0);

    if (*src == '\0' || *end != '\0' ||
            (errno == ERANGE && value == ULLONG_MAX)) {
        errno = EINVAL;
        return -1;
    }

    if (maxval && value > maxval) {
        errno = EINVAL;
        return -1;
    }

   *dst = (uint64_t)value;
    return 0;
}

/* computes the size of verity hash tree for `file_size' bytes and returns the
   number of hash tree levels in `verity_levels,' and the number of hashes per
   level in `level_hashes', if the parameters are non-NULL */
uint64_t verity_get_size(uint64_t file_size, uint32_t *verity_levels,
        uint32_t *level_hashes)
{
    /* we assume a known metadata size, 4 KiB block size, and SHA-256 to avoid
       relying on disk content */

    uint32_t level = 0;
    uint64_t total = 0;
    uint64_t hashes = file_size / FEC_BLOCKSIZE;

    do {
        if (level_hashes) {
            level_hashes[level] = hashes;
        }

        hashes = fec_div_round_up(hashes * SHA256_DIGEST_LENGTH, FEC_BLOCKSIZE);
        total += hashes;

        ++level;
    } while (hashes > 1);

    if (verity_levels) {
        *verity_levels = level;
    }

    return total * FEC_BLOCKSIZE;
}

/* computes a SHA-256 salted with `f->verity.salt' from a FEC_BLOCKSIZE byte
   buffer `block', and copies the hash to `hash' */
static inline int verity_hash(fec_handle *f, const uint8_t *block,
        uint8_t *hash)
{
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    check(f);
    check(f->verity.salt);
    SHA256_Update(&ctx, f->verity.salt, f->verity.salt_size);

    check(block);
    SHA256_Update(&ctx, block, FEC_BLOCKSIZE);

    check(hash);
    SHA256_Final(hash, &ctx);
    return 0;
}

/* computes a verity hash for FEC_BLOCKSIZE bytes from buffer `block' and
   compares it to the expected value in `expected' */
bool verity_check_block(fec_handle *f, const uint8_t *expected,
        const uint8_t *block)
{
    check(f);
    check(block);

    uint8_t hash[SHA256_DIGEST_LENGTH];

    if (unlikely(verity_hash(f, block, hash) == -1)) {
        error("failed to hash");
        return false;
    }

    check(expected);
    return !memcmp(expected, hash, SHA256_DIGEST_LENGTH);
}

/* reads a verity hash and the corresponding data block using error correction,
   if available */
static bool ecc_read_hashes(fec_handle *f, uint64_t hash_offset,
        uint8_t *hash, uint64_t data_offset, uint8_t *data)
{
    check(f);

    if (hash && fec_pread(f, hash, SHA256_DIGEST_LENGTH, hash_offset) !=
                    SHA256_DIGEST_LENGTH) {
        error("failed to read hash tree: offset %" PRIu64 ": %s", hash_offset,
            strerror(errno));
        return false;
    }

    check(data);

    if (fec_pread(f, data, FEC_BLOCKSIZE, data_offset) != FEC_BLOCKSIZE) {
        error("failed to read hash tree: data_offset %" PRIu64 ": %s",
            data_offset, strerror(errno));
        return false;
    }

    return true;
}

/* reads the verity hash tree, validates it against the root hash in `root',
   corrects errors if necessary, and copies valid data blocks for later use
   to `f->verity.hash' */
static int verify_tree(fec_handle *f, const uint8_t *root)
{
    uint8_t data[FEC_BLOCKSIZE];
    uint8_t hash[SHA256_DIGEST_LENGTH];

    check(f);
    check(root);

    verity_info *v = &f->verity;
    uint32_t levels = 0;

    /* calculate the size and the number of levels in the hash tree */
    v->hash_size =
        verity_get_size(v->data_blocks * FEC_BLOCKSIZE, &levels, NULL);

    check(v->hash_start < UINT64_MAX - v->hash_size);
    check(v->hash_start + v->hash_size <= f->data_size);

    uint64_t hash_offset = v->hash_start;
    uint64_t data_offset = hash_offset + FEC_BLOCKSIZE;

    v->hash_data_offset = data_offset;

    /* validate the root hash */
    if (!raw_pread(f, data, FEC_BLOCKSIZE, hash_offset) ||
            !verity_check_block(f, root, data)) {
        /* try to correct */
        if (!ecc_read_hashes(f, 0, NULL, hash_offset, data) ||
                !verity_check_block(f, root, data)) {
            error("root hash invalid");
            return -1;
        } else if (f->mode & O_RDWR &&
                    !raw_pwrite(f, data, FEC_BLOCKSIZE, hash_offset)) {
            error("failed to rewrite the root block: %s", strerror(errno));
            return -1;
        }
    }

    debug("root hash valid");

    /* calculate the number of hashes on each level */
    uint32_t hashes[levels];

    verity_get_size(v->data_blocks * FEC_BLOCKSIZE, NULL, hashes);

    /* calculate the size and offset for the data hashes */
    for (uint32_t i = 1; i < levels; ++i) {
        uint32_t blocks = hashes[levels - i];
        debug("%u hash blocks on level %u", blocks, levels - i);

        v->hash_data_offset = data_offset;
        v->hash_data_blocks = blocks;

        data_offset += blocks * FEC_BLOCKSIZE;
    }

    check(v->hash_data_blocks);
    check(v->hash_data_blocks <= v->hash_size / FEC_BLOCKSIZE);

    check(v->hash_data_offset);
    check(v->hash_data_offset <=
        UINT64_MAX - (v->hash_data_blocks * FEC_BLOCKSIZE));
    check(v->hash_data_offset < f->data_size);
    check(v->hash_data_offset + v->hash_data_blocks * FEC_BLOCKSIZE <=
        f->data_size);

    /* copy data hashes to memory in case they are corrupted, so we don't
       have to correct them every time they are needed */
    std::unique_ptr<uint8_t[]> data_hashes(
       new (std::nothrow) uint8_t[f->verity.hash_data_blocks * FEC_BLOCKSIZE]);

    if (!data_hashes) {
        errno = ENOMEM;
        return -1;
    }

    /* validate the rest of the hash tree */
    data_offset = hash_offset + FEC_BLOCKSIZE;

    for (uint32_t i = 1; i < levels; ++i) {
        uint32_t blocks = hashes[levels - i];

        for (uint32_t j = 0; j < blocks; ++j) {
            /* ecc reads are very I/O intensive, so read raw hash tree and do
               error correcting only if it doesn't validate */
            if (!raw_pread(f, hash, SHA256_DIGEST_LENGTH,
                    hash_offset + j * SHA256_DIGEST_LENGTH) ||
                !raw_pread(f, data, FEC_BLOCKSIZE,
                    data_offset + j * FEC_BLOCKSIZE)) {
                error("failed to read hashes: %s", strerror(errno));
                return -1;
            }

            if (!verity_check_block(f, hash, data)) {
                /* try to correct */
                if (!ecc_read_hashes(f,
                        hash_offset + j * SHA256_DIGEST_LENGTH, hash,
                        data_offset + j * FEC_BLOCKSIZE, data) ||
                    !verity_check_block(f, hash, data)) {
                    error("invalid hash tree: hash_offset %" PRIu64 ", "
                        "data_offset %" PRIu64 ", block %u",
                        hash_offset, data_offset, j);
                    return -1;
                }

                /* update the corrected blocks to the file if we are in r/w
                   mode */
                if (f->mode & O_RDWR) {
                    if (!raw_pwrite(f, hash, SHA256_DIGEST_LENGTH,
                            hash_offset + j * SHA256_DIGEST_LENGTH) ||
                        !raw_pwrite(f, data, FEC_BLOCKSIZE,
                            data_offset + j * FEC_BLOCKSIZE)) {
                        error("failed to write hashes: %s", strerror(errno));
                        return -1;
                    }
                }
            }

            if (blocks == v->hash_data_blocks) {
                memcpy(data_hashes.get() + j * FEC_BLOCKSIZE, data,
                    FEC_BLOCKSIZE);
            }
        }

        hash_offset = data_offset;
        data_offset += blocks * FEC_BLOCKSIZE;
    }

    debug("valid");

    if (v->hash) {
        delete[] v->hash;
        v->hash = NULL;
    }

    v->hash = data_hashes.release();
    return 0;
}

/* reads, corrects and parses the verity table, validates parameters, and if
   `f->flags' does not have `FEC_VERITY_DISABLE' set, calls `verify_tree' to
   load and validate the hash tree */
static int parse_table(fec_handle *f, uint64_t offset, uint32_t size, bool useecc)
{
    check(f);
    check(size >= VERITY_MIN_TABLE_SIZE);
    check(size <= VERITY_MAX_TABLE_SIZE);

    debug("offset = %" PRIu64 ", size = %u", offset, size);

    verity_info *v = &f->verity;
    std::unique_ptr<char[]> table(new (std::nothrow) char[size + 1]);

    if (!table) {
        errno = ENOMEM;
        return -1;
    }

    if (!useecc) {
        if (!raw_pread(f, table.get(), size, offset)) {
            error("failed to read verity table: %s", strerror(errno));
            return -1;
        }
    } else if (fec_pread(f, table.get(), size, offset) != (ssize_t)size) {
        error("failed to ecc read verity table: %s", strerror(errno));
        return -1;
    }

    table[size] = '\0';
    debug("verity table: '%s'", table.get());

    int i = 0;
    std::unique_ptr<uint8_t[]> salt;
    uint8_t root[SHA256_DIGEST_LENGTH];

    auto tokens = android::base::Split(table.get(), " ");

    for (const auto& token : tokens) {
        switch (i++) {
        case 0: /* version */
            if (token != stringify(VERITY_TABLE_VERSION)) {
                error("unsupported verity table version: %s", token.c_str());
                return -1;
            }
            break;
        case 3: /* data_block_size */
        case 4: /* hash_block_size */
            /* assume 4 KiB block sizes for everything */
            if (token != stringify(FEC_BLOCKSIZE)) {
                error("unsupported verity block size: %s", token.c_str());
                return -1;
            }
            break;
        case 5: /* num_data_blocks */
            if (parse_uint64(token.c_str(), f->data_size / FEC_BLOCKSIZE,
                    &v->data_blocks) == -1) {
                error("invalid number of verity data blocks: %s",
                    token.c_str());
                return -1;
            }
            break;
        case 6: /* hash_start_block */
            if (parse_uint64(token.c_str(), f->data_size / FEC_BLOCKSIZE,
                    &v->hash_start) == -1) {
                error("invalid verity hash start block: %s", token.c_str());
                return -1;
            }

            v->hash_start *= FEC_BLOCKSIZE;
            break;
        case 7: /* algorithm */
            if (token != "sha256") {
                error("unsupported verity hash algorithm: %s", token.c_str());
                return -1;
            }
            break;
        case 8: /* digest */
            if (parse_hex(root, sizeof(root), token.c_str()) == -1) {
                error("invalid verity root hash: %s", token.c_str());
                return -1;
            }
            break;
        case 9: /* salt */
            v->salt_size = token.size();
            check(v->salt_size % 2 == 0);
            v->salt_size /= 2;

            salt.reset(new (std::nothrow) uint8_t[v->salt_size]);

            if (!salt) {
                errno = ENOMEM;
                return -1;
            }

            if (parse_hex(salt.get(), v->salt_size, token.c_str()) == -1) {
                error("invalid verity salt: %s", token.c_str());
                return -1;
            }
            break;
        default:
            break;
        }
    }

    if (i < VERITY_TABLE_ARGS) {
        error("not enough arguments in verity table: %d; expected at least "
            stringify(VERITY_TABLE_ARGS), i);
        return -1;
    }

    check(v->hash_start < f->data_size);

    if (v->metadata_start < v->hash_start) {
        check(v->data_blocks == v->metadata_start / FEC_BLOCKSIZE);
    } else {
        check(v->data_blocks == v->hash_start / FEC_BLOCKSIZE);
    }

    if (v->salt) {
        delete[] v->salt;
        v->salt = NULL;
    }

    v->salt = salt.release();

    if (v->table) {
        delete[] v->table;
        v->table = NULL;
    }

    v->table = table.release();

    if (!(f->flags & FEC_VERITY_DISABLE)) {
        if (verify_tree(f, root) == -1) {
            return -1;
        }

        check(v->hash);

        uint8_t zero_block[FEC_BLOCKSIZE];
        memset(zero_block, 0, FEC_BLOCKSIZE);

        if (verity_hash(f, zero_block, v->zero_hash) == -1) {
            error("failed to hash");
            return -1;
        }
    }

    return 0;
}

/* rewrites verity metadata block using error corrected data in `f->verity' */
static int rewrite_metadata(fec_handle *f, uint64_t offset)
{
    check(f);
    check(f->data_size > VERITY_METADATA_SIZE);
    check(offset <= f->data_size - VERITY_METADATA_SIZE);

    std::unique_ptr<uint8_t[]> metadata(
        new (std::nothrow) uint8_t[VERITY_METADATA_SIZE]);

    if (!metadata) {
        errno = ENOMEM;
        return -1;
    }

    memset(metadata.get(), 0, VERITY_METADATA_SIZE);

    verity_info *v = &f->verity;
    memcpy(metadata.get(), &v->header, sizeof(v->header));

    check(v->table);
    size_t len = strlen(v->table);

    check(sizeof(v->header) + len <= VERITY_METADATA_SIZE);
    memcpy(metadata.get() + sizeof(v->header), v->table, len);

    return raw_pwrite(f, metadata.get(), VERITY_METADATA_SIZE, offset);
}

static int validate_header(const fec_handle *f, const verity_header *header,
        uint64_t offset)
{
    check(f);
    check(header);

    if (header->magic != VERITY_MAGIC &&
        header->magic != VERITY_MAGIC_DISABLE) {
        return -1;
    }

    if (header->version != VERITY_VERSION) {
        error("unsupported verity version %u", header->version);
        return -1;
    }

    if (header->length < VERITY_MIN_TABLE_SIZE ||
        header->length > VERITY_MAX_TABLE_SIZE) {
        error("invalid verity table size: %u; expected ["
            stringify(VERITY_MIN_TABLE_SIZE) ", "
            stringify(VERITY_MAX_TABLE_SIZE) ")", header->length);
        return -1;
    }

    /* signature is skipped, because for our purposes it won't matter from
       where the data originates; the caller of the library is responsible
       for signature verification */

    if (offset > UINT64_MAX - header->length) {
        error("invalid verity table length: %u", header->length);
        return -1;
    } else if (offset + header->length >= f->data_size) {
        error("invalid verity table length: %u", header->length);
        return -1;
    }

    return 0;
}

/* attempts to read verity metadata from `f->fd' position `offset'; if in r/w
   mode, rewrites the metadata if it had errors */
int verity_parse_header(fec_handle *f, uint64_t offset)
{
    check(f);
    check(f->data_size > VERITY_METADATA_SIZE);

    if (offset > f->data_size - VERITY_METADATA_SIZE) {
        debug("failed to read verity header: offset %" PRIu64 " is too far",
            offset);
        return -1;
    }

    verity_info *v = &f->verity;
    uint64_t errors = f->errors;

    if (!raw_pread(f, &v->header, sizeof(v->header), offset)) {
        error("failed to read verity header: %s", strerror(errno));
        return -1;
    }

    /* use raw data to check for the alternative magic, because it will
       be error corrected to VERITY_MAGIC otherwise */
    if (v->header.magic == VERITY_MAGIC_DISABLE) {
        /* this value is not used by us, but can be used by a caller to
           decide whether dm-verity should be enabled */
        v->disabled = true;
    }

    if (fec_pread(f, &v->ecc_header, sizeof(v->ecc_header), offset) !=
            sizeof(v->ecc_header)) {
        warn("failed to read verity header: %s", strerror(errno));
        return -1;
    }

    if (validate_header(f, &v->header, offset)) {
        /* raw verity header is invalid; this could be due to corruption, or
           due to missing verity metadata */

        if (validate_header(f, &v->ecc_header, offset)) {
            return -1; /* either way, we cannot recover */
        }

        /* report mismatching fields */
        if (!v->disabled && v->header.magic != v->ecc_header.magic) {
            warn("corrected verity header magic");
            v->header.magic = v->ecc_header.magic;
        }

        if (v->header.version != v->ecc_header.version) {
            warn("corrected verity header version");
            v->header.version = v->ecc_header.version;
        }

        if (v->header.length != v->ecc_header.length) {
            warn("corrected verity header length");
            v->header.length = v->ecc_header.length;
        }

        if (memcmp(v->header.signature, v->ecc_header.signature,
                sizeof(v->header.signature))) {
            warn("corrected verity header signature");
            /* we have no way of knowing which signature is correct, if either
               of them is */
        }
    }

    v->metadata_start = offset;

    if (parse_table(f, offset + sizeof(v->header), v->header.length,
            false) == -1 &&
        parse_table(f, offset + sizeof(v->header), v->header.length,
            true)  == -1) {
        return -1;
    }

    /* if we corrected something while parsing metadata and we are in r/w
       mode, rewrite the corrected metadata */
    if (f->mode & O_RDWR && f->errors > errors &&
            rewrite_metadata(f, offset) < 0) {
        warn("failed to rewrite verity metadata: %s", strerror(errno));
    }

    if (v->metadata_start < v->hash_start) {
        f->data_size = v->metadata_start;
    } else {
        f->data_size = v->hash_start;
    }

    return 0;
}

int fec_verity_set_status(struct fec_handle *f, bool enabled)
{
    check(f);

    if (!(f->mode & O_RDWR)) {
        error("cannot update verity magic: read-only handle");
        errno = EBADF;
        return -1;
    }

    verity_info *v = &f->verity;

    if (!v->metadata_start) {
        error("cannot update verity magic: no metadata found");
        errno = EINVAL;
        return -1;
    }

    if (v->disabled == !enabled) {
        return 0; /* nothing to do */
    }

    uint32_t magic = enabled ? VERITY_MAGIC : VERITY_MAGIC_DISABLE;

    if (!raw_pwrite(f, &magic, sizeof(magic), v->metadata_start)) {
        error("failed to update verity magic to %08x: %s", magic,
            strerror(errno));
        return -1;
    }

    warn("updated verity magic to %08x (%s)", magic,
        enabled ? "enabled" : "disabled");
    v->disabled = !enabled;

    return 0;
}
