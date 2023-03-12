## Тръби и водопроводчици

### Pipe
Системното извикване pipe(2) създава тръба
- Тръбата е структура в ядрото, имплементираща FIFO опашка

Взаимодействаме с тръбата през два файлови дескриптора:
- pipe() приема като аргумент масив от 2 елемента, в който да запише номерата на двата файлови дескриптора.
- Дескриптор за четене (индекс 0).
- Дескриптор за писане (индекс 1).

```c
int pfd[2];
if (pipe(pfd) < 0) {
    err(1, "could not create pipe");
}

pid_t pid = fork();
if (pid < 0) {
    err(1, "could not fork");
}

if (pid == 0) {
    close(pfd[0]);
    write(pfd[1], "foo\n", 4);
    close(pfd[1]);
    exit(0);
} else {
    close(pfd[1]);
    char buf[20];
    read(pfd[0], buf, 20);
    // ... do something with data
}
```

Тръбите са удобен метод за комуникация между процеси. <br />

При четене от тръбата, текущият процес блокира докато някой друг не запише данни в тръбата. <br />

Когато всички краища за писане се затворят, краищата за четене получават “край на файл” (EOF) и четенето от тях вече
не блокира. <br />

Нужно е всеки процес да затваря краищата на тръбата, които не ползва
- В противен случай може да се получи deadlock.
- Например, може процесът, който пише данни, да е приключил, но процесът, който чете данни, да чака блокиран до безкрай, защото не е затворил своя край за писане.

### Копиране на файлови дескриптори

Можем да копираме файлови дескриптори със системните извиквания dup() и dup2().
- int dup(int oldfd)
    - копира подадения файлов дескриптор с номер oldfd на първия свободен номер, и връща новия номер
- int dup2(int oldfd, int newfd)
    - копира подадения файлов дескриптор oldfd и го свързва с новия файлов дескриптор newfd
    - Ако файловият дескриптор newfd вече съществува, dup2() ще го затвори преди да създаде новото копие.
Тези системни извиквания са много полезни, ако искаме да имплементираме пренасочване на стандартните потоци.

### Пренасочване
Пренасочване на изхода на команда(stdout) към файл.
```c
// Open the file in write-only mode, creating it if necessary
int fd = open("/tmp/test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
if (fd < 0) {
    err(1, "could not open file");
}

// Duplicate fd to stdout (file descriptor 1)
int result = dup2(fd, 1); // Now stdout goes to fd(a.k.a to the file "/tmp/test.txt")
if (result < 0) {
    err(1, "could not dup");
}

// Close the original file descriptor as it's no longer needed
close(fd);

// Execute the "ps -e" command and redirect its output to the file
execlp("ps", "ps", "-e", (char *)NULL);

// If execlp fails
err(1, "could not exec");
```

Пренасочване на текст към stdin на команда (pipe + dup)
```c
int pfd[2];
if (pipe(pfd) < 0) {
    err(1, "could not create pipe");
}

pid_t pid = fork();
if (pid < 0) {
    err(1, "could not fork");
}

if (pid == 0) { // Child process
    close(pfd[0]); // Close unused read end
    if (write(pfd[1], "foo\n", 4) < 0) { // Write to the pipe
        err(1, "could not write to pipe");
    }
    close(pfd[1]); // Close write end after writing
    exit(0);
} else { // Parent process
    close(pfd[1]); // Close unused write end
    dup2(pfd[0], 0); // Redirect stdin to the read end of the pipe
    close(pfd[0]); // Close the original read end as it's now redirected
    execlp("wc", "wc", "-m", (char *)NULL); // Execute wc -m
    err(1, "could not exec wc");
}
```

Пренасочване на изхода на една команда към входа на друга:
```c
int pfd[2];
if (pipe(pfd) < 0) {
    err(1, "could not create pipe");
}

pid_t pid = fork();
if (pid < 0) {
    err(1, "could not fork");

}

if (pid == 0) {
    close(pfd[0]);
    dup2(pfd[1], 1); // replace stdout by pfd[1]
    execlp("ps", "ps", "-e", (char*)NULL);
    err(1, "could not exec ps");
} else {
    close(pfd[1]);
    dup2(pfd[0], 0); // replace stdin by pfd[0]
    execlp("grep", "grep", "firefox", (char*)NULL);
    err(1, "could not exec grep");
}
```

Всъщност, shell-ът имплементира операторите >, >>, < и | точно така: чрез pipe() и dup().