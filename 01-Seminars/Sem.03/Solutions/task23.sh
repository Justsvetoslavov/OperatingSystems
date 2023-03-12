# Convert lowercase to uppercase, however, not inplace
cat file2 | tr 'a-z' 'A-Z' > file2

# Proper solution, using sed -i
sed -i 's/[a-z]/\U&/g' dir5/file2