cat $(find /usr -type f -iname "*.sh") | grep -E "^#!" | tr -d " " | sort | uniq -c | sort -n | tail -n 1 | cut -d '!' -f 2
#or
find /usr/ -iname "*.sh" | xargs -I{} head -n 1 {} | grep "^#!" | tr -d " " | sort | uniq -c | sort -nr | head -n 1 | cut -d '!' -f2
#or
find /usr/ -iname '*.sh' -exec head -n 1 {} \; | grep "^#!" | cut -d "!" -f 2 | sort | uniq -c | sort -nr | head -n 1 | awk '{print $2}'