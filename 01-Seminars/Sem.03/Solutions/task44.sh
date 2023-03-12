cat population.csv | grep "1969" | sort -t ',' -k 4 -nr | head -n 42 | tail -n 1 | cut -d ',' -f 1,4 | tr ',' ' '
