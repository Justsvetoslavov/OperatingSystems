## Bash

// TODO: Въведение

## Променливи
- Всеки работещ shell има таблица с променливи
- Стойността на всяка променлива е низ (bash is stringly-typed)
- С командата $ set можем да видим всички променливи

Можем да зададем стойност на променлива с 
```
<име на променлива>=<стойност>
```
- Можем да реферираме променлива с ${<име на променлива>}
- работи по подобен начин като command substitution
```
$ name="Ivan Ivanov"
$ echo "my name is ${name}"
my name is Ivan Ivanov
```

## Видове променливи:
Променливите са два вида - обикновени променливи и environment променливи
- Обикновените променливи са само за текущия shell
- Environment променливите се наследяват от всички процеси, деца на текущия shell
- Всъщност, environment променливите са част от контекста на всеки процес, и се копират при fork
- Командата $env извежда цялата таблица с environment променливи на текущия shell
- $ export <име на променлива>=<стойност> задава стойностна environment променлива

При извикване на команда, shell-ът търси изпълним файл със съответното име в директориите, изброени в променливата ${PATH} <br />

Директориите се разделят със символа `:` <br />

Ако добавите някаква директория в ${PATH}, всички изпълними файлове в нея ще могат да се изпълняват като команди, без задаване на пълен път. <br />

### Environment променливи:
- ${SHELL} – пълен път до текущия shell
- ${USER} – потебителско име на текущия потребител
- ${HOME} – home директория на текущия потребител
- ${PS1} – низ, управляващ prompt-а пробвайте да го смените
- ${PATH} – пътища за търсене на команди
- ${PWD} – текуща директория
- ${IFS} – Internal Field Separator
- ${EDITOR} – текстов редактор по подразбиране

Параметри на скриптовете:
- Когато изпълняваме скрипт, можем да му подадем параметри, както на всяка друга команда: 
```
$ /path/to/my_script.sh param_1 param_2 ...
```
- Вътре в скрипта можем да достъпим параметрите от променливите \${1}, \${2}, \${3}
- Променливата `${0}` съдържа името (пътя) на самия скрипт
- Променливата `${#}` съдържа броя параметри
```
$ cat print_args.sh
#!/bin/bash
echo "num args: ${#}"
echo "arg 0: ${0}"
echo "arg 1: ${1}"
echo "arg 2: ${2}"
echo "arg 3: ${3}"

$ ./print_args foo bar
num args: 2д
arg 0: ./print_args
arg 1: foo
arg 2: bar
```

- Променливата `${@}` съдържа всички параметри, има специално поведение в кавички
    -  `"${@}"` се разпакетира до "\${1}" "\${2}" "\${3}" ...
- Променливата `${*}` също съдържа всички параметри
    - няма допълнително специално поведение
    - `"${*}"` се разпакетира до "\${1} \${2} \${3} ..."
- От двете, почти винаги искате да използвате ${*}
    - Use `${*}`(or `*`) when you want to treat the arguments as a single compound word. It's less commonly used than ${@} because such use cases are rarer.
    - Use \${@} (or "\$@") when you want to process or pass each argument separately. It's especially useful in loops or when forwarding arguments to another command. This is the more common and recommended usage in most cases to ensure arguments with spaces, wildcards, etc., are handled correctly.
    - To summarize, "\$@" retains the integrity of each individual argument, preserving spaces and other special characters within them, while "\$*" will combine all arguments into a single string, separated by the IFS delimiter (by default, a space). In most scripting scenarios, "\$@" is preferred because it maintains the original word boundaries of the arguments. 

## Stdin/Stdout
```
$ cat foo.sh
#!/bin/bash
echo -n "What's your name? "
read name
echo "Hello, ${name}"

# echo - stdout
# read - stdin
```

С вградената конструкция $ read -p <prompt> показва някакъв prompt на потребителя преди да чака за вход:
```
$ cat foo.sh
#!/bin/bash
read -p "What's your name?" name
echo "Hello, ${name}"
```

read може да прочете вход в няколко променливи:
```
$ cat add.sh
#!/bin/bash
read a b
echo "${a} plus ${b} is $(( a + b ))"
$ echo '42 26' | ./add.sh
42 plus 26 is 69
```

Входът, който read и някои други конструкции обработват, се разделя на думи
- границите между думите се определят от символите в съдържанието на променливата IFS
- можем да я променим, ако искаме текстът да се разделя по друг символ
- интересно: всъщност, IFS управлява и символите, по които се разделят аргументите на произволни команди
- Това е полезно, ако искаме да обработим текст, съдържащ полета, разделени с някакъв символ
```
$ cat add.sh

#!/bin/bash
old_IFS="${IFS}"
IFS=':'
read a b
IFS="${old_IFS}"

echo "${a} plus ${b} is $(( a + b ))"

$ echo '42:26' | ./add.sh
42 plus 26 is 69
```

