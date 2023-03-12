grep -E "^#" /etc/services # | wc -l
# or
cat /etc/services | grep "^#" # | wc -l

# The above gives us only lines that are comments, however, if we have a comment as in:
# <code> #comment
# We are not catching those cases.

cat /etc/services | grep -o "#.*$" | sed -E 's/^(.)(.*)$/\2/g'
#or
cat /etc/services | grep -o "#.*$" | cut -c 2-