# In awk, you cannot directly assign the output of an external command to a variable using the $(command) syntax 
# like you can in the shell. Instead, you have to use getline with a command pipe.

cat /etc/group | awk -F ':' 'BEGIN {
    "id -gn" | getline gname
    close("id -gn")
    
    "id -g" | getline gid
    close("id -g")
} 
{
    if (gname == $1 && gid == $3) {
        print "Hello, "$1" - I am here!"
    } else {
        print "Hello, "$1" "
    }
}'
#or
cat /etc/group | awk -F ':' 'BEGIN {
    "id -gn" | getline gname
    close("id -gn")
    
    "id -g" | getline gid
    close("id -g")
} 
{
    if (gname == $1 && gid == $3) {
        print "Hello, "$1" - I am here!"
    } else {
        print "Hello, " $3
    }
}'
# or
cat /etc/group | cut -d ':' -f 1 | awk -v gname="$(id -gn)" '{if($1==gname) {print"Hello, "$1" - I am here"} else {print "Hello, " $1} }'