read спира да чете, когато види символ за нов ред:
```
$ cat foo.sh
#!/bin/bash
read a
read b
echo "a: ${a}"
echo "b: ${b}"
$ echo -e 'baba\ndyado' | ./foo.sh
a: baba
b: dyado
```
- Опцията -d <символ> на read го кара да спира да чете, когато види дадения символ (вместо нов ред)

Вградената конструкция $(( <израз> )) пресмята произволен аритметичен израз и се заменя с резултата
В израза могат да участват променливи
```
$ cat foo.sh

#!/bin/bash
a=42
b=26

echo "result: $(( a + b + 1 ))"

$ ./foo.sh
result: 69
```

## Аритметика с цели числа
Вградената конструкция $(( <израз> )) пресмята произволен аритметичен израз и се заменя с резултата. <br />
В израза могат да участват променливи. <br />
```
$ cat foo.sh

#!/bin/bash
a=42
b=26
echo "result: $(( a + b + 1 ))"

$ ./foo.sh
result: 69
```

Можем да използваме външни команди за аритметика. <br />
Например, командата `$ bc` чете израз от stdin и връща резултата на stdout. Също така, поддържа десетични дроби. <br />
```
$ echo '3 + 5' | bc
8

$ echo '3 + 2.5' | bc
5.5

# Пример с използване на bc заедно с променливи:
$ cat foo.sh

#!/bin/bash
a=42.5
b=26
result="$(echo "${a} + ${b}" | bc)"
echo "result: ${result}"

$ ./foo.sh
69.5
```

## Exit status
- При приключване, всеки процес предава на родителя сичисло, наречено exit status
- Съответно всяка команда в shell-а има exit status
- По конвенция, exit status, равен на 0, означава успех, а всякакъв друг означава неуспех
- Променливата ${?} съдържа exit status-а от предната команда
- grep има exit status 0, ако е намерил редове, отговарящи на условието
    - Опцията -q на grep го кара да не извежда нищо
```
#!/bin/bash

grep -q '^ivan:' /etc/passwd
echo "status: ${?}"
```

Поведение при exit status
- По подразбиране, exit status-ът на поредица от команди,отделени с пайпове, е този на последната команда
- Можем да конфигурираме shell-а така, такава поредица да завършва неуспешно ако която и да е от командите е неуспешна
    - това се прави с $ set -o pipefail, например в началото на скрипта
- Можем да конфигурираме shell-а да прекратява скрипта в момента, в който някоя команда е неуспешна
    - това се прави с $ set -e
- Можем да конфигурираме shell-а да прекратява скрипта при рефериране на несъществуваща променлива
    - това се прави с $ set -u
```
#!/bin/bash
set -euo pipefail

# По този начин може да сме сигурни, че скриптовете ни ще се прекратяват при грешка
```
- Недостатъкът е, че ако искаме да проверим дали дадена команда е завършила с неуспех, тя задължително трябва да е част от логически израз или if

## Блокове
Конструкцията { <команда 1>; <команда 2>; ... <команда n>; } се нарича блок и третира поредицата от команди като една команда
```
$ { echo 'The time is:'; date; } | tr a-z A-Z
THE TIME IS: SUN 12 MAR 17:42:08 EET 2023
```

## Subshell
Конструкцията ( <команда 1>; <команда 2>; ... <команда n>; ) се нарича subshell и много прилича на блок
- също както блоковете, е начин да третираме поредица от команди като една команда
- разликата е, че командите се изпълняват в отделен shell от текущия

Тъй като subshell-ът е отделен shell, той наследява копия на променливите от оригиналния shell, които можем да променяме без тези промени да се отразят на оригиналната
променлива.

```
# Ето примера с IFS от по-рано, но вместо да запазваме старото състояние на IFS, използваме subshell:

$ cat add.sh

#!/bin/bash

(
    IFS=':'
    read a b
    echo "${a} plus ${b} is $(( a + b ))"
)

$ echo '42:26' | ./add.sh
42 plus 26 is 69
```

## Process substitution
От по-рано знаем конструкцията command substitution – (<команда>), която се заменя с низ, който е съдържанието на stdout на командата
Сега ще покажем още две конструкции, наречени process substitution
- \>(<команда>) се заменя с низ, който е име на виртуален файл, представляващ тръба, закачена към stdin на командата
- <(<команда>) се заменя с низ, който е име на виртуален файл, представляващ тръба, закачена към stdout на командата

