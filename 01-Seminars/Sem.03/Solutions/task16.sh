cat /etc/passwd | cut -d ':' -f 5 | cut -d ',' -f 1 | cut -d ' ' -f 2 | grep -E -v '^.{8,}$' | grep -v '^$' |  xargs -I{} grep -w {} /etc/passwd
#or
cat /etc/passwd | cut -d ':' -f5 | cut -d ',' -f1 | cut -d ' ' -f2 | grep -v -E '.{8,}$' | grep -E '[^a-zA-Z]' | sort | uniq | xargs -I{} grep -w {} /etc/passwd