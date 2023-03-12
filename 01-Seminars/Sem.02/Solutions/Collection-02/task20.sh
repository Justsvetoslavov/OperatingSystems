tar -c -f c_start.tar $(find myetc/ -iname 'c*') 
# or
tar -caf c_start.tar /etc/c*

# Deletion
rm -rf myetc/
rm -i c_start.tar