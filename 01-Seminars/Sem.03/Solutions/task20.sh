find /bin | xargs -I{} file {} | grep "shell script" | wc -l
#or
find /bin -type f -exec cat {} \; | grep "#!/bin/" | wc -l

#Колко файлове в дадена директория са ASCII text?
find /bin | xargs -I{} file {} | grep "ASCII text executable" | wc -l