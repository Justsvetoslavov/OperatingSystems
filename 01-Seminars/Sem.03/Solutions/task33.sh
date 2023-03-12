# %T@: Prints the modification time of each file in seconds since the epoch.
# This reflects when the contents of the file were last modified.
This reflects when the contents of the file were last modified.
find ~ -printf "%T@ %f\n" | sort -k 1 -nr | cut -d ' ' -f 2 | head -n 10

# Off-topic to the task.
# %A@: Prints the access time of each file in seconds since the epoch.
# This reflects when the file was last read or accessed.
# The command sorts files by their access time, showing the most recently accessed files.
find ~ -printf "%A@ %f\n" | sort -k 1 -nr | cut -d ' ' -f 2 | head -n 10

# Key Difference:
#   %T@ sorts files based on when they were last modified.
#   %A@ sorts files based on when they were last accessed.