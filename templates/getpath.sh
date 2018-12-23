#!/bin/bash
echo $(realpath $1) | xclip -sel clip
echo \<$1\> is sent to clipboard