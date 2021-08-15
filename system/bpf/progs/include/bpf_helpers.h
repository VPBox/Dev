/* Common BPF helpers to be used by all BPF programs loaded by Android */

#include <linux/bpf.h>
#include <stdbool.h>
#include <stdint.h>

/* place things in different elf sections */
#define SEC(NAME) __attribute__((section(NAME), used))

/*
 * Helper functions called from eBPF programs written in C. These are
 * implemented in the kernel sources.
 */

/* generic functions */

/*
 * Type-unsafe bpf map functions - avoid if possible.
 *
 * Using these it is possible to pass in keys/values of the wrong type/size,
 * or, for 'unsafe_bpf_map_lookup_elem' receive into a pointer to the wrong type.
 * You will not get a compile time failure, and for certain types of errors you
 * might not even get a failure from the kernel's ebpf verifier during program load,
 * instead stuff might just not work right at runtime.
 *
 * Instead please use:
 *   DEFINE_BPF_MAP(foo_map, TYPE, KeyType, ValueType, num_entries)
 * where TYPE can be something like HASH or ARRAY, and num_entries is an integer.
 *
 * This defines the map (hence this should not be used in a header file included
 * from multiple locations) and provides type safe accessors:
 *   ValueType * bpf_foo_map_lookup_elem(KeyType *)
 *   int bpf_foo_map_update_elem(KeyType *, ValueType *, flags)
 *   int bpf_foo_map_delete_elem(KeyType *)
 *
 * This will make sure that if you change the type of a map you'll get compile
 * errors at any spots you forget to update with the new type.
 */
static void* (*unsafe_bpf_map_lookup_elem)(void* map, void* key) = (void*)BPF_FUNC_map_lookup_elem;
static int (*unsafe_bpf_map_update_elem)(void* map, void* key, void* value,
                                         unsigned long long flags) = (void*)
        BPF_FUNC_map_update_elem;
static int (*unsafe_bpf_map_delete_elem)(void* map, void* key) = (void*)BPF_FUNC_map_delete_elem;

/* type safe macro to declare a map and related accessor functions */
#define DEFINE_BPF_MAP_NO_ACCESSORS(the_map, TYPE, TypeOfKey, TypeOfValue, num_entries) \
    struct bpf_map_def SEC("maps") the_map = {                                          \
            .type = BPF_MAP_TYPE_##TYPE,                                                \
            .key_size = sizeof(TypeOfKey),                                              \
            .value_size = sizeof(TypeOfValue),                                          \
            .max_entries = (num_entries),                                               \
    };

#define DEFINE_BPF_MAP(the_map, TYPE, TypeOfKey, TypeOfValue, num_entries)                 \
    DEFINE_BPF_MAP_NO_ACCESSORS(the_map, TYPE, TypeOfKey, TypeOfValue, num_entries)        \
                                                                                           \
    static inline __always_inline __unused TypeOfValue* bpf_##the_map##_lookup_elem(       \
            TypeOfKey* k) {                                                                \
        return unsafe_bpf_map_lookup_elem(&the_map, k);                                    \
    };                                                                                     \
                                                                                           \
    static inline __always_inline __unused int bpf_##the_map##_update_elem(                \
            TypeOfKey* k, TypeOfValue* v, unsigned long long flags) {                      \
        return unsafe_bpf_map_update_elem(&the_map, k, v, flags);                          \
    };                                                                                     \
                                                                                           \
    static inline __always_inline __unused int bpf_##the_map##_delete_elem(TypeOfKey* k) { \
        return unsafe_bpf_map_delete_elem(&the_map, k);                                    \
    };

static int (*bpf_probe_read)(void* dst, int size, void* unsafe_ptr) = (void*) BPF_FUNC_probe_read;
static unsigned long long (*bpf_ktime_get_ns)(void) = (void*) BPF_FUNC_ktime_get_ns;
static int (*bpf_trace_printk)(const char* fmt, int fmt_size, ...) = (void*) BPF_FUNC_trace_printk;
static unsigned long long (*bpf_get_current_pid_tgid)(void) = (void*) BPF_FUNC_get_current_pid_tgid;
static unsigned long long (*bpf_get_current_uid_gid)(void) = (void*) BPF_FUNC_get_current_uid_gid;
static unsigned long long (*bpf_get_smp_processor_id)(void) = (void*) BPF_FUNC_get_smp_processor_id;
/* networking  */
static uint64_t (*bpf_get_socket_cookie)(struct __sk_buff* skb) = (void*)BPF_FUNC_get_socket_cookie;
static uint32_t (*bpf_get_socket_uid)(struct __sk_buff* skb) = (void*)BPF_FUNC_get_socket_uid;
static int (*bpf_skb_load_bytes)(struct __sk_buff* skb, int off, void* to,
                                 int len) = (void*)BPF_FUNC_skb_load_bytes;

static int (*bpf_skb_change_proto)(struct __sk_buff* skb, __be16 proto,
                                   __u64 flags) = (void*)BPF_FUNC_skb_change_proto;
static int (*bpf_l3_csum_replace)(struct __sk_buff* skb, __u32 offset, __u64 from, __u64 to,
                                  __u64 flags) = (void*)BPF_FUNC_l3_csum_replace;
static int (*bpf_l4_csum_replace)(struct __sk_buff* skb, __u32 offset, __u64 from, __u64 to,
                                  __u64 flags) = (void*)BPF_FUNC_l4_csum_replace;
static int (*bpf_redirect)(__u32 ifindex, __u64 flags) = (void*)BPF_FUNC_redirect;

/*
 * Map structure to be used by Android eBPF C programs. The Android eBPF loader
 * uses this structure from eBPF object to create maps at boot time.
 *
 * The eBPF C program should define structure in the maps section using
 * SEC("maps") otherwise it will be ignored by the eBPF loader.
 *
 * For example:
 * struct bpf_map_def SEC("maps") mymap { .type=... , .key_size=... }
 */
struct bpf_map_def {
    unsigned int type;
    unsigned int key_size;
    unsigned int value_size;
    unsigned int max_entries;
    unsigned int map_flags;
    unsigned int pad1;
    unsigned int pad2;
};
