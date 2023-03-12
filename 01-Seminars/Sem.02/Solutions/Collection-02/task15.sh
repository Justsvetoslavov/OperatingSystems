find /tmp -type f -group 512 -perm /g=w,o=w

#or

find /tmp -type f -group $(id -g) -perm /022 2>/dev/null