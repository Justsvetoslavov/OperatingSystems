# Read the file "emails.txt" and process each line
# Filter lines that match the  pattern for valid email addresses
# Exclude lines that contain two or more consecutive dots ("..")
cat emails.txt | grep '^[_0-9a-zA-Z][a-zA-Z0-9_.-]*[a-zA-Z0-9_]@[a-z0-9A-Z][a-zA-Z0-9_.-]*.[a-zA-Z]*$' | grep -v '.*[.][.].*'
