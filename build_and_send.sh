#! /usr/bin/env bash

set -euo pipefail

HERE=$(readlink -f $(dirname "$BASH_SOURCE"))

echo 'Compiling...'

# bash TiConstructor/build_prog.sh fkngarnd fkngarnd
bash "$HERE/build_files/build_app.sh" fkngarnd fkngarnd

COMFILED_FILE="$HERE/fkngarnd/fkngarnd.8xk"

echo "Compiled binary size: $(stat -c %s "$COMFILED_FILE") bytes"

echo 'Transfering...'

# tilp --no-gui --silent fkngarnd/fkngarnd.8xp > /dev/null
tilp --no-gui --silent "$COMFILED_FILE" > /dev/null

# if tilp refuses to transfer this because of invalid signature
# you need to download `rabbitsign` from your package manager and
# replace `TiConstructor/other_files/rabbitsign`

echo 'Success'
