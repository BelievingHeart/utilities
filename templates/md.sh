#!/bin/bash
SET_DIR=0
LS=0
MODE=READ
SENT_HELP=0
NEW_FILE=0
OPEN_FOLDER=0
while getopts "fhln:ws:" opt 
do
case $opt in
    f) OPEN_FOLDER=1 ;;
    s) MARKDOWN_DIR=$OPTARG;SET_DIR=1 ;;
    l) LS=1 ;;
    w) MODE=WRITE ;;
    n) NEW_FILE=1;NEW_FILE_NAME=$OPTARG ;;
    *) SENT_HELP=1
esac
done

# help docs
if [ $SENT_HELP -eq 1 ]
then

cat <<End-of-message
------------------------------
\$1 :   tag to match
[-s:]   set the directory of .md files
[-l ]   list files
[-n ]   create a new markdown file
[-w ]   WRITE mode, default READ mode
[-f ]   open markdown folder
------------------------------
End-of-message

exit 0
fi

# set up paths
prefix=@INSTALL_DIR@
log_file="$prefix"/config/MARKDOWN_DIR
script="$prefix"/scripts/find_tag.py

# set up markdown directory
if [ $SET_DIR -eq 1 ]
then
    if [ -d $MARKDOWN_DIR ]
    then
        echo "export MARKDOWN_DIR=$MARKDOWN_DIR" > $log_file
        echo "MARKDOWN_DIR is set to $MARKDOWN_DIR"
        exit 0
    else
        echo "$MARKDOWN_DIR is not a valid directory" && exit 1
    fi
else
    . $log_file
fi

# open markdown directory
if [ $OPEN_FOLDER -eq 1 ]
then 
 xdg-open $MARKDOWN_DIR
 exit 0
fi

# create new markdown
if [ $NEW_FILE -eq 1 ]
then
    NEW_FILE_PATH="$MARKDOWN_DIR/$NEW_FILE_NAME".md
    touch $NEW_FILE_PATH
    echo  '[//]: # ()' > $NEW_FILE_PATH
    code $NEW_FILE_PATH
    exit 0
fi

# list mode
if [ $LS -eq 1 ]
then
    
    echo "ALL MARKDOWNS: "
    FILE_NAMES=($(cd $MARKDOWN_DIR && ls *.md))

    # select
    SELECTED=0
    N_ITEMS=$((${#FILE_NAMES[@]}-1))
    select n in ${FILE_NAMES[@]}
    do
        for i in $( seq 0 $N_ITEMS )
        do
            if [ $((REPLY-1)) -eq $i ]
            then
                FILE_TO_OPEN=${FILE_NAMES[$i]}
                SELECTED=1
                break
            fi
        done
        if [ $SELECTED -eq 1 ]
        then
            break
        fi
    done

    if [ $MODE = WRITE ]
    then
        code $MARKDOWN_DIR $MARKDOWN_DIR/$FILE_TO_OPEN
    else
        # xdg-open $MARKDOWN_DIR/$FILE_TO_OPEN
        echo $(xdg-open $MARKDOWN_DIR/$FILE_TO_OPEN) > /dev/null
    fi


    exit 0
fi


# search tags
shift $((OPTIND -1)) #get rid off all the option-related arguments, this works because they are all in the front
tag=$1
if [ -z $1 ]
then
    code $MARKDOWN_DIR 
else
    SEARCH_RESULTS=($(python3 $script $MARKDOWN_DIR $tag))
    if [ $? -eq 2 ]
    then
        echo "Can not find any match of <$tag>" && exit 2
    fi

    # get rid of file prefixes and ask usr to select
    index=0
    for r in ${SEARCH_RESULTS[@]}
    do
        FILE_NAMES[$index]=${r##*/}
        index=$((index+1))
    done

    # select
    SELECTED=0
    N_ITEMS=$((${#FILE_NAMES[@]}-1))
    select n in ${FILE_NAMES[@]}
    do
        for i in $( seq 0 $N_ITEMS )
        do
            if [ $((REPLY-1)) -eq $i ]
            then
                FILE_TO_OPEN=${SEARCH_RESULTS[$i]}
                SELECTED=1
                break
            fi
        done
        if [ $SELECTED -eq 1 ]
        then
            break
        fi
    done

    if [ $MODE = WRITE ]
    then 
        code $MARKDOWN_DIR $FILE_TO_OPEN
    else
        echo $(xdg-open $FILE_TO_OPEN) > /dev/null
    fi
fi