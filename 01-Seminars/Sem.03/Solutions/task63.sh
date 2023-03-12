ps -e -o pid=,tty=,comm= | awk '{if($2=="?") print $1 " " $3}' | sort -k 2 -u
