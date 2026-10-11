#!/bin/sh
set -eu
cmake -S "$(dirname "$0")" -B "$(dirname "$0")/build" -DCMAKE_BUILD_TYPE=Release
cmake --build "$(dirname "$0")/build" --parallel
