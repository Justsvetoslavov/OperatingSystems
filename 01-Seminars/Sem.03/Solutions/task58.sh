ps -e -o comm --sort=etimes | tail -n 1
#or
ps -e --sort=etimes -o comm | tail -n 1
#or 
ps -e --sort=etimes -o cmd | tail -n 1 # this one shows arguments