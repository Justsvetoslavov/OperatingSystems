## Езикът C и системни извиквания

Връзката между един процес и (ядрото на) операционната система се извършва чрез операции, наречени системни извиквания (system calls или syscalls). <br />

**Системно програмиранe** - когато използваме програми, които директно използват системните извиквания на операционната система. <br />

От гледна точка на програмиста, системните извиквания са “просто” библиотечни функции, които може да извикват. <br />

Специалното на тези функции е, че вместо да изпълняват код като част от програмата, казват на ядрото да изпълни съответната операция и чакат резултат. <br />

Програмата заспива докато ядрото не стане готово с изпълнението на системното извикване, и когато се събуди, получава резултат. <br />

### Защо С?
- Езикът, на който е написан Linux (и други UNIX-и) е C;
- По тази причина, системните извиквания използват подреждане и формат на данните, съобразени с ABI1(Application Binary Interface) на C;
- Затова повечето библиотеки, имплементиращи syscall wrappers, са написани на C
    - Нищо не ни пречи да използваме системни извиквания директно и от друг език - пример за език, който има собствена имплементация на syscall wrappers е Go;
    - На практика почти всички останали езици извикват C код през FFI, който от своя страна вика syscall wrappers от библиотека като glibc, вместо да ги имплементират сами;

Пример за системни извиквания на C:
```c
#include <unistd.h> // getuid, geteuid & various POSIX functions
#include <sys/types.h> // uid_t
#include <stdio.h> // printf

int main(void) {
    uid_t me = getuid(); // Real user ID(ruid) - user who started the process
    uid_t pretending = geteuid(); // Effective user ID(euid) - user used for permissions check
    printf("uid: %d euid: %d\n", me, pretending);

    return 0; // exit status 0
}
```

### Exit status
```c
// Exit status-ът на една програма е стойността, върната от функцията main():

int main(void) {
    return 42; // exit status 42
}
```

Системното извикване _exit(2) прекратява изпълнението на процеса, независимо от текущата функция:
```c
#include <unistd.h> // _exit

void foo(void) {
    _exit(42); // exit status 42
}

int main(void) {
    foo();
}
```

Библиотечната функция exit(3) вътрешно вика _exit(2), но финализира и някои други неща преди това. <br />
Използвайте нея, когато искате да прекратите програмата. <br />
```c
#include <stdlib.h> // exit

void foo(void) {
    exit(42); // exit status 42
}

int main(void) {
    foo();
}
```

##  Syscall wrapper
Тъй като тези функции “обвиват” системните извиквания, ще ги наричаме **syscall wrappers**. <br />

Най-често използваната C библиотека, имплементираща **syscall wrappers**, е **glibc**;
- glibc е “стандартна библиотека” за C, която освен syscall wrappers имплементира и всички функции от C стандарта;
- Има и други такива библиотеки, напр. musl;

Съответните библиотеки съдържат хедъри със стандартизирани имена (fcntl.h, unistd.h, …), в които са дефинирани syscall wrapper-ите. <br />

### Какво правят syscall wrappers?
- Копира аргументите на системното извикване и неговия номер в специфични процесорни регистри;
- Изпълнява процесорна инструкция, която предизвиква хардуерно прекъсване;
- При стартиране на операционната система, тя е конфигурирала процесора при такова прекъсване да скочи на специфично място в кода на ядрото, където ще се прочетат въпросните аргументи и номер на системно извикване, и то ще се обработи;
- След това ядрото записва резултата от системното извикване в специфичен процесорен регистър и скача обратно в кода на syscall wrapper-а;
- Вади резултата от въпросния процесорен регистър и го връща;

## Обработване на грешки при системни извиквания

### Резултат от системни извиквания
По конвенция, повечето системни извиквания връщат резултат от числов тип, който е отрицателно число, ако извикването е било неуспешно. <br />
За пример ще използваме open(2) - системно извикване, отварящо файл. <br />
```c
int open(const char *pathname, int flags)
```
- първият аргумент е път до файл;
- вторият аргумент е множество от опции, задаващи режима на отваряне на файл;
- резултатът при успех е положително число - номер на файлов дескриптор (след малко ще говорим за него)
- засега важното е, че резултатът от open() е -1, ако отварянето на файл е било неуспешно.

