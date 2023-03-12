ps -e -g root -o rss= | awk 'BEGIN{sum = 0; counter = 0} {sum += $1; counter += 1} END{if(counter > 0) print (sum/counter); else print "No entries"}'
#or
ps -e -g root -o rss= | sed 's/ //g' | awk 'BEGIN{sum=0;counter=0} {sum+=$1; counter+=1} END{if(counter!=0) printf "%.2f\n", sum/counter}'
