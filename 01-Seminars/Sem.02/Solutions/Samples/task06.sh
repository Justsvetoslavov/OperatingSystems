find ~ -type f -user $(whoami) -exec tail -n 5 {} \;