Грешки при системни извиквания:
```c
#include <fcntl.h> // open
                   // Provides constants and functions for file control
#include <stdio.h> // printf
#include <stdlib.h> // exit

int main() {
    int result = open("/tmp/some_file", O_RDONLY);

    if (result < 0) {
        printf("opening /tmp/some_file failed\n");
        exit(1);
    }

    printf("opened /tmp/some_file successfully\n");
}
```

Когато някое системно извикване е неуспешно, ни се иска начин да разберем какво не е било наред. <br />
Затова, при неуспех системните извиквания записват число (код на грешка) в глобалната променлива **errno**. <br />

Можем да използваме errno, за да разберем каква е била грешката:
```c
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <errno.h> // errno
#include <stdlib.h> // exit

int main() {
    int result = open("/tmp/some_file", O_RDONLY);
    if (result < 0) {
        switch (errno) {
            case 2: printf("no such file\n"); break;
            case 13: printf("permission denied\n"); break;
            // ...
        }
        exit(1);
    }
    printf("opened /tmp/some_file successfully\n");
}
```

По-добре е да ползваме константите, дефинирани в errno.h, вместо чисти номера на грешки:
```c
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <errno.h> // errno
#include <stdlib.h> // exit

int main() {
    int result = open("/tmp/some_file", O_RDONLY);

    if (result < 0) {
        switch (errno) {
            case ENOENT: printf("no such file\n"); break;
            case EACCES: printf("permission denied\n"); break;
            // ...
        }
        exit(1);
    }
    printf("opened /tmp/some_file successfully\n");
}
```

Най-добрият вариант е да използвате функцията err(), която изписва форматирано съобщение за грешка (вътрешно гледа променливата errno). <br />
Първият аргумент на err() е exit status, с който да прекрати програмата. <br />
```c
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <err.h> // err

int main() {
    int result = open("/tmp/some_file", O_RDONLY);

    if (result < 0) {
        err(1, "could not open file");
    }

    printf("opened /tmp/some_file successfully\n");
}

// Например при грешка ENOENT, тази програма ще изведе съобщение, изглеждащо така:
// could not open file: No such file or directory
```

```c
// Вторият и следващите аргументи на err() задават формат на низ (както при printf()):

#include <fcntl.h> // open
#include <stdio.h> // printf
#include <err.h> // err

int main() {
    const char filename[] = "/tmp/some_file";
    int result = open(filename, O_RDONLY);
    if (result < 0) {
        err(1, "could not open file %s", filename);
    }
    printf("opened %s successfully\n", filename);
}

// Например при грешка ENOENT, тази програма ще изведе съобщение, изглеждащо така:
// could not open file /tmp/some_file: No such file or directory
```

Всъщност, err.h задава 4 полезни функции:
- void err(int eval, const char* fmt, ...)
    - изписва съобщението, което сме подали;
    - изписва грешката от errno;
    - прекратява програмата с подадения статус;
- void errx(int eval, const char* fmt, ...)
    - изписва съобщението, което сме подали;
    - не изписва грешката от errno;
    - прекратява програмата с подадения статус;
- void warn(const char* fmt, ...)
    - изписва съобщението, което сме подали;
    - изписва грешката от errno;
    - не прекратява програмата;
- void warnx(const char* fmt, ...)
    - изписва съобщението, което сме подали;
    - не изписва грешката от errno;
    - не прекратява програмата;

## Файлови дескриптори
Споменахме, че системното извикване open() връща число, което наричаме **номер на файлов дескриптор**. <br />
При отваряне на файл, ядрото създава системна структура, наречена файлов дескриптор, която съдържа:
- Указател към самия файл;
- Текуща позиция (индекс на байт) във файла;
- И други;

За всеки процес ядрото алокира масив от файлови дескриптори: номерът на файлов дескриптор, върнат от open(), е индекс в този масив. <br />

open() може да приема 2 или 3 аргумента:
```c
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
```
Аргументите на open() са:
- Път до файл;
- Множество от опции;
- Права за достъп (mode) - в осмична бройна система;

