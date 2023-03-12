# Note: You can use rm -i for prompt whether to delete ( applicable for both A) & B) )

# A)
find . -type f -printf "%p %s\n" | awk '$2 == 0 {print $1}' | xargs -I{} rm -f {}
#or
find . -mindepth 1 -type f -size 0 -exec rm -rf {} \;
#or
find . -type f -size 0 -delete
#or
rm -fv $(find . -mindepth 1 -type f -size 0) 

# B)
find ~ -mindepth 1 -type f -printf "%p %s\n" 2>/dev/null | sort -nr -k 2 | head -n 5 | xargs -I {} rm -if {}
# or
find ~ -mindepth 1 -type f -printf "%p %s\n" | sort -nr -t ' ' -k 2 | head -n 5 | cut -d ' ' -f 1 | xargs -I {} rm -if {}
