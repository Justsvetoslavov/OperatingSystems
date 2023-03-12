# Cat -> truncate -> grep -> save to a new file
# Not inplace and not the best solution
cat file3 | tr -d '1' | grep -E -v '^$' > file3-New.txt # But it is in a new file

# Proper solution:
sed -i 's/1//g' dir5/file3