Вторият аргумент на open() е битова маска от опции.
- Комбинираме опциите с “побитово ИЛИ”.
Някои опции, които ще ни трябват, са:
- O_WRONLY - отваряне за писане;
- O_RDONLY - отваряне за четене;
- O_RDWR - отваряне за четене и писане едновременно;
- O_CREAT - ако файлът не съществува, го създава преди да го отвори;
    - Ако използваме O_CREAT в комбинация с O_EXCL при функция open() - подсигурява, че файла, който се опитваме да създадем не съществува. Ако той все пак съществува - fail-ва с error EEXIST;
- O_TRUNC - ако файлът съществува, зачиства съдържанието му преди да го отвори;
- O_APPEND - ако файлът съществува, началната позиция е в края му вместо в началото;

Третият аргумент на open() задава права за достъп на файла, ако го създаваме сега.
- Има смисъл само с O_CREAT

Единият вариант е да зададем правата директно като число
```c
int fd = open(
    "/tmp/some_file",
    O_WRONLY | O_CREAT | O_TRUNC,
    0644 // u=rw | g=r | o=r
);
```

Другият вариант е да ползваме побитови константи, дефинирани в стандартната библиотека
```c
int fd = open(
    "/tmp/some_file",
    O_WRONLY | O_CREAT | O_TRUNC,
    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH // same as 0644 - u=rw | g=r | o=r
);
```

Отваряне на файл за четене:
```c
int fd = open("/tmp/some_file", O_RDONLY);
```

Отваряне на файл за писане, създавайки го с права 644 ако не съществува и зачиствайки го, ако съществува:
```c
int fd = open(
    "/tmp/some_file",
    O_WRONLY|O_CREAT|O_TRUNC,
    0644
);
```

Отваряне на файл за писане, създавайки го с права 644 ако не съществува и заставайки накрая му, ако съществува:
```c
int fd = open(
    "/tmp/some_file",
    O_WRONLY|O_CREAT|O_APPEND,
    0644
);
```

Когато един процес умре, всички негови файлови дескриптори се затварят автоматично. <br />
Можем ръчно да затворим файлов дескриптор, използвайки системното извикване close(2). <br />
```c
#include <fcntl.h> // open, close
#include <err.h> // err

int main() {
    const char filename[] = "/tmp/some_file";
    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        err(1, "could not open file %s", filename);
    }

    // ...

    if (close(fd) < 0) {
        err(1, "could not close file %s", filename);
    }
}
```

Добре е отворените файлове да се затварят веднага, щом сме приключили да работим с тях.

Можем да четем от отворен файл със системното извикване read(2):
```c
char buf[20];

int num_bytes = read(fd, buf, 20);

if (num_bytes < 0) {
    err(1, "could not read data");
}
```
Аргументите на read() са:
- Номер на файлов дескриптор, от който четем;
- Указател към памет, в която искаме да се запишат прочетените данни;
- Максимална дължина на четене;

Резултатът от read() е броят реално прочетени байтове.
- При успешно изпълнение на read(), текущата позиция на файловия дескриптор се придвижва напред с броя успешно прочетени байтове;
- Това означава, че всяко следващо викане на read() чете нови данни от файла;
- Ако read() прочете 0 байта, това означава, че сме стигнали края на файла;

Пример:
```c
/* Code overview:
 * open a file,
 * read its content in chunks of up to 20 bytes,
 * process the data within the loop
 * close the file
 * handlе errors that may occur during these operations.
 */ 
int fd = open("/tmp/some_file", O_RDONLY);
if (fd < 0) {
    err(1, "could not open file");
}

char buf[20];
int num_bytes = read(fd, buf, 20);
while (num_bytes > 0) {
    // < do something with the data in buf >
    num_bytes = read(fd, buf, 20);
}

if (num_bytes < 0) {
    err(1, "could not read from file");
}

if (close(fd) < 0) {
    err(1, "could not close file");
}
```

Пример:
```c
/* Code overview:
 * open a file,
 * read its content in chunks of up to 4096 bytes,
 * process the data within the loop
 * closе the file
 * handlе errors that may occur during these operations.
 */ 
int fd = open("/tmp/some_file", O_RDONLY);
if (fd < 0) {
    err(1, "could not open file");
}

char buf[4096];
int num_bytes;
while ((num_bytes = read(fd, buf, sizeof(buf))) > 0) {
    // < do something with the data in buf >
}

if (num_bytes < 0) {
   err(1, "could not read from file");
}

if (close(fd) < 0) {
    err(1, "could not close file");
}
```

