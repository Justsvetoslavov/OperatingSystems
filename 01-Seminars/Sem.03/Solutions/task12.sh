cat /etc/passwd | grep $(whoami)

cat /etc/passwd | grep $(id -u)

cat /etc/passwd | grep $(id -u) -B 2

cat /etc/passwd | grep $(id -u) -B 2 -A 3

cat /etc/passwd | grep $(id -u) -B 2 | head -n 1