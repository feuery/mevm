echo Compiling assembler 
./compile.sh

echo
echo

echo Compiling disassembler
./disassembler/compile-disassembler.sh

echo
echo

cd ./test/sum
assembler_printout=$(../../bin/main -c ./source.mlispa)
expected_assembler_printout=$(<./assembler_printout)
if [[ $assembler_printout == $expected_assembler_printout ]]; then
    echo SUCCESS!
    echo "Assembler verification of `pwd`/source.mlispa succeeded!"
    echo
else
    echo "Assembler verification of `pwd`/source.mlispa failed"
    echo $assembler_printout
    echo
    echo $expected_assembler_printout
fi

disassembler_printout=$(../../disassembler/bin/main ./out.mlisp)
expected_disassembler_printout=$(<./disassembler_printout)

if [[ $disassembler_printout == $expected_disassembler_printout ]]; then
    echo SUCCESS!
    echo "Disassembler verification of `pwd`/out.mlisp succeeded!"
    echo
else
    echo "Disassembler verification of `pwd`/out.mlisp failed"
    echo $disassembler_printout
    echo
    echo $expected_disassembler_printout
fi
