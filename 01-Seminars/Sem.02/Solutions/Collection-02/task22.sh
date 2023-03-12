cp $(find /etc -type f -exec wc -c {} \; 2> /dev/null | sort -n | head -n 1 | cut -d ' ' -f 2) ~

# Copy all files that are with minimum size, not just the first one
find /etc -type f -printf "%s %p\n" 2>/dev/null | sort -n | awk 'NR==1 {min=$1} $1==min {print $2}' | xargs -I {} cp {} ~ 2>/dev/null
#or
find /etc -type f -exec du -b {} + 2>/dev/null | sort -n | awk 'NR==1 {min=$1} $1==min {print $2}' | while read file; do cp "$file" ~/playground/ 2>/dev/null; done