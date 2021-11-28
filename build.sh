#!/bin/bash

set -e
set -o pipefail
echo Running tests...
cd test
./run-tests.sh
echo Finished running tests!
cd ..
echo Building and running brunix...
rm -rf build/; mkdir build/; cd build/; cmake ../; make; make qemu
cd ..