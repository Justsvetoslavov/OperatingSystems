find ~ -maxdepth 1 -type f -user $(whoami) 2>/dev/null | xargs -I {} chmod {}
#or
find ~ -mindepth 1 -maxdepth 1 -type f -user $(id -u) 2>/dev/null | xargs -I{} chmod 664 {}
#or 
find ~ -mindepth 1 -maxdepth 1 -type f -user $(id -u) 2>/dev/null -exec chmod 664 {} \;