cat population.csv | grep "2016" | sort -t ',' -k 4 -nr | head -n 1 | cut -d ',' -f 1
#and
cat population.csv | grep "2016" | sort -t ',' -k 4 -nr | tail -n 1 | cut -d ',' -f 1
