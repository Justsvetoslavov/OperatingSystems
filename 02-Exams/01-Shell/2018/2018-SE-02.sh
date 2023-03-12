#Note: sort -nr -k3,3:
# The 3 before the comma indicates that sorting should start at the third field.
# The 3 after the comma specifies that sorting should end at the third field. This ensures that only the third field is used for sorting, not any subsequent fields.
find /home/pesho -type f -printf "%i %n %T@ %p\n" | awk '$2 > 1' | sort -k3,3nr | head -n 1 | awk '{print $1}'