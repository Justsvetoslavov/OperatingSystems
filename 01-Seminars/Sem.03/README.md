# Информационни команди. Команди за многопотребителския режим, комуникация. Команди за работа с процеси.
		
## Команди за работа с процеси

### Command ps

`ps` - показва всички процеси пуснати от терминала
- `ps aux`
	- комбинация от долупосочените опции. Показва информация за абсолютно всички процеси на всички потребители
	- `x`
		- показва всички процеси (включително и тези, които нямат контролиращ терминал), които са ваша собственост
	- `a`
		- показва процесите и на другите потребители заедно с тези на текущия
	- `u` - Display user-oriented format.
	```
	ps | head -n 1 # Outputs: PID TTY TIME CMD
    ps a | head -n 1 # PID TTY STAT TIME COMMAND
	ps u | head -n 1 # USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
- `-e`
	- this option stands for "everything." It selects all processes on the system.
- `-g`
	- This option filters the processes by group.
- `-o` - The -o (or "output") option specifies the format of the output.
	```
	-o pid=,args=,vsz=
	```
	- `pid`: Process ID.
		- `ps -o pid` - Първи ред ще е pid;
		- `ps -o pid=` - няма заглавие за pid;
		- `ps -o pid=Gosho` - преимненува pid column header на Gosho;
	- `args`: Command with arguments.
	- `comm`: Command name (comm) of the process, without any arguments.
	- `vsz`: Virtual memory size of the process in KiB (kilobytes).
	- `rss`: Physical memory.
	- `ppid`: Stands for "Parent Process ID." It represents the unique ID of the process that started the current process.
	- `-etimes` - elapsed time since the process was started, in seconds.
	- `--sort` - specify sorting order (e.g: --sort=etimes).
- `-u [username]` - показва всички процеси на подадения потребител

**Каква информация виждаме?** 
- USER - на кой потребител принадлежи процеса;	
- PID - идентификационен номер на процеса;
- %CPU - използваното време на процесора, разделено на времето, в което е стартиран процесът;
- %MEM  - процент на реалната памет, използвана от процеса;
- VSZ   - използване на виртуална памет за целия процес (в KiB);
- RSS - незаменяемата физическа памет, която е използвана в задачата (в KiB);
- TTY  - контролиращ терминал;
- STAT - кодът на състоянието на процеса, който може да бъде Z (зомби), S (спящ), R (работещ) и т.н.
- START  - начално време на процеса
- TIME - количество време на процесора, изразходвано за изпълнение на процеса
- COMMAND - името на изпълнената команда с нейните опции 

### Command kill
`kill [опции] [pid на процес] ` - изпраща сигнал на процес
- Какво е сигнал?
	- Сигналите са предупреждения за важни събития, изпращани към процесите от ядрото или от друг процес.
	- Те прекъсват дейността на процеса и го принуждават да ги обработи веднага.  Всички сигнали могат да бъдат изведени чрез `kill -l`
- По-важни сигнали:
	- SIGHUP  с номер 1 -  изпраща се от ядрото при отписване на потребителя. 
	- SIGINT с номер 2  - прекъсване. Изпраща се от ядрото при въвеждане  **Ctrl+C**  на терминала.
	- SIGKILL с номер  9 - принуждава процеса да завърши без да му се дава възможност за нормална завършваща дейност.
	- SIGTERM с номер 15  - заявка за нормално приключване на процеса.
	- TSTP с номер 18 - временно прекратява действието на процеса. Изпраща се от ядрото при въвеждане  **Ctrl+Z**  на терминала.
- kill без посочен сигнал по default изпраща SIGTERM.
``` 
# Пример:
kill -9 1234 # убиваме (безмилостно) процес с pid 1234
```
We all love memes: [FUN 1](https://i.imgur.com/6u3dd.png); [FUN 2](https://i.redd.it/1meun32avtj11.jpg); [BE GOOD PEOPLE](https://linuxhandbook.com/content/images/2020/06/dont-sigkill.jpeg)

### pkill vs kill

Difference between pkill and kill:
Usage Context:
- kill: Used to send a signal to a specific process given its process ID.
- pkill: Used to send a signal to processes based on their names and other attributes (e.g., user name).

Flexibility:
- kill: Requires the process ID (PID) to send a signal.
- pkill: Can target multiple processes based on pattern matching, which makes it more flexible in certain situations.
Examples:

If you know a process has the PID of 1234 and you want to terminate it, you'd use:
```
kill 1234
```
If you want to terminate all processes named "myprocess", you'd use:
```
pkill myprocess
```
Both kill and pkill are just front-ends to the same underlying system call to send signals to processes. The primary distinction is in how they identify which processes to signal.

## Регулярни изрази
Както казахме, grep търси редове в текст, отговарящи на някакъв регулярен израз. <br />
Други команди, за които ще говорим, също могат да работят с регулярни изрази (sed, awk, perl …) <br />

### Да припомним математическата дефиниция на регулярен израз:
- Имаме азбука на езика и азбука на специалните символи;
- Всеки символ от езика е регулярен израз;
- ‘()‘ е регулярен израз (конкатенация);
- (<регулярен израз>|<регулярен израз>) е регулярен израз (“или”);
- (<регулярен израз>*) е регулярен израз (звезда на Клини);

### Basic regex
- По подразбиране grep използва такива изрази;
- Различен синтаксис за различните имплементации на grep (GNU grep, BSD grep);
- Доста малко възможности;

### Extended regex
- `grep -E` - използва extended regex;
- `egrep е` същото като grep -E;
Синтаксисът за регулярни изрази, който ще покажем сега, е именно Extended.
За разлики между различните варианти, вижте regex(7)
- Perl-compatible regex (PCRE);
- `grep -P` - използва PCRE;
- Най-пълният синтаксис;

Някои по-странни символи можем да ги “опишем” в регулярен израз, използвайки съответния escape code:
- `\t` - таб;
- `\n` - символ за нов ред / line feed (LF);
- `\r` - carriage return (CR);
- `\s` - whitespace character;
- `\x42`- конкретен символ, изразен чрез неговия код в шестнадесетична бройна система (hex);
- `.` - Точката match-ва един произволен символ;
  - `grep 'ba..'` match-ва низа “ba” последван от два произволни символа;

Някои специални конструкции са котви (anchors) и не match-ват конкретен символ, а вместо това фиксират позиция:
- `$` - фиксира край на ред;
- `^` - фиксира начало на ред;
- `\>` - фиксира край на дума;
- `\<` - фиксира начало на дума;
- `\b` - фиксира начало или край на дума;

Множество от символи, заградени в квадратни скоби, наричаме **символен клас (character class)**.
- Целият клас match-ва един произволен символ от множеството
	- [foabx] - match-ва един от символите `f`, `o`, `a`, `b` или `x`;
	- [a-zA-Z] - match-ва произволна латинска буква;
	- [^a-zA-Z] - match-ва произволен символ, който не е латинскабуква;
		- Важно: Символът `^` в началото на `символен клас` го **инвертира**;
		- Да **НЕ** се бърка с другото значение на `^` извън символен клас (начало на ред);
	- `\w` - вътре в клас е равносилно на `a-zA-Z0-9_`
	- `[:alnum:]` - Включва всички буквено-цифрови символи (т.е. всички букви и цифри). Например, "A", "b", "5" и "9".
	- `[:alpha:]` - Включва всички буквени символи, без цифрите. Например, "A", "z", "m", и "B"
	- `[:cntrl:]` - Включва всички контролни символи, които не се изобразяват визуално. Например, символите за начало на текст, край на текст, backspace и други.
	- `[:digit:]` - Включва само цифровите символи. Например, "0" до "9".
	- `[:punct:]`- Включва всички пунктуационни символи. Например, ",", ".", "!", ":", ";" и други.
	- `[:space:]` - Включва всички символи, които се считат за празно пространство, включително интервал, табулация, нов ред и други.
	- `[:lower:]` - Включва всички малки букви. Например, "a" до "z" (в английската азбука).
	- `[:upper:]` - Включва всички главни букви. Например, "A" до "Z" (в английската азбука).
```
grep -E '[[:alpha:]_.]' match-ва един символ, който е буква, долна черта или точка
# забележете двойните квадратни скоби
```

Съществуват количествени оператори (quantifiers), които модифицират броя срещания на атома преди тях:
- `<атом>*` - означава произволен брой (включително 0) срещания на дадения атом
- `<атом>+` - означава произволен брой (1 или повече) срещания на дадения атом
- `<атом>?` - означава 0 или 1 срещания на дадения атом
- `<атом>{3,5}` - означава 3, 4 или 5 срещания на дадения атом
- `<атом>{5,}` - означава 5 или повече срещания на дадения атом
- `<атом>{,42}` - означава най-много 42 срещания на дадения атом

```
# Примери:

