#! /usr/bin/env bash

set -euo pipefail

#bash TiConstructor/build_app.sh fkngarnd fkngarnd
bash TiConstructor/build_prog.sh fkngarnd fkngarnd

tilp --no-gui --silent fkngarnd/fkngarnd.8xp > /dev/null
