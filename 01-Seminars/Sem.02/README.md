## Команди за работа с файлове

## Работа с файлове и директории

1. `cat [файл/ове]` 
    - показва съдържанието на файл/овете
    - без подадени файлове, cat очаква вход от потребителя и го изписва на екрана след enter.
    - **ВАЖНО**: ако сме написали cat само без подадени файлове и искаме да спрем командата използваме Ctrl+C - така изпращаме сигнал до ядрото за терминиране на процеса.
```
cat file1 file2
#показва съдържанието на file1 и след това на file2
```

2. `touch [име_на_файл/ове]` - създава празни файлове
```
touch file1 file2 
#създава празните файлове file1 и file2 в текущата директория

touch file_name
touch File1_name File2_name File3_name 
touch -a fileName //change access time only
touch -c fileName //commands is used to check whether a file is created.If not - don't create it
touch -t YYMMDDHHMM fileName //create a file using a specified time

//More on man page
```

3. `tee [име_на_файл/ове]` - създава файлове и ни позволява да пишем в тях едновременно (т.е. съдържанието им накрая е едно и също)
```
tee file1 file2
# създава файловете file1 и file2 в текущата директория и очаква от нас вход. Като приключим с въвеждането приключваме процеса с Ctrl+C.
```

4. `cp [файл/ове] [дестинация]` - копира файлове
```
cp f1 f2 f3 ~/Desktop
# копира файловете f1 f2 f3 в директорията Desktop, която се намира в нашата начална потребителска директория
```

```
cp f1 f2 
# копираме съдържанието на f1 във f2
```

```
cp d1 d2 # Error: -r not specified; omitting directory d3
# Ако искате да копирате/премествате цялата директория използвате опция `-r` (recursive)
cp -r d1 d2
```

```
# За да копирате нещо от сървър до компютъра ви ползвате scp (secure copy):
scp <user_name>@<server_ip>:<path_to_file> /some/local/directory
```

5. `mv [файл/ове] [дестинация]` - премества/преименува файлове
```
mv ./f1 ~/Desktop 
# премества файла f1 от текущата директория в Desktop, която се намира в нашата начална потребителска директория
	
mv f1 f2
# преименува файла f1 на f2
```

6. `mkdir [име_на_директория/и]` - създава празни директории 
```
mkdir d1 d2
# създава празните директории d1 и d2 в текущата директория
```

```
mkdir -p dir1/dir2/dir3
# създава целия указан път, т.е. създава dir1, в нея създава dir2 и в dir2 създава dir3
```

7. `rmdir [име_на_директория/и]` - трие **само** празни директории
```
rmdir d1 # където d1 е празна
```

8. `rm [име_на_файл/ове]` - трие файлове
```
rm -i f1 f2 f3
# пуска ни запитване дали искаме да изтрием файловете

rm -r f1 d1
# рекурсивно триене, чрез него можем да изтрием непразна директория
# **ВАЖНО**: Използвайте `-r` с внимание, особено, когато е в комбинация с `-f` (force command)
```

9. Symlinks/Hardlinks

A soft link or symbolic link will point to the original file on your system.  <br />
A hard link will create a copy of the file. <br />

`ln -s <path to the file/folder to be linked> <the path of the link to be created>`

```
ln -s /etc/passwd ~/passwd_symlink
```

```
# Unlink
unlink <path-to-symlink>

# Remove
rm <path-to-symlink>
```

```
# Find broken symlinks

find /home/james -xtype l

# Find & delete broken symlinks
find /home/james -xtype l -delete
```

## Timestaps
**The Access time of a file** will get updated every time the file is accessed. <br />
**The Modification time** of a file will get updated every time the file content is modified. <br />
**The Change time** of a file will get updated every time the file’s metadata is changed
- such as permissions or ownership (triggering the inode number to change);

### Commands 
- time
    - command in Linux is used to execute a command and prints a summary of real-time, user CPU time and system CPU time spent by executing a command when it terminates;
    - ‘real‘ time is the time elapsed wall clock time taken by a command to get executed;
    - ‘user‘ and ‘sys‘ time are the number of CPU seconds that command uses in user and kernel mode respectively;
