ps -e -o pid=,ppid= | awk '{if($1 > $2) print $1}'