Пример:
```c
/* Code overview:
 * open a file,
 * read its content one character at a time
 * process each character within the loop
 * close the file
 * handlе errors that may occur during these operations.
 */ 
int fd = open("/tmp/some_file", O_RDONLY);
if (fd < 0) {
    err(1, "could not open file");
}

char c;
int num_bytes;
while ((num_bytes = read(fd, &c, 1)) > 0) {
    // < do something with the character in c >
}

if (num_bytes < 0) {
    err(1, "could not read from file");
}

if (close(fd) < 0) {
    err(1, "could not close file");
}
```

Писането във файл е аналогично на четенето: чрез системното извикване write(2):
```c
char buf[] = "Hello world!\n";

int num_bytes = write(fd, buf, 13);

if (num_bytes < 0) {
    err(1, "could not write data");
}

if (num_bytes != 13) {
    errx(1, "could not write data all at once");
}
```
Аргументите на write() са:
- Номер на файлов дескриптор, в който пишем;
- Указател към памет, от която да се прочетат данни;
- Брой байтове (размер на данните), които искаме да запишем;
Резултатът от write() е броят реално записани байтове. <br />

### Записване на текстови низове от паметта в текстов файл:
При записване на текстови низове, трябва да внимаваме да не запишем **терминиращия нулев символ(\0)** във файла. <br />
Функцията strlen(3) е полезна: връща размера на низа, без да включва терминиращата нула.
```c
char buf[] = "Hello world!\n";

int num_bytes = write(fd, buf, strlen(buf));

if (num_bytes < 0) {
    err(1, "could not write data");
}
if (num_bytes != 13) {
    errx(1, "could not write data all at once");
}
```

## Файлови дескриптори на стандартни потоци
Стандартните потоци stdin, stdout и stderr по подразбиране съществуват при създаване на процес:
- Файлов дескриптор **0** е **stdin**;
- Файлов дескриптор **1** е **stdout**;
- Файлов дескриптор **2** е **stderr**;

```c
char name_buf[512];
const char prompt[] = "What's your name? ";
const char hello[] = "Hello, ";
const char end[] = "!\n";

int write_result = write(1, prompt, strlen(prompt));
if (write_result < 0) { 
    err(1, "could not write prompt"); 
}

int name_len = read(0, name_buf, sizeof(name_buf) - 1);
if (name_len < 0) { 
    err(1, "could not read name");
}
// Ensure null-termination
name_buf[name_len] = '\0';

write_result = write(1, hello, strlen(hello));
if (write_result < 0) {
    err(1, "could not write hello");
}

write_result = write(1, name_buf, strlen(name_buf));
if (write_result < 0) {
    err(1, "could not write name");
}

write_result = write(1, end, strlen(end));
if (write_result < 0) {
    err(1, "could not write end");
}
```

## Преместване на текущата позиция във файл
Досега видяхме, че read() и write() местят текущата позиция напред. <br />
Със системното извикване lseek(2) можем да преместим текущата позиция на произволно място във файла: <br />
```c
off_t lseek(int fd, off_t offset, int whence);
```
Аргументите на lseek() са:
- Файлов дескриптор;
- Отместване;
- Интерпретация на отместването;

Резултатът от lseek() е новата абсолютна позиция. <br />

Възможните интерпретации на отместването (whence) са:
- SEEK_SET - абсолютно отместване;
- SEEK_CUR - относително отместване спрямо текущата позиция;
- SEEK_END - относително отместване спрямо края на файла;

Скачане в началото на файла:
```c
int new_pos = lseek(fd, 0, SEEK_SET);
if (new_pos < 0) {
    err(1, "could not go to start of file");
}
```

Скачане на позиция 42 във файла:
```c
int new_pos = lseek(fd, 42, SEEK_SET);
if (new_pos < 0) {
    err(1, "could not jump to byte 42");
}
```