- atime
    - access time, последния път, когато някой е достъпил този файл;
- mtime
    - modify time, последния път, когато някой е променил съдържанието;
- ctime
    - change time, последния път, когато някой е променил съдържанието или някаква метаинформация;

| Option | Description |
| :---:  |    :---:    |
| -atime n | The last access time of a file was n*24 hours ago. |
| -ctime n | The last time a file has been changed was n*24 hours ago. |
| -mtime n | The last time a file have been modified was n*24 hours ago. |
| -amin n |	The last access time of a file was n minutes ago. |
| -cmin n |	The last time a file has been changed was n minutes ago. |
| -mmin n |	The last time a file has been modified was n minutes ago. |
| -daystart | Starting the time measurement from the start of today rather than 24 hours ago. |

```
find ~ -type f -mmin -60

#or

find ~ -type f -ctime -1
```

## Търсене на файлове

`find` - командата без аргументи извежда на екрана абсолютно всички файлове (дори и тези в поддиректориите) на текущата директория, в която се намирате

Синтаксис: `find [директория] [аргументи]` 
		
Аргументи:
- `-name` - търси по име на файл	
- `-type` - търси по тип на файл	
- `-user` - търси по притежател на файла	
- `-group` - търси по групата, притежател на файла
- `-size` - търси по размер на файла (трябва да специфицирате мерната единица) 
- `-exec` - изпълнява команда върху всеки един намерен резултат;
- `-printf` - позволява да форматирате изхода. В този случай той ще отпечата подробности за всеки файл, който намери:
    - `%m` - това ще отпечата битовете за разрешение на файла като осмично число (например 755 или 644);
    - `%f` - това отпечатва името на файла без водещи директории;
    - `%p` - това отпечатва името на файла (с водещи директории);
    - `%s` - file size in bytes;
    - `%A@` - това ще отпечата последния час на достъп до файла за секунди от началото на 1970 г. (епохата на Unix). Изходът ще бъде под формата на число с плаваща запетая;
    - `%T@` - това ще отпечата времето за последна модификация на файла за секунди от началото на 1970 г. (епохата на Unix). Изходът ще бъде число с плаваща запетая;
- `-perm`
    - `find -perm mode`
        - This searches for files that have permissions exactly matching the specified mode.
        - For example, -perm 644 finds files with permissions set exactly to 644.
    - `find -perm -mode`
        - This looks for files that have all the permissions specified in mode set.
        - It's a bit more flexible because the file permissions can be greater than or include additional permissions beyond what is specified.
        - For example, -perm -644 will match any file that has at least the permissions 644, so it could match files with permissions like 744, 755, etc.
    - `find -perm /mode`
        - This option searches for files that have any of the permissions specified in mode.
        - It's used to match files that have any part of the specified mode.
        - For example, -perm /644 might find files that are:
            - readable by the owner
            - or writable by the group
            - or anything that matches any of the permissions bits set in 644.
```
# МОЛЯ НЕ ГО ПРАВЕТЕ АКО ИЗРИЧНО НЕ ТРЯБВА ДА ТЪРСИТЕ В ЦЯЛАТА ФАЙЛОВА СИСТЕМА**

find / -name *.txt
# Тук ще намерим всеки файл с разширение txt в цялата файлова директория.
```

```
find /home/directory -name *.txt -exec -wc -c {} \;
# Тук ще намерим размера на всеки файл с разширение txt в цялата файлова директория
```
**Защо ни трябва `{} \;`**?
- `find` ще изпълни нашата команда и ще замести `{}` с намерените резултати;
- `\;` гарантира, че командата ще се изпълни точно веднъж върху всеки един от резултатите;

## Работа със съдържанието на файловете
1. `file [файл/ове]` - дава информация за типа на файла
2. `wc [файл/ове]` - брои:
	- `-l` - редове;
	- `-c` - байтове;
	- `-w` - думи;
	- Default: Ако не зададем опция командата изписва: бр. редове, бр. символи, бр. думи и името на файла;
    - `-m` - брой симовли;
