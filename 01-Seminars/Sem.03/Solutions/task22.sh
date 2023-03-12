# Create directory & files
mkdir -p dir5
echo -e '1\n2\n3' > dir5/f1
echo -e 's\na\nd\nf' > dir5/f2
echo -e '3\n2\n1\n45\n42\n14\n1\n52' > dir5/f3

# Statistic of lines, words, symbols of each file
wc dir5/* | head -n -1 
# or
wc dir5/f[123] | head -n -1
# or
printf "lines: $(wc -l  dir5/file1)\n" && printf "words: $(wc -w  dir5/file1)\n" && printf "symbols: $(wc -c  dir5/file1)\n"
printf "lines: $(wc -l  dir5/file2)\n" && printf "words: $(wc -w  dir5/file2)\n" && printf "symbols: $(wc -c  dir5/file2)\n"
printf "lines: $(wc -l  dir5/file3)\n" && printf "words: $(wc -w  dir5/file3)\n" && printf "symbols: $(wc -c  dir5/file3)\n"

# Lines & symbols of each file
wc -lc *
# Lines & symbols for all files
wc -lc * | tail -n 1 | cut -d ' ' -f 1,2

# Lines of all files
wc -l * | tail -n 1 | cut -d ' ' -f 1