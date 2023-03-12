cat population.csv | grep "BGR" | sort -t ',' -k 4 -n | tail -n 1 | cut -d ',' -f 3
