#!/bin/bash
# This script will initialize a cpp project folder with my preferred files

if [ -z $1 ]
then    
    echo 'What is the c++ porject name?'
    read PORJECT_NAME
else
    PORJECT_NAME=$1
fi 

mkdir $PORJECT_NAME
prefix=@INSTALL_DIR@
CONFIG_FILE_DIR="$prefix"/cpp_env
cp -r $CONFIG_FILE_DIR/. $PORJECT_NAME
git init $(pwd)/$PORJECT_NAME
echo 'Created c++ project folder:'
echo \<$(pwd)/$PORJECT_NAME\> is sent to the clipboard
echo $(pwd)/$PORJECT_NAME | xclip -sel clip