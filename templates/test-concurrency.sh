#!/bin/bash
if [[ -z $1 ]]
then
cat <<End-of-message
------------------------------
\$1 :   How many times to repeat test
\$2 :   The binary to test
------------------------------
End-of-message
else
for (( i = 0; i < $1; i++ )) 
do
    $2 > /dev/null
    echo -n .
    if [[ $? -ne 0 ]]
    then 
        echo "Bug appeared at the $i test"
        exit 1
    fi
done
echo "Test success"
exit 0
fi


