#!/bin/bash
CWD=$(pwd)

while getopts t:c: flag
do
    case "${flag}" in
        t) TEST=${OPTARG};;
        c) CORE=${OPTARG};;
    esac
done

sudo make clean

if [[ $CORE == "orca" ]]
then
  sudo make test/$TEST ORCA=1
  sudo ./elf2hex.sh test/$TEST
  sudo make test/$TEST.ihex ORCA=1 # used for hw implementation
  sudo make test/$TEST.coe ORCA=1   # used for simulation
  sudo /opt/riscv/bin/riscv32-unknown-elf-objdump -d -S $CWD/test/$TEST > $CWD/test/generated_asm_$TEST.txt
elif [[ $CORE == "steel" ]]
then
  sudo make test/$TEST
  sudo ./elf2hex.sh test/$TEST
  sudo cp test/$TEST.hex ../../steel_new_fpau/riscv-steel-core/hello_world/hello_world.mem
  sudo /opt/riscv/bin/riscv32-unknown-elf-objdump -d -S $CWD/test/$TEST > $CWD/test/generated_asm_$TEST.txt
else
  echo "Invalid core option."
fi
