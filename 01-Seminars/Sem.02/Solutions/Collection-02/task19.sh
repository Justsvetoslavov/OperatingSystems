mkdir -p ~/myetc
find /etc -type f -perm -444 -exec cp -r {} myetc/ \; 2>/dev/null

#or

mkdir -p ~/myetc
find /etc -type f -perm -a=r -exec cp -r {} myetc/ \; 2>/dev/null