# редове, започващи с някоя от буквите s, t или u, дълги общо 15 символа
$ grep -E '^[stu].{14}$' /usr/share/dict/words

# редове, започващи с гласна буква, дълги общо 13 символа и завършващи на “ion”
$ grep -E '^[aeiou].{9}ion$' /usr/share/dict/words

# редове, започващи с “c”, дълги над 16 символа
$ grep -E '^c.{15,}$' /usr/share/dict/words

# редове, започващи с “n”, завършващи с “c” и дълги от 8 до 12 символа
$ grep -E '^n.{6,10}c$' /usr/share/dict/words

# match-ва низовете “ba”, “baba”, “bababa” и т.н
$ grep -E '(ba)+'
```

Освен това, групираният текст може да се реферира на друго място в израза, използвайки конструкцията \<номер>
- номерата на групите се индексират спрямо позицията на лявата скоба в израза
- група 0 е целият match-нат текст;
- група 1 е текстът, match-нат от израза, ограден от най-лявата лява скоба;
- група 2 е текстът, match-нат от израза, ограден от втората най-лява лява скоба;

```
# match-ва гласна буква, последвана от произволен символ и след това същата гласна буква
$ grep '([aouei]).\1'
```

Всички специални символи могат да се escape-нат с `\`:
-  `\(` - match-ва лява скоба;
-  `\*` - match-ва звездичка;

## Xargs
Командата xargs чете записи от stdin и извиква някаква команда, подавайки ѝ ги като аргументи: <br />
```
xargs [OPTION]... COMMAND [INITIAL-ARGS]...
```
OPTION: There are several options you can use with xargs. Here are some of the more common ones:
- `-I {}`
	- Allows you to define a placeholder (like `{}`) in the `COMMAND`;
	- For each input item, xargs will replace `{}` with the input item and then execute the command;
- `-d DELIM`
	- Use `DELIM` as the delimiter between input items instead of whitespace;
	- This is often used with `-d '\n'` to ensure each line of input is treated as a single item;
- `--no-run-if-empty` or `-r`
	- If there are no input items, don't run the command;
	- By default, xargs runs the command at least once, even if there's no input.
- `-n MAX-ARGS`
	- Run the command with up to `MAX-ARGS` arguments at once.
- `-P MAX-PROCS`
	- Run up to `MAX-PROCS` processes at a time;
	- This is useful for parallelizing work;
- `-L MAX-LINES`
	- Use up to `MAX-LINES` lines of input for each invocation of the command.
- `-p`
	- Prompt the user before executing each command, allowing them to decide whether to run it.
- `-t`
	- Print the command before executing it;

```
# Delete all files listed in files_to_delete.txt:
cat files_to_delete.txt | xargs rm

# Replace all occurrences of {} with a line from names.txt and execute echo:
cat names.txt | xargs -I {} echo "Hello, {}!"

# Remove directories listed in dirs_to_remove.txt, prompting the user before each removal:
cat dirs_to_remove.txt | xargs -p rmdir
```

```
# създава файлове в текущата директория с имената напотребителите, логнати в момента
$ who | cut -d ' ' -f 1 | sort | uniq | xargs touch

# убива всички процеси на потребителя ivan със сигнал TERM
$ ps -u ivan -o pid= | xargs kill -TERM 

# ps -u ivan -o pid=: Lists all process IDs (pid) owned by user ivan.
# The = sign after -o pid removes the column header.
# xargs kill -TERM: Uses xargs to apply the kill -TERM command to each of the PIDs fetched by the ps command.
```

## Awk
Eзик за програмиране, специализиран за обработка на табличен текст <br />
Командата awk приема скрипт, написан на езика awk, и го изпълнява върху текста, получен на stdin <br />

```
# чете awk-скрипт от файл вместо от аргумент
awk -f <скрипт> 
```

awk скриптовете имат следната структура:
```
<тест> { <действие>; <действие> ... }
<тест> { <действие>; <действие> ... }

# За всеки ред от входа се изпълняват действията, чиито тестове са успешни
```

-F позволява да изберем разделител за колонки
	- по подразбиране, колонките се разделят по произволно количество whitespace
```
# Тази команда изписва съдържанието на петата колонка от редовете на /etc/passwd, чиято първа колонка е ivan.
$ cat /etc/passwd | awk -F ':' '$1 == "ivan" { print $5 }' Ivan Ivanov
```

- BEGIN { <действие> ... } - изпълнява се в началото на цялото изпълнение
- END { <действие> ... } - изпълнява се в края на цялото изпълнение

Всеки awk скрипт може да ползва някои вградени променливи:
- $0 - съдържанието на текущия ред;
- $1, $2, $3, … - съдържанието на първата, втората и т.н. колонка от текущия ред;
- NF - брой колонки в текущия ред;
- NR - брой редове, прочетени досега (т.е. номер на текущия ред);
- Освен това, можем да използваме потребителски променливи;
- С опцията ‘-v =’ можем да подадем глобална променлива от shell-а на awk скрипта;

```
# Да преброим всички редове в /etc/services, които съдържат името на даден сървис в първата колонка,
# подавайки името на сървиса (напр. “asp”) като променлива:
cat /etc/services awk -v service='asp' 'BEGIN { i = 0 } $1 == service { i += 1 } END { print i }'
```

Регулярни изрази в awk <br />
Операторът ~ /<регулярен израз>/ е тест за регулярен израз
```
# отпечатва редовете от изхода на $ who, които имат низ, състоящ се от s, последвано от произволен брой цифри, в първата колонка
$ who | awk '$1 ~ /^s[0-9]*$/'
```

Tестът може да липсва - тогава се подразбира true
```
# изписва третата колонка от всеки ред
‘awk ‘{ print $3 }’ 
```

Действието може да липсва - тогава се подразбира print $0

```
# изписва всички редове, чиято първа колонка е ivan
awk '$1 == "ivan"'
```