3. `head [-опции] [файл/ове]` - командата без аргументи по подразбиране дава първите 10 реда от указаните файлове
	- `-n` - така командата ни дава първите num реда от файловете;
	- `-c` - дава ни първите num байта от файла;
4.	`tail [-опции] [файл/ове]` - без аргументи ни дава последните 10 реда от указаните файлове
	- `-n` - така командата ни дава последните num реда от файловете;
	- `-c` - дава ни последните num байта от файла;
    - `-f` - показва ни крайното съдържание на файла, но командата не приключва, а чака и изписва всички нови промени накрая на файла;
5. `more`
    - `more f1` - ако имаме голям файл го показва екран по екран;
    - `less f1` - подобно на more, но докато more показва файла самоотгоре-надолу, less ни позволява да се движим и в двете посоки;
6. `sort f1` - сортира f1 по редове лексикографски
    - `-r` - reverse order;
    - `-f` - forces the sort to ignore the case of a letter when ordering lines;
    - `-s` - stabilises the sort by outputting identical lines in the same order as they appeared in the original file;
    - `-u` - removes duplicate lines;
    - `-k` - sort via a key; keydef gives location and type;
    - `-t` - field separator;
    - `-n` - numerical sort;
7. `uniq` - accepts input from a text-based file and removes any repeated lines, only if they are adjacent to each other. That’s why it’s used in conjunction with sort to remove non-adjacent lines;
    - `-i` - ignore case differences;
    - `-c` - counts the number of times a line occurs;
    - `-d` - only prints the duplicated lines;
8. `comm f1 f2` - сравнява два сортирани файла ред по ред.
- Изходът е 3 колони:
    - 1-ва колона е уникалните редове за f1;
    - 2-ра е уникалните редове за f2;
    - 3-та е редове, които са еднакви в двата файла;
9.  `diff f1 f2`  - сравнява два файла ред по ред;
- Дава ни кои редове и по какъв начин трябва да бъдат променени в единия файл, за да бъдат двата файла идентични;
- `-q` - silently check if the two files are different without displaying any output;
    - If the files are the same, diff -q will exit with status 0;
    - If the files are different, diff -q will exit with status 1;
10. `cmp f1 f2` - сравнява 2 файла байт по байт ;
	- ако файловете са еднакви то командата не връща нищо;
	- ако файловете се различават, командата ни връща първата позиция, на която се различават;
11. `cut [options] [files]` - реже части от всеки ред на файл
    - `-c` - извлича специфично указани символи;
    ```
    cut -c 1-3,8-10 f1
    # показва ни първите 3 символа от всеки ред на файла f1, прескача до 8-мия и показва от 8-мия до 10-тия;
    ```

    ```
    cut -c 1,5,7 f1 
    # показва ни 1-вия, 5-тия и 7-мия символ от всеки ред на файла f1
    ```
    **Важно:** На пръв поглед `-c` изглежда прави същото като `-b` (`-b` реже по байтове), но не е така, защото някои символи са повече от един байт. <br /> 
    Примерно "♣" e 3 байта. Ако имаме файл със съдържание "♣abc" `cut -c 1,4` ще ни даде "♣c", докато `cut -b 1,4` няма да успее да ни принтира "♣" . <br />
- `-f` - реже по номер на колона. Ако не е указан разделител реже по TAB;
- `-d` - указва по какъв разделител да се реже;
    **Note:** `-f` и `-d` често се ползват заедно
    ```
    cut -d ":" -f 1-3 f1
    # ще ни покаже първите 3 колони от файла f1
    ```
11. `tr [options] [SET1] [SET2]` - заменя или изтрива специфични символи в подаден низ, където `SET1` е множеството от символи, които трябва да се заменят, `SET2` е множеството, с което да заменим символите от `SET1` в низа
	-  tr без опции прави точно гореспоменатото
	- `-c`
        - сменя всички символи в низа, които **не са** в `SET1`, с тези от `SET 2`;
	- `-d`
        - изтрива всички срещания на символите от `SET1`;
	- `-s`
        - замества повтарящите се символи, изброени в `SET1`, само с един символ (премахва повторения);

