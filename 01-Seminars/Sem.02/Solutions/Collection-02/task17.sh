find ~ -newer practice/01/f3 -type f -exec rm -i {} \;
#or
rm -i $(find ~ -type f -newer practice/01/f3)

# Alternative: Does not ask for confirmation to delete
find ~ -newer practice/01/f3 -type f -delete
