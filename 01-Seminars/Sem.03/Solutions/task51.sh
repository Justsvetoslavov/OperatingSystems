find . -type f -printf "%f\n" | cut -d '-' -f 1 | sort | uniq | tr -d ' ' | xargs -I{} mkdir ~/{}