12. sed - редактор на потоци за филтриране и трансформиране на текст.
    - `-i` - "in-place" editing.
        - Той променя файла директно, без да е необходимо да пренасочва изхода към друг файл или обратно към същия файл;
    - `-E`
        - enables extended regular expressions, which provide a more powerful pattern matching than basic regular expressions.
```
#Example:
's/[a-z]/\U&/g': This is the actual sed expression that describes the transformation to be applied.
- s/.../.../g: This is a substitution expression.
- The s means "substitute".
- The g at the end stands for "global", which means that the substitution should occur for all occurrences in each line, not just the first one.
- [a-z]: This is a character class that matches any lowercase letter from a to z.
- \U&: This is the replacement pattern.
- \U means "convert the following characters to uppercase".
- & is a special reference in sed that refers to the matched text (i.e., the lowercase letter matched by [a-z] in this context).
```

13. grep
`grep <str> <file>` - търси низ/рег израз във файл. Като резултат командата връща редовете от файла, в които се съдържа низа/рег. израз;
- `-v <str> <file>` - връща редовете, в които **не се** съдържа низа/рег. израз;
- `-r <str> <directory>` - търси файлове, съдържащи подадения низ, рекурсивно в подадената директория;
- `-n ` - кара grep да отпечатва и номера на редовете;
```
# grep -n "pattern" filename
egrep -n "error" logfile.txt # Outputs the lines and text of lines containing the error string
```
- `-B (before)` - отпечатвa зададен брой редове преди тези, които съдържат търсения низ;
- `-A (after)` - отпечатвa зададен брой редове след тези, които съдържат търсения низ;
- `-C (context)` - карат grep да отпечатвa зададен брой редове преди/след тези, които съдържат търсения низ;
- `-w` - търси по цяла дума;
- `-q`-  will only return an exit status:
    - 0 if a line is selected (meaning there was a match);
    - 1 if no lines were selected;
    - 2 if an error occurred;
- `-o` - this option tells grep to output only the matched parts of the file line, not the entire line that contains a match.
- `-F`
    - this option tells grep to interpret the pattern as a fixed string, not a regular expression;
    - this is particularly useful when you are searching for exact strings rather than patterns to be interpreted by the regex engine;
- `-f`
    - this tells grep to take the patterns from a file;
    - each line in this file is treated as a separate pattern;

## Команди за многопотребителския режим
- `login`
- `su [username]` - сменяме потребителя или ставаме суперпотребител;
- `chown [потребител] [файл/ове]` - смяна на собственик на файла (UID)
    - only root can change ownership to another user;
    - newly created files will usually be given GID ownership based on the current active group of the person who creates the file;
    ```
    chown foo:bar
    # chown new_owner:new_group
    # chown new_owner
    # chown new_owner: filename // new_owner=new_group -> changes both with value of new_owner
    ```
- `chgrp [група] [файл/ове] ` - смяна на групата на файла (GID)
```
# Change ownership of a file
sudo chgrp geeksforgeeks abc.txt
sudo chgrp geeksforgeeks GFG
```
- `chmod [режим] [файл/ове] ` -  промяна на правата за достъп до даден файл или директория. <br />
Режимът може да е:
    - Зададен чрез число в 8-мична бройна система: 000-777.
        - r - read - 4
        - w - write - 2
        - x - execute - 1
        ```
        # Пример:
        chmod 777 f1
        ```

### Как можем да изчислим всяка цифра?
От първото упражнение знаем, че правата за достъп на файловете изглеждат така: 
```
drwxr-xr-x 2 user user 4096 окт 5 2018 directory
-rw-r--r-- 1 user user 2243935 дек 8 2017 file
```
Като на първата позиция стои съответно индикатор дали това е директория (d) или файл (-). <br />
Следват 3 групи от 3 символа **r**ead, **w**rite, e**x**ecute, които указват правата за достъп съответно на текущия потребител, групата му и всички останали. Ако някое право е забранено съответно на мястото му присъства `-`. <br />

За да получим 3-цифрения код за правата за достъп просто заместваме r,w,x с 1-ца, а `-` с 0. <br />

