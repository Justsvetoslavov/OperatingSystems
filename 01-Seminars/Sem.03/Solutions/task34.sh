find /usr/include -type f -name "*.[ch]" -exec cat {} \; | wc -l
#or
find /usr/include -type f | grep -o ".*\.[ch]" | xargs -I {} cat {} 2>/dev/null \; | wc -l
#or
cat $(find /usr/include -name '*.[ch]') |  wc -l