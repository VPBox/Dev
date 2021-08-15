#!/bin/bash
CLANG_PACKAGE=clang
GNSHA1_URL="https://chromium.googlesource.com/chromium/buildtools/\
+/master/linux64/gn.sha1?format=TEXT"

# Check if clang is already installed on current system
clang_path=`which clang`
if [ -f "$clang_path" ]; then
  # if clang binary is avalable, check its version
  clang_version=$($clang_path --version | grep clang | sed "s/.*version\s*\([0-9]*\.[0-9]*\).*/\1/")
  IFS="." read -ra clang_version_array <<< "$clang_version"
  clang_version_major=${clang_version_array[0]}
  clang_version_minor=${clang_version_array[1]}
  # if the version is greater than 3.5 then do not install clang here
  if [ $clang_version_major -ge 3 ] && [ $clang_version_minor -ge 5 ]; then
    echo "Detected clang $clang_version"
    CLANG_PACKAGE=""
  fi
fi

if [ ! -z "$CLANG_PACKAGE" ]; then
  # Try to find clang from a known list
  for clang_version in 3.9 3.8 3.7 3.6 3.5
  do
    clang_path=`which clang-$clang_version`
    if [ -f "$clang_path" ]; then
      echo "Detected clang-$clang_version"
      CLANG_PACKAGE=""
      break
    fi
  done
fi

if [ ! -z "$CLANG_PACKAGE" ]; then
  echo "clang not found on current system, installing"
  if [ -f /etc/lsb-release ]; then
	  # Ubuntu
	  ubuntu_version=$(lsb_release --release --short)
	  IFS="." read -ra ubuntu_version_array <<< "$ubuntu_version"
	  ubuntu_version_major=${ubuntu_version_array[0]}
	  ubuntu_version_minor=${ubuntu_version_array[1]}
	  if [ $ubuntu_version_major -lt 15 ]; then
	    echo "Choose clang-3.8 for Ubuntu 14 and below"
	    CLANG_PACKAGE=clang-3.8
	  fi
  fi
fi

sudo apt-get -y install $CLANG_PACKAGE libevent-dev libc++-dev libc++abi-dev \
                        ninja-build
gn_path=`which gn`
if [ -z $gn_path ]; then
  gnsha1=$(curl $GNSHA1_URL | base64 -d)
  wget -O gn http://storage.googleapis.com/chromium-gn/$gnsha1
  chmod a+x ./gn
  sudo mv ./gn /usr/bin/
fi
