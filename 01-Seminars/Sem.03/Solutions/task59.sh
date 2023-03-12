ps -e -g root -o rss= | awk 'BEGIN{sum = 0} {sum+=$1} END{print sum}'
#or
ps -e -g root -o rss= | awk 'BEGIN{sum = 0} {sum+=$0} END{print sum}'
#or
ps -e -g root -o rss= | sed 's/ //g' | awk 'BEGIN{sum = 0} {sum+=$1} END{print sum}'

