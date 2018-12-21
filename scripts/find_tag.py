from glob import glob
import sys,os
dir = sys.argv[1]
tag = sys.argv[2]

tag_dict={}
for file_name in glob(os.path.join(dir,"*")):
    if file_name.endswith(".md"):
        with open(file_name) as f:
            tag_dict[file_name]=f.readline()

meet_tag = []
for key,val in tag_dict.items():
    if tag in val:
        meet_tag.append(key)
if meet_tag != []:
    print("The list found for <",tag,">:")
    for i,file_name in enumerate(meet_tag):
        print(i+1,". ",file_name)
else:
    print("Could not find any markdown meets <",tag,">")