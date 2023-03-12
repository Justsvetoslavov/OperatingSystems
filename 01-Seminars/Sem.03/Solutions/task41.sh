cat population.csv | cut -d ',' -f 3,4 | tr ',' ' ' | awk 'BEGIN{value=0} {if($1==2008) {value+=$2}} END{print value}'
#or
cat population.csv | awk -F ',' 'BEGIN{counter=0} {if($3==2008) {counter+=$4}} END{print counter}'