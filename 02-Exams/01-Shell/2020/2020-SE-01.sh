find ~ -mindepth 1 -type f -perm 644 2>/dev/null | xargs -I {} chmod g+w {}
#or 
find ~ -mindepth 1 -type f -perm 644 2>/dev/null -exec chmod g+u {} \;
