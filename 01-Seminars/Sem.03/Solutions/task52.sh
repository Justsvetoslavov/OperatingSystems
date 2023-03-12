find /etc -type f -printf "%s %f %m\n" 2>/dev/null | sort -nr | head -n 1 | cut -d ' ' -f 3 | xargs -I{} find . -perm {}
