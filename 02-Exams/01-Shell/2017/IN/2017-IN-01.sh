find / -user $(whoami) 2>/dev/null | wc -l
#or
find -user $(id -u) 2>/dev/null | wc -l