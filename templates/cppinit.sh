#!/bin/bash
# This script will initialize a cpp project folder with my preferred files

if [ -z $1 ]
then    
    echo 'What is the c++ porject name?'
    read PROJECT_NAME
else
    PROJECT_NAME=$1
fi 

mkdir $PROJECT_NAME
prefix=@INSTALL_DIR@
CONFIG_FILE_DIR="$prefix"/cpp_env

# config project_name of CMakeLists.txt
CMAKE_LISTS_IN="$prefix"/config/CMakeLists.txt.in
CMAKE_LISTS_OUT="$prefix"/cpp_env/CMakeLists.txt
sed "s/project_name/$PROJECT_NAME/g" $CMAKE_LISTS_IN > $CMAKE_LISTS_OUT

cp -r $CONFIG_FILE_DIR/. $PROJECT_NAME
git init $(pwd)/$PROJECT_NAME
echo 'Created c++ project folder:'
echo \<$(pwd)/$PROJECT_NAME\> is sent to the clipboard

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    clipboard="xclip -sel clip"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    clipboard="pbcopy"
fi
printf $(pwd)/$PROJECT_NAME | $clipboard