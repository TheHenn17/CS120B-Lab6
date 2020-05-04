# Test file for "Lab6"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

test "9-->0 holding"
setPINA 0xFF
timeContinue 1
expectPORTB 7
setPINA 0xFE
timeContinue 1
expectPORTB 8
timeContinue 11
expectPORTB 9
setPINA 0xFD
timecontinue 1
expectPORTB 8
timeContinue 11
expectPORTB 7
timeContinue 11
expectPORTB 6
timeContinue 11
expectPORTB 5
timeContinue 11
expectPORTB 4
timeContinue 11
expectPORTB 3
timeContinue 11
expectPORTB 2
timeContinue 11
expectPORTB 1
timeContinue 11
expectPORTB 0
checkResult

test "0-->9 holding"
setPINA 0xFF
expectPORTB 0
setPINA 0xFE
timeContinue 1
expectPORTB 1
timeContinue 11
expectPORTB 2
timeContinue 11
expectPORTB 3
timeContinue 11
expectPORTB 4
timeContinue 11
expectPORTB 5
timeContinue 11
expectPORTB 6
timeContinue 11
expectPORTB 7
timeContinue 11
expectPORTB 8
timeContinue 11
expectPORTB 9
checkResult

test "9-->5 clicking, reset, 0-->5 clicking"
setPINA 0xFF
expectPORTB 9
setPINA 0xFD
timeContinue 1
expectPORTB 8
setPINA 0xFF
timeContinue 1
expectPORTB 8
setPINA 0xFD
timeContinue 1
expectPORTB 7
setPINA 0xFF
timeContinue 1
expectPORTB 7
setPINA 0xFD
timeContinue 1
expectPORTB 6
setPINA 0xFF
timeContinue 1
expectPORTB 6
setPINA 0xFD
timeContinue 1
expectPORTB 5
setPINA 0xFF
timeContinue 1
expectPORTB 5
setPINA 0xFC
timeContinue 1
expectPORTB 0
setPINA 0xFF
timeContinue 1
expectPORTB 0
setPINA 0xFE
timeContinue 1
expectPORTB 1
setPINA 0xFF
timeContinue 1
expectPORTB 1
setPINA 0xFE
timeContinue 1
expectPORTB 2
setPINA 0xFF
timeContinue 1
expectPORTB 2
setPINA 0xFE
timeContinue 1
expectPORTB 3
setPINA 0xFF
timeContinue 1
expectPORTB 3
setPINA 0xFE
timeContinue 1
expectPORTB 4
setPINA 0xFF
timeContinue 1
expectPORTB 4
setPINA 0xFE
timeContinue 1
expectPORTB 5
setPINA 0xFF
timeContinue 1
expectPORTB 5
checkResult

test "going over under"
setPINA 0xFC
timeContinue 1
expectPORTB 0
setPINA 0xFD
timeContinue 1
expectPORTB 0
timeContinue 11
expectPORTB 0
setPINA 0xFE
timeContinue 100
expectPORTB 9
timeContinue 11
expectPORTB 9
setPINA 0xFF
timeContinue 1
expectPORTB 9
setPINA 0xFE
timeContinue 1
expectPORTB 9
checkResult

# Add tests below

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