```
# Така можем да вземем имената на потребителите, които не са логнати в момента:
$ comm -2 -3 <(cat /etc/passwd | cut -d : -f 1 | sort) \ <(who | cut -d ' ' -f 1 | sort)
pesho
gosho

# Всъшност, резултата от тази конструкция е просто име на файл:
$ echo <(whoami)
/dev/fd/63
```

```
$ whoami | tr a-z A-Z
IVAN

# same as

$ tr a-z A-Z < <(whoami)
IVAN
```

## Не, И, ИЛИ
- ! <команда> изпълнява командата и
    - има status 0, ако командата е имала не-нулев статус
    - има status 1, ако командата е имала нулев статус
- <команда 1> || <команда 2> има семантика на “ИЛИ”
    - ако команда 1 е била успешна, целият израз е успешен и команда 2 не се изпълнява
    - ако команда 1 е била неуспешна, се изпълнява команда 2 и се гледа нейният статус
- <команда 1> && <команда 2> има семантика на “И”
    - ако команда 1 е била успешна, целият израз е успешен и команда 2 не се изпълнява
    - ако команда 1 е била неуспешна, се изпълнява команда 2 и се гледа нейният статус
- Ляво-асоциативни са

# Проверка/Test
- Сравнение на числа (-eq, -ne, -gt, -lt, -le, -ge)
```
$ [[ 42 -eq 26 ]] && echo yes || echo no
no

$ [[ 42 -gt 26 ]] && echo yes || echo no
yes
```

- Лексикографско сравнение на низове (==, <, >)
```
$ [[ "$(whoami)" == "ivan" ]] && echo yes || echo no
yes

$ [[ "foo" > "bar" ]] && echo yes || echo no
yes
```

- Операции за низове
    - [[ -n <низ> ]]
        - проверява дали низ е не-празен;
    - [[ -z <низ> ]]
        - проверява дали низ е празен;
    - [[ <низ/променлива> =~ <pattern> ]]
        - checks if a string matches a regex pattern.

- Операции за файлове
    - [[ -f <име> ]]
        - проверява дали обект с такова име съществува и е файл;
    - [[ -d <име> ]]
        - проверява дали обект с такова име съществува и е директория;
    - [[ -r <име> ]]
        - проверява дали можем да четем файл;
    - [[ -w <име> ]]
        - проверява дали можем да пишем файл;
    - [[ -x <име> ]]
        - проверява дали можем да изпълним файл;
    - [[ -e <име> ]]
        - проверява дали <име> съществува, независимо от типа на файла (файл, директория, symlink, etc);
```
[[ "$(whoami)" != "root" ]] && {
    echo "This script must only be run as root!" >&2
    exit 1
}
```

## if-clause
Конструкцията if във bash изглежда така:
```
if <команда>; then
    <блок от команди>
else
    <блок от команди>
fi
```
Можем и да изпуснем else-блока:
```
if <команда>; then
<блок от команди>
fi
```

Exit status-ът на командата-условие определя дали да се изпълни then-блока или else-блока. <br />

Символът за нов ред и ; са взаимно заменяеми - следните 3 конструкции са еднакви: <br />
```
if grep -q '^ivan:' /etc/passwd
then
    echo yes
else
    echo no
fi

if grep -q '^ivan:' /etc/passwd; then
    echo yes
else
    echo no
fi

if grep -q '^ivan:'; then echo yes; else echo no; fi
```

```
#!/bin/bash
read -p "Are you sure you want to continue? (yes/no)" answer

if [[ "${answer}" != "yes" ]]; then
    echo "Coward!"
else
    rm -rf /*
fi
```

## Case
Конструкцията case в bash изглежда така:
```
case <низ> in
    <glob1>)
        <блок>
        ;;
    <glob2>)
        <блок>
        ;;
    ...
    <globN>)
        <блок>
        ;;
esac
```

```
case "${1}" in
    start)
        echo "starting service"
        ;;
    stop)
        echo "stopping service"
        ;;
    restart)
        echo "restarting service"
        ;;
    *)
        echo "Usage: ${0} (start|stop|restart)"
        exit 1
    ;;
esac
```

## For-loop
Конструкцията for в bash изглежда така:
```
for <име на променлива> in <аргументи>; do
    <блок>
done
```

```
$ cat foo.sh
#!/bin/bash
for name in 'Pesho' 'Gosho' 'Penka'; do
    echo "${name} is awesome!"
done

$ ./foo.sh
Pesho is awesome!
Gosho is awesome!
Penka is awesome!
```

