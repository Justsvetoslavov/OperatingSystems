head -n 12 /etc/passwd

# head -c is for bytes, which isn't the same as character
head -c 26 /etc/passwds
# cut -c lists first 26 characters and then we pipe for only first line (first 26 characters)
cut -c 1-26 /etc/passwd | head -n 1

head -n -4 /etc/passwd

tail -n 17 /etc/passwd

# can check with wc -l /etc/passwd if we have 151 lines
head -n 151 /etc/passwd | tail -n 1

head -n 13 /etc/passwd | tail -n 1 | tail -c 5
#or
head -n 13 /etc/passwd | tail -n 1 | rev | cut -c 1-4 | rev