find /etc/ -type f -exec wc -l {} \;
# or
wc -l $(find /etc/ -type f)