```
$ cat foo.sh
#!/bin/bash
for name in "${@}"; do
    echo "${name} is awesome!"
done

$ ./foo.sh 'Pesho Peshkov' 'Gosho Goshkov' 'Penka Penkova'
Pesho Peshkov is awesome!
Gosho Goshkov is awesome!
Penka Penkova is awesome!

# Кавичките имат значение

$ cat foo.sh
#!/bin/bash
for name in ${@}; do
    echo "${name} is awesome!"
done

$ ./foo.sh 'Pesho Peshkov' 'Gosho Goshkov' 'Penka Penkova'
Pesho is awesome!
Peshkov is awesome!
Gosho is awesome!
Goshkov is awesome!
Penka is awesome!
Penkova is awesome!

# Поведението в кавички на ${*} е различно от тована ${@}:

$ cat foo.sh
#!/bin/bash
for name in "${*}"; do
    echo "${name} is awesome!"
done

$ ./foo.sh 'Pesho Peshkov' 'Gosho Goshkov' 'Penka Penkova'
Pesho Peshkov Gosho Goshkov Penka Penkova is awesome!
```

```
# Този скрипт ще обходи полетата на реда от /etc/passwd, отнасящ се за потребителя ivan, последователно:

#!/bin/bash
old_IFS="${IFS}"
IFS=':'

for field in $(grep '^ivan:' /etc/passwd); do
    echo "field: ${field}"
done

IFS="${old_IFS}
```

### seq - Командата $ seq <начало> <край> изписва всички числа в дадения интервал:
```
$ seq 1 5
1
2
3
4
5
```

```
$ cat foo.sh
#!/bin/bash
for i in $(seq 1 5); do
    echo "i: ${i}"
done

$ ./foo.sh
i: 1
i: 2
i: 3
i: 4
i: 5
```

## While-loop

Конструкцията while в bash изглежда така:
```
while <команда>; do
    <блок>
done
```
Блокът се изпълнява множество пъти, докато командата завърши с неуспех. <br />

```
#!/bin/bash
while ps -e -o cmd= | grep -q firefox; do
    sleep 1
done
echo "firefox has died"
```

```
# Можем да се възползваме от факта, че read чете по един ред наведнъж, и да пренасочим изхода на някаква команда в целия блок на while:

#!/bin/bash
while read filename; do
    echo "found file: ${filename}"
done < <(find ~ -type f)
```

```
# Може да имаме име на файл, съдържащо символ за нов ред. За да се предпазим от тези ситуации, когато обхождаме файлове, можем да използваме тази конструкция:

#!/bin/bash
while read -d $'\0' filename; do
    echo "found file: ${filename}"
done < <(find ~ -type f -print0)

# Опцията -print0 на find го кара да изписва имената разделени с нулев символ вместо символ за нов ред
# Опцията -d на read му казва до какъв символ да спира да чете
# Конструкцията $'<низ>' интерпретира специални последователности в низа (в случая заменя \0 с нулев символ)
```

Командите $ break и $ continue могат да се използват в тялото на цикъл.

## Функции
Можем да обявяваме функции в bash със следната конструкция:
```
function <име на функция> {
    <тяло>
}
```
Тялото на функцията може да вижда параметрите, с които е извикана, чрез променливите ${@}, ${*}, ${#}, ${1}, ${2}, …
Можем да създаваме локални променливи в тялото нa функция така: local <име>=<стойност>
```
$ cat foo.sh
#!/bin/bash
function greet {
    local name="${1}"
    echo "Hello, ${name}"
}
greet "$(whoami)"

$ ./foo.sh
Hello, ivan
```

## Временни файлове
- Понякога, в рамките на скрипт, ни се налага да използваме временен файл
- Командата $ mktemp създава временен файл и изписва името му
    - $ mktemp -d създава директория
- Добре е да създаваме временните файлове само с $ mktemp, и да ги изтриваме след ползването им

## Допълнителни команди:
- cmp - проверява дали два файла са еднакви
- sha256sum, md5sum - алгоритми за пресмятане на хеш
- diff - сравнява текстови файлове ред по ред (изкарва редовете, на които има разлика)
    - `-q` - ще изведе 1 ред само ,ако файловете са различни 
    - `-s` - ще изведе 1 ред, на който ще спомене, че файловете са еднакви
- printf - форматиран изход
- pwgen – генерира пароли
- shift - shell built-in 
- source example.sh - Подобно на '#include <library>` в C++, тоест, заменя кода/скрипта.


