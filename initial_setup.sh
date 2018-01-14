#!/bin/bash

# 途中で失敗したら終了するようにする
set -e

SCRIPT_DIR=$(cd $(dirname $0);pwd)

# Google Testを取得する
TEMP_DIR=$(mktemp -d /tmp/tmp_dir.XXXX)
cd ${TEMP_DIR}
git clone git@github.com:google/googletest.git googletest
cd googletest
git checkout refs/tags/release-1.8.0
cp -r ${TEMP_DIR}/googletest/googletest ${SCRIPT_DIR}/lib/googletest
rm -rf ${TEMP_DIR}
