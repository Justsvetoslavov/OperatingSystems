cat /etc/passwd | grep "/home/SI/" | awk -F ':' '{print $6}' | xargs -I {} find {} -maxdepth 0 -type d -printf "%T@ %p\n" | \
awk '{if ($1<"1551176100" && $1 >"1551168000") print $2}' | xargs -I {} grep :{}: /etc/passwd | \
awk -F ':' '{print $1 " " $5}' | awk '{print $1 " " $2 " " $3}' | awk -F ',' '{print $1}'