find ~ -mindepth 1-type d | xargs -I{} chmod 755 {}
#or
find ~ -mindepth 1 -type d -exec chmod 755 {} \;
