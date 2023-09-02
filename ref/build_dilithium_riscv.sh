#!/bin/bash
CWD=$(pwd)

Help()
{
   # Display Help
   echo "Build the Dilithium scheme with a defined security level test and RISC-V core."
   echo
   echo "Syntax: ./build_dilithium_riscv.sh -t <test> -c <core>"
   echo
   echo "options for -t: test_dilithium2, test_dilithium3, test_dilithium5"
   echo "options for -c: steel, orca"
   echo
   echo "Example: ./build_dilithium_riscv.sh -t test_dilithium5 -c orca"
}

while getopts t:c:h flag
do
    case "${flag}" in
        t) TEST=${OPTARG};;
        c) CORE=${OPTARG};;
        h) # display Help
           Help
           exit;;
    esac
done

if [[ $TEST == "test_dilithium2" ]] || [[ $TEST == "test_dilithium3" ]] || [[ $TEST == "test_dilithium5" ]]
then
if [[ $CORE == "orca" ]]
then
  echo "Building $TEST for the ORCA core."
  sudo make clean
  sudo make test/$TEST ORCA=1
  sudo ./elf2hex.sh test/$TEST
  sudo make test/$TEST.ihex ORCA=1 # used for hw implementation
  sudo make test/$TEST.coe ORCA=1  # used for simulation
  sudo /opt/riscv/bin/riscv32-unknown-elf-objdump -d -S $CWD/test/$TEST > $CWD/test/generated_asm_$TEST.txt
elif [[ $CORE == "steel" ]]
then
  echo "Building $TEST for the STEEL core."
  sudo make clean
  sudo make test/$TEST
  sudo ./elf2hex.sh test/$TEST
  sudo cp test/$TEST.hex ../../steel_new_fpau/riscv-steel-core/hello_world/hello_world.mem
  sudo /opt/riscv/bin/riscv32-unknown-elf-objdump -d -S $CWD/test/$TEST > $CWD/test/generated_asm_$TEST.txt
else
  echo "Invalid core option. Call with -h for help"
fi
else
  echo "Invalid test option. Call with -h for help"
fi
