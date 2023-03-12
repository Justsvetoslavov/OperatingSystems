cat /srv/fmi-os/exercises/data/mypasswd.txt | egrep "/home/SI" | cut -d ':' -f 1 | sed -n 's/[^0-9]*\([0-9]\+\).*/\1/p' | sort -nr > si.txt
# or
cat /srv/fmi-os/exercises/data/mypasswd.txt | egrep "/home/SI" | cut -d ':' -f 1 | sed -n 's/[^0-9]*\([0-9]\+\).*/\1/p' | sort -n > si.txt

# Explanation of sed:
# sed -n 's/[^0-9]*\([0-9]\+\).*/\1/p': Processes each line to retain only the numeric part:
# sed -n:
#   The -n option tells sed to suppress automatic printing of each line.
#   Without -n, sed prints every line by default after applying the script.
#   With -n, only explicitly requested lines (via commands like p) are printed.
# s///: Represents the substitution command in sed.
# [^0-9]*: Matches any sequence of non-numeric characters.
# \([0-9]\+\): Matches and captures one or more numeric characters.
# .*: Matches the rest of the line.
# \1: Refers to the first captured group (the numeric sequence).
# /p: Instructs sed to print lines where the substitution was successful.