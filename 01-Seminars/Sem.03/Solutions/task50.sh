find . -type f | cut -d '/' -f 2 | grep -E "Pink -|Beatles" | cut -d '(' -f 2 | cut -d '.' -f 1 | tr -d ')'
