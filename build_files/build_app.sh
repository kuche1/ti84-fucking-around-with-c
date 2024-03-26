#! /usr/bin/env bash

set -euo pipefail

HERE=$(readlink -f $(dirname "$BASH_SOURCE"))

SDCC_ADDITIONAL_FLAGS='--Werror'

cd $1

if [ ! -f .buildid ]; then
    echo 0x$(openssl rand -hex 2) > .buildid
fi
BC="$(cat .buildid)"
echo "build id: $BC"

export MAINC="main.c"
export OUT_NAME=$2

trunkName=$(printf '%-8s' "$(head -c 8 <<<$2)") # truncate name and pad with extra spaces to make sure it is 8 char long

crt="$HERE/tios_crt0_app.s" # just got path to crt0

sed "s/qwertyui/$trunkName/" $crt > TEMP_crt0.s # fill in name in crt0
sed "s/0x6969/$BC/" TEMP_crt0.s > TEMP_crt0.s.s

sdasz80 -p -g -o tios_crt0.rel TEMP_crt0.s.s
sdcc $SDCC_ADDITIONAL_FLAGS -DFLASH_APP --no-std-crt0 --code-loc 16429 --data-loc 0 --std-sdcc99 -mz80 --reserve-regs-iy -o $OUT_NAME.ihx tios_crt0.rel $MAINC

objcopy -Iihex -Obinary $OUT_NAME.ihx $OUT_NAME.bin

rm $OUT_NAME.ihx $OUT_NAME.lk $OUT_NAME.lst $OUT_NAME.map $OUT_NAME.noi $OUT_NAME.rel $OUT_NAME.sym tios_crt0.rel TEMP_crt0.s TEMP_crt0.s.s

# you need to have `rabbitsign` installed
rabbitsign -t 8xk -g -f $OUT_NAME.bin

rm $OUT_NAME.bin
