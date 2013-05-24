#!/bin/bash

testUtmCgi()
{
    local utmCgiRegex='^type=(1|23|7[01256])&'
    local testString=""

    # test valid cases.
    local valid="1 23 70 71 72 75 76"
    for i in $valid; do
        testString="type=$i&"
        [[ "$testString" =~ $utmCgiRegex ]]
        assertEquals "test valid" 0 $?
    done

    # test invalid cases.
    local invalid="type=12 type=1a type=2 type=3 type=234 type=233
                   type=7 type=73 type=74 type=722 type=723 atype=1"
    for i in $invalid; do
        testString=$i
        [[ "$testString" =~ $utmCgiRegex ]]
        assertEquals "Failure test string: [$testString]" 1 $?
    done
}

source shunit2