Скачане с 5 байта назад:
```c
int new_pos = lseek(fd, -5, SEEK_CUR);
if (new_pos < 0) {
    err(1, "could not jump 5 bytes backwards");
}
```

Скачане 2 байта преди края на файл:
```c
int new_pos = lseek(fd, -2, SEEK_END);
if (new_pos < 0) {
    err(1, "could not jump to 2 bytes before end");
}
```

Скачане 42 байта след края на файл:
```c
// Това работи само ако можем да пишем във файла.
// Файлът пораства с толкова, колкото е нужно.
int new_pos = lseek(fd, 42, SEEK_END);
if (new_pos < 0) {
    err(1, "could not jump to 42 bytes after end");
}
```

## Четене и писане на двоични данни от паметта във файлове
Мислено можем да разделим подходите за вход/изход на две категории:
- Форматиран вход/изход
- Неформатиран вход/изход

### Неформатиран вход/изход
Когато говорим за неформатиран вход/изход, имаме предвид, че програмата чете и пише данни във формат, който не може да се интерпретира като текст.<br />

Числата най-често ги представяме по същия начин, както са представени в паметта.<br />

Системните извиквания read() и write() могат да се използват за неформатиран вход/изход на данни в паметта, без промяна на тяхната структура. <br />

```c
void write_number(int fd, uint16_t num) {
    write(fd, &num, sizeof(num));
}

uint16_t read_number(int fd) {
    uint16_t num;
    read(fd, &num, sizeof(num));
    return num;
}
```

### Форматиран вход/изход
Когато говорим за форматиран вход/изход, имаме предвид, че програмата чете и пише текст, предназначен за четене от хора. <br />
Нищо не пречи текстът да е и машинно четим. <br />
Числата са форматирани като последователности от цифри (текст). <br />

```c
int n_digits(uint16_t num) {
    if (num == 0) { 
        return 1;
    }

    int result = 0;

    for (; num != 0; num /= 10) { 
        result++;
    }

    return result;
}

void num_to_text(uint16_t num, char* buf) {
    buf[n_digits(num)] = '\0';

    for (int i = n_digits(num) - 1; i >= 0; i--) {
        buf[i] = '0' + (num % 10);
        num /= 10;
    }   
}

void print_number(int fd, uint16_t num) {
    char num_text[6];
    num_to_text(num, num_text);
    write(fd, num_text, n_digits(num));
}
```

Можем да използваме вградената функция snprintf(3) за да форматираме числа като текст:
```c
void print_number(int fd, uint16_t num) {
    char num_text[6];
    snprintf(num_text, sizeof(num_text), "%d", num);
    write(fd, num_text, n_digits(num));
}
```