**Примерно** `-rw-r--r--` е файл, който може да се чете и пише от текущия потребител и само да се чете от групата и всички останали. <br />
След като заместим символите с 0 и 1 получаваме: `110 100 100`  и така след преобразуване в 8-мична бр. система получаваме `644`. <br />

- Зададен чрез букви: следва следния синтаксис: [Кой] [Действие] [Какво]
    - Кой: **u**ser, **g**roup, **o**thers, **a**ll;
    - Действие: 
        - `+` - добавя право;
        - `-` - премахва право;
        - `=` - сменя изцяло правата за достъп с тези, който стоят от дясната страна на равенството   ;
    - Какво: **r**ead, **w**rite, e**x**ecute;

**Примери:**
- `chmod u=rw,og=r file.txt` - текущия потребител получава правото само да чете и да пише, а групата и всички останали само да четат;
- `chmod a+wx script.sh` - всички получават право да пишат и изпълняват файла;
- `chmod go+w file` - групата и всички останали получават право да пишат във file;
- `chmod u+x,go-r file` - текущия потребител получава право за изпълнение на файла, а на потребителите в групата му и всички останали се отнема правото да четат този файл;

### Type of file
- `-` - regular file;
- `b` - block special file;
- `c` - character special file;
- `d` - directory;
- `l` - symbolic link;
- `p` - FIFO (named pipe);
- `s` - socket;

### Permision sets
```
user (owner)
group (group owner)
everyone else (other)
symbolic representation rwxr-xr-x
numeric representation 0755

r - 100b - 4 - Read
w - 010b - 2 - Write
x - 001b - 1 - Execute

r | w = 6
r | x = 5

754
7 - owner
5 - group
4 - others
```

### Special permissions
- set UID upon execution (SUID) 100b
- set GID upon execution (SGID) 010b

**SUID and SGID on files**
- an executable with the SUID bit set runs with the security context of the user who owns it, regardless of the executing user;
- an executable with the SGID bit set runs with the security context of the group who owns it, regardless of the executing user;

**SGID on directories**
```
file from root:root
/dir2
new permissions that of file -> my_user : root
```

**Sticky Bit on directories**
```
/dir1
777
pesho --> pesho:pesho
niki --> niki:niki

both pesho and niki
================
/dir2
777
^sticky bit 
pesho --> pesho:pesho
niki --> niki:niki

# only owner of file can delete file;
# Pesho cannot delete niki and niki cannot delete pesho;
```

### Special permissions
```
Numeric
7777
SUID - 4
SGID - 2 
Sticky bit - 1

Symbol
SUID -> execute owner
SGID -> execute group
Sticky bit -> execute other

- -> no permissions. Neither execute nor special.
x -> execute permissions, no special permission.
s or t -> Execute + special
S or t -> no execute + special

SUID = s/S
GUID = s/S
Sticky bit = t/T
```

### Changing File permissions

```
chmod 0664 foo.txt
chmod u-rw,g-rw,o=r foo.txt
chmod -R 644 dir1 //directory + all files - recursively
```

```
permissions = base - unmask
base dir = 777
base !dir = 666 # all that is not a directory, files for example
umask 022 //can be changed

def perm file = 644
def perm dir = 755

umask = 123
perm file = 644
# umask only removes if it has the specific permission
# thus execute is skipped
```

```
umask 435
owner - r
group - wx
other - rx
```

## Archives
`tar` - stands for "tape archive" and is used to create, manipulate, or extract archive files.
- `-c`
    - create a new archieve;
    - it's used when you want to pack a directory or file(s) into a .tar archive.
- `-f`
    - option is used to specify the name of the archive file we want to create;
- `-x` - extract;
- `-a` - "auto-compress."
    - It tells tar to use the appropriate compression program based on the archive's file extension. 
- `-v` - verbose.
    - When this option is used, tar will display the progress in the terminal, typically listing the files being archived or extracted.
- `-z` - to compress the archive using gzip (or decompress if extracting);
- `-C` - option is really handy when you want to make sure the extracted files go exactly where you intend them to;
```
tar -xf <tarball_name> -C <directory_path>
```