cat emp.data | awk '{ for (i = NF; i >= 1; i -= 1) printf $i "\t" } { printf "\n" }'
#or
cat emp.data | awk '{ for (i = NF; i >= 1; i -= 1) printf "%s\t", $i } { printf "\n" }'

