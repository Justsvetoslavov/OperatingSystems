cat planets.txt | sort -k 3 -nr -t ';' -r | uniq -i | grep -E $(sort -k 3 -nr -t ';' -r planets.txt | cut -d ';' -f 2 | head -n 1) | tail -n -1 | cut -d ';' -f 1,4 | tr ';' \t
#or
cat planets.txt | sort -k3n -t ';' | grep $(sort -k3nr -t ';' planets.txt | head -n 1 | cut -d ';' -f2) | head -n 1 | tr ';' ' ' | awk '{print $1 "\t" $4}'
#or
cat planets.txt | tail -n +2 | sort -k3nr -t ';' | head -n1 | awk -F ';' '{print $2}' | xargs -I{} awk -F ';' '{if ($2=="{}") print $0}' planets.txt | sort -k3n -t ';' | head -n1 | awk -F ';' '{print $1 "\t" $4}'