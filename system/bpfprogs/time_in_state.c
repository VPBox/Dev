/*
 * time_in_state eBPF program
 *
 * Copyright (C) 2018 Google
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <bpf_helpers.h>

typedef struct {
    uint32_t uid;
    uint32_t freq;
} time_key;

DEFINE_BPF_MAP(uid_times_map, PERCPU_HASH, time_key, uint64_t, 10240)
DEFINE_BPF_MAP(cpu_last_update_map, PERCPU_ARRAY, uint32_t, uint64_t, 1)

/* Assume max of 1024 CPUs */
DEFINE_BPF_MAP(cpu_freq_map, ARRAY, uint32_t, uint32_t, 1024)

struct switch_args {
    unsigned long long ignore;
    char prev_comm[16];
    int prev_pid;
    int prev_prio;
    long long prev_state;
    char next_comm[16];
    int next_pid;
    int next_prio;
};

SEC("tracepoint/sched/sched_switch")
int tp_sched_switch(struct switch_args* args) {
    uint32_t zero = 0;
    uint64_t* last = bpf_cpu_last_update_map_lookup_elem(&zero);
    if (!last) return 0;
    uint64_t old_last = *last;
    uint64_t time = bpf_ktime_get_ns();
    *last = time;
    uint32_t cpu = bpf_get_smp_processor_id();
    uint32_t* freq = bpf_cpu_freq_map_lookup_elem(&cpu);
    if (args->prev_pid && old_last && freq && *freq) {
        uint32_t uid = bpf_get_current_uid_gid();
        time_key key = {.uid = uid, .freq = *freq};
        uint64_t* tot_time = bpf_uid_times_map_lookup_elem(&key);
        uint64_t delta = time - old_last;
        if (!tot_time)
            bpf_uid_times_map_update_elem(&key, &delta, BPF_ANY);
        else
            *tot_time += delta;
    }
    return 0;
}

struct cpufreq_args {
    unsigned long long ignore;
    unsigned int state;
    unsigned int cpu_id;
};

SEC("tracepoint/power/cpu_frequency")
int tp_cpufreq(struct cpufreq_args* args) {
    uint32_t cpu = args->cpu_id;
    unsigned int new = args->state;
    bpf_cpu_freq_map_update_elem(&cpu, &new, BPF_ANY);
    return 0;
}

char _license[] SEC("license") = "GPL";
