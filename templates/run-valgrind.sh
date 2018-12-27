#!/bin/bash
if [ -z $1 ]
then

cat <<End-of-message
-----------------------------------------------------------------------------
Run memcheck, helgrind and DRD of valgrind on your cxx runtime

Usage:
\$1 :   cxx runtime

NOTE:
There might be a lot a false positives in the drd log when std::thread is used
-----------------------------------------------------------------------------
End-of-message

exit 2

else
    NAME=${1##*/}
    valgrind --log-file="$NAME"_memcheck.log --read-var-info=yes --leak-check=full $1
    valgrind --tool=helgrind --read-var-info=yes --log-file="$NAME"_helgrind.log $1
    valgrind --tool=drd --log-file="$NAME"_drd.log --read-var-info=yes $1
    echo 'WARNING: There might be a lot a false positives in the drd log when std::thread is used'
fi

exit 0