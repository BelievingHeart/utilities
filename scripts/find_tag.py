from glob import glob
import sys,os
dir = sys.argv[1]
tag = sys.argv[2]

tag_dict={}
for file_name in glob(os.path.join(dir,"*.md")):
    # if file_name.endswith(".md"):
        with open(file_name) as f:
            tag_dict[file_name]=f.readline()

meet_tag = []
for key,val in tag_dict.items():
    if tag in val:
        meet_tag.append(key)
if meet_tag != []:
    for file_name in meet_tag:
        print(file_name)
else:
    print("Could not find any markdown meets <",tag,">")
    sys.exit(2)

    #exit 2 can not find any matching tag