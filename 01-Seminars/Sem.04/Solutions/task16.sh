# Solution 1

#!/bin/bash

users=$(who | awk '{print $1}' | sort -u)

echo "<table>"
echo "  <tr>"
echo "    <th>Username</th>"
echo "    <th>group</th>"
echo "    <th>login shell</th>"
echo "    <th>GECOS</th>"
echo "  </tr>"
while read username ; do
        echo "  <tr>"
        groupId=$(egrep "^${username}" /etc/passwd | awk -F ':' '{print $4}')
        group=$(egrep "${groupId}:$" /etc/group | awk -F ':' '{print $1}')
        loginShell=$(egrep "^${username}" /etc/passwd | awk -F ':' '{print $NF}')
        fullName=$(egrep "^${username}" /etc/passwd | awk -F ':' '{print $5}' | awk -F ',' '{print $1}')
        echo "    <td>${username}</td>"
        echo "    <td>${group}</td>"
        echo "    <td>${loginShell}</td>"
        echo "    <td>${fullName}</td>"
        echo "  </tr>"
done < <(echo "${users}")
echo "</table>"

#=================================================================================================#

# Solution 2

#!/bin/bash

users=$(who | awk '{print $1}' | sort -u)

echo "<table>"
echo "  <tr>"
echo "    <th>Username</th>"
echo "    <th>group</th>"
echo "    <th>login shell</th>"
echo "    <th>GECOS</th>"
echo "  </tr>"

passwd="/etc/passwd"
while read user; do
        username=$(egrep $user $passwd | cut -d ':' -f 1)
        group=$(grep $(egrep $user $passwd | cut -d ':' -f 4) /etc/group | cut -d ':' -f 1)
        loginShell=$(egrep $user $passwd | cut -d ':' -f 7)
        gecos=$(egrep $user $passwd | awk -F ':' '{print $5}' | awk -F ',' '{print $1}')
        echo "  <tr>"
        echo "    <td>$username</td>"
        echo "    <td>$group</td>"
        echo "    <td>$loginShell</td>"
        echo "    <td>$gecos</td>"
        echo "  </tr>"
done< <(echo "$users")
echo "</table>"
