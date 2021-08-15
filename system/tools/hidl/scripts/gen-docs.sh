#!/usr/bin/env bash

# TODO: use the build system to generate docs once we can

# e.x. convertFqName "android.hardware.foo@1.0::foo::IFoo" "\5" -> IFoo
function convertFqName() {
    # android.hardware.foo@1.0::foo::IFoo
    # \1 => android.hardware.foo
    # \3 => 1
    # \4 => 0
    # \5 => IFoo # optional capture group
    local FQNAME="^(([a-zA-Z0-9]+\.)*[a-zA-Z0-9]+)@([0-9]+).([0-9]+)(::([a-zA-Z0-9]+))?$"

    local fqName="$1"
    local pattern="$2"

    echo $1 | sed -E "s/$FQNAME/$2/g"
}
function assertInterfaceFqName() {
    if [ -n "$(convertFqName "$1")" ]; then
        echo "$1 is not an interface fqname"
        return 1
    fi
}
function javaTarget() { convertFqName "$1" "\1-V\3.\4-java"; }
function cppTarget()  { convertFqName "$1" "\1@\3.\4"; }

function hidl-gen-output() {
    local out="$1"

    while read -r fqName; do
        echo "Generating output for $fqName."
        hidl-gen -Lc++-headers -o "$out/cpp" "$fqName" || exit 1
        hidl-gen -Ljava -o "$out/java" "$fqName" 2>/dev/null
    done
}

function current-interfaces() {
    [ $# = 1 ] || { echo "usage: current-interfaces <package root directory>" && return 1; }

    local package_root="$1"
    [ -d "$package_root" ] || { echo "current-interfaces: directory $package_root does not exist" && return 1; }

    local current_file="$package_root/current.txt"
    [ -f "$current_file" ] || { echo "current-interfaces: current file $current_file does not exist" && return 1; }

    cat "$current_file" | cut -d '#' -f1 | awk '{print $2}' | sed "/^ *$/d" | sort | uniq
}

function google-interfaces() {
    local roots=(
        hardware/interfaces
        system/hardware/interfaces
        frameworks/hardware/interfaces
        system/libhidl/transport
    )

    for root in "${roots[@]}"; do
        current-interfaces "$ANDROID_BUILD_TOP/$root"
    done | sort | uniq
}

function google-interface-packages() {
    google-interfaces | cut -d ':' -f1 | sort | uniq
}

function hidl-doc-generate-sources() {
    local outputDir="$1"
    [ -z "$1" ] && outputDir="gen"

    echo "Generating sources in $(realpath $outputDir)"

    google-interface-packages | hidl-gen-output "$outputDir"

    echo "Deleting implementation-related files from $outputDir."
    rm $(find "$outputDir/cpp" -\( -name "IHw*.h" \
                                   -o -name "BnHw*.h" \
                                   -o -name "BpHw*.h" \
                                   -o -name "Bs*.h" \
                                   -o -name "hwtypes.h" \))

    echo "Done: generated sources are in $outputDir"
}

