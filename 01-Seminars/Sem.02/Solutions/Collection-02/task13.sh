find /etc/ -mindepth 1 -maxdepth 1 -type d | tail -n 10 > result.txt
# or
find /etc/ -mindepth 1 -maxdepth 1 -type d | tail > result.txt