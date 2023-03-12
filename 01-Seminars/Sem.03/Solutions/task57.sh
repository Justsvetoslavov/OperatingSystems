ps -e -o pid=,args=,vsz= --sort=vsz | tail -n 1 | cut -d ' ' -f 2,3
#or 
ps -e --sort=vsz -o pid,cmd | tail -n 1 | cut -d ' ' -f 2,3
#or
ps -e -o vsz=,pid=,cmd= | tr -s ' ' | sort -k 1 -nr | head -n 1 | cut -d ' ' -f 2,3