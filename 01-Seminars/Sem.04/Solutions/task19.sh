#!/bin/bash

# Проверка за минимален брой аргументи
if [ "$#" -lt 1 ]; then
    echo "Моля, подайте поне един файл като аргумент."
    exit 1
fi

# Прочитане на символен низ от стандартния вход
read -p "Моля, въведете низ: " searchString

# Обработка на всеки предоставен файл
for file in "$@"; do
    # Проверка дали файла съществува
    if [ ! -f "$file" ]; then
        echo "Файлът $file не съществува."
        continue
    fi
    
    # Броене на редовете, които съдържат низа
    count=$(grep -E "$searchString" "$file" | wc -l)

    # Извеждане на резултата
    echo "Файл ${file}: ${count} реда съдържат низа ${searchString}."
done

#or 

#!/bin/bash

read -p "String: " string

while [ ${#} -ne 0 ] ; do
        FILE=${1}
        shift
        echo "${FILE} - occurances: $(cat ${FILE} | grep -E "${string}" | wc -l)"
done