#!/bin/sh
# A utility script that runs benchmark on Android device
#
# Note: Only one Android device can be connected when running this script
#
# Example usage:
#   $ cd system/bt
#   $ ./test/run_benchmarks.sh bluetooth_benchmark_example

known_benchmarks=(
  bluetooth_benchmark_thread_performance
  bluetooth_benchmark_timer_performance
)

usage() {
  binary="$(basename "$0")"
  echo "Usage: ${binary} --help"
  echo "       ${binary} [-i <iterations>] [-s <specific device>] [--all] [<benchmark name>[.<filter>] ...] [--<arg> ...]"
  echo
  echo "Unknown long arguments are passed to the benchmark."
  echo
  echo "Known benchmark names:"

  for name in "${known_benchmarks[@]}"
  do
    echo "    ${name}"
  done
}

iterations=1
device=
benchmarks=()
benchmark_args=()
while [ $# -gt 0 ]
do
  case "$1" in
    -h|--help)
      usage
      exit 0
      ;;
    -i)
      shift
      if [ $# -eq 0 ]; then
        echo "error: number of iterations expected" 1>&2
        usage
        exit 2
      fi
      iterations=$(( $1 ))
      shift
      ;;
    -s)
      shift
      if [ $# -eq 0 ]; then
        echo "error: no device specified" 1>&2
        usage
        exit 2
      fi
      device="$1"
      shift
      ;;
    --all)
      benchmarks+=( "${known_benchmarks[@]}" )
      shift
      ;;
    --*)
      benchmark_args+=( "$1" )
      shift
      ;;
    *)
      benchmarks+=( "$1" )
      shift
      ;;
  esac
done

if [ "${#benchmarks[@]}" -eq 0 ]; then
  benchmarks+=( "${known_benchmarks[@]}" )
fi

adb=( "adb" )
if [ -n "${device}" ]; then
  adb+=( "-s" "${device}" )
fi

source ${ANDROID_BUILD_TOP}/build/envsetup.sh
target_arch=$(gettargetarch)

failed_benchmarks=()
for spec in "${benchmarks[@]}"
do
  name="${spec%%.*}"
  if [[ $target_arch == *"64"* ]]; then
    binary="/data/benchmarktest64/${name}/${name}"
  else
    binary="/data/benchmarktest/${name}/${name}"
  fi

  push_command=( "${adb[@]}" push {"${ANDROID_PRODUCT_OUT}",}"${binary}" )
  benchmark_command=( "${adb[@]}" shell "${binary}" )
  if [ "${name}" != "${spec}" ]; then
    filter="${spec#*.}"
    benchmark_command+=( "--benchmark_filter=${filter}" )
  fi
  benchmark_command+=( "${benchmark_args[@]}" )

  echo "--- ${name} ---"
  echo "pushing..."
  "${push_command[@]}"
  echo "running..."
  failed_count=0
  for i in $(seq 1 ${iterations})
  do
    "${benchmark_command[@]}" || failed_count=$(( $failed_count + 1 ))
  done

  if [ $failed_count != 0 ]; then
    failed_benchmarks+=( "${name} ${failed_count}/${iterations}" )
  fi
done

if [ "${#failed_benchmarks[@]}" -ne 0 ]; then
  for failed_benchmark in "${failed_benchmarks[@]}"
  do
    echo "!!! FAILED TEST: ${failed_benchmark} !!!"
  done
  exit 1
fi

exit 0