### Работим само с файлови дескриптори
В стандартната библиотека на C има абстракция за работа с файлове, наречена FILE*, която обвива системните извиквания за работа с входно-изходни операции във функции от по-високо ниво.
- Повечето такива функции са в <stdio.h>.
- Ние няма да ползваме тези функции, а ще работим директно с файловите дескриптори.
- Единствените функции от <stdio.h>, които ще си позволим, са:
    - [printf()](https://www.geeksforgeeks.org/printf-in-c/) - за **форматиран изход към stdout**(generally console screen)
        ```c
        printf("formatted_string", arguments_list);
        
        int printf(const char *format, ...);
        ```
        - Parameters
            - formatted_string - a string that specifies the data to be printed. It may also contain a format specifier to print the value of any variable such as a character and an integer.
            - arguments_list - the variable names corresponding to the format specifier.
        - Return Value
            - returns the number of characters printed after successful execution.
            - If an error occurs, a negative value is returned. 
    - dprintf() - за форматиран изход към файлов дескриптор.
    ```c
    int dprintf(int fd, const char *format, ...);
    
    // Example
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dprintf(fd, "Hello, %s!\n", "world");
    close(fd);
    ```
    - snprintf() - за генериране на форматиран низ.

## Информация за файловете чрез stat
Системното извикване stat(2) дава достъп до метаданните на файла (командата stat(1) използва това системно извикване). <br />
Първият аргумент е път до файл, а вторият е указател към структура от тип struct stat, която е дефинирана в стандартната библиотека. <br />

```c
// man 3 stat
struct stat {
    dev_t st_dev; // ID of device containing file

    ino_t st_ino; // Inode number

    mode_t st_mode; // File type and mode
    
    nlink_t st_nlink; // Number of hard links

    uid_t st_uid; // User ID of owner
    gid_t st_gid; // Group ID of owner
    dev_t st_rdev; // Device ID (if special file)

    off_t st_size; // Total size, in bytes

    blksize_t st_blksize; // Block size for filesystem I/O
    blkcnt_t st_blocks; // Number of 512 B blocks allocated

    struct timespec st_atim; // Time of last access
    struct timespec st_mtim; // Time of last modification
    struct timespec st_ctim; // Time of last status change
};
```

```c
// Example
struct stat info;

int result = stat("/tmp/foo.txt", &info);

if (result < 0) {
     err(1, "could not stat /tmp/foo.txt")
};

dprintf(1, "owner UID: %d\n", info.st_uid);
dprintf(1, "owner GID: %d\n", info.st_gid);
dprintf(1, "size: %d bytes\n", info.st_size);
```

Алтернативният вариант **fstat()** приема файлов дескриптор като първи аргумент вместо път. <br />
Може да го ползвате за вече отворени файлове. <br />

## truncate/ftruncate
- truncate
    - Prototype: `int truncate(const char *path, off_t length);`
    - Description: Adjusts the size of the file specified by the path path to length bytes.
    - Usage:
        - Shrinking
            - If length is less than the current file size, the file is truncated and data beyond length is lost.
        - Expanding
            - If length is greater than the current file size, the file is extended and the extended part reads as zero bytes.
    - Parameters:
        - path: Path to the file.
        - length: Desired new size of the file in bytes.
        - Return Value: Returns 0 on success, -1 on failure (sets errno).
```c
// Example of truncate
#include <unistd.h>
#include <stdio.h>

int main() {
    const char *filename = "example.txt";
    if (truncate(filename, 1024) == -1) {
        err(1, "Failed to truncate file %s", filename);
    }
    return 0;
}
```
- ftruncate
    - Prototype: int ftruncate(int fd, off_t length);
    - Description: Adjusts the size of the file specified by the file descriptor fd to length bytes.
    - Usage:
        - Shrinking
            - If length is less than the current file size, the file is truncated and data beyond length is lost.
        - Expanding
            - If length is greater than the current file size, the file is extended and the extended part reads as zero bytes.
    - Parameters:
        - fd: File descriptor of the open file.
        - length: Desired new size of the file in bytes.
        - Return Value: Returns 0 on success, -1 on failure (sets errno).
```c
// Example of ftruncate
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        err(1, "Could not open file");
    }

    if (ftruncate(fd, 1024) == -1) {
        close(fd);
        err(2, "Failed to truncated file");
    }

    close(fd);
    return 0;
}
```

Key Differences
- Identification:
- truncate works with a file specified by its path.
- ftruncate works with a file specified by its file descriptor.

File Descriptor:
- truncate does not require you to open the file first; it operates directly on the file path.
- ftruncate requires the file to be already opened, as it operates on the file descriptor.

### Unlink
- Prototype: `int unlink(const char *pathname);`
- Description
    - The unlink function deletes a name from the filesystem.
    - If that name was the last link to a file and no processes have the file open, the file is deleted and the space it was using is freed.
    - If the file is open in any process, the actual file data is not removed until all file descriptors are closed.
- Usage: Commonly used to delete files.
- Parameters:
    - pathname: Path to the file to be deleted.
    - Return Value: Returns 0 on success, -1 on failure (sets errno).

How unlink Works
- Deleting a Name
    - unlink removes a directory entry (name) from the filesystem.
    - It decreases the link count of the file.
- Removing the File
    - If the link count reaches zero and no process has the file open, the file is deleted from the filesystem, and its resources are freed.
- Open Files
    - If a file is open when unlink is called, the file remains accessible through the file descriptor until it is closed.
    - The space will only be freed after the last file descriptor is closed.

# TODO
1) find C function to create and unlike temporary file - mkstemp & unlink
    - Add examples for truncate/ftruncate/stat/fstat and mktemp&unlink
2) Побитови
3) Endiness