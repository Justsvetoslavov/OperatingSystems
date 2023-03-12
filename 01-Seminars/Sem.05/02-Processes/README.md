## Изпълняване на програми с exec
Фамилията от системни извиквания exec(3) се използва, за да изпълним външна програма в текущия процес.
- Различни варианти на извикване - execl(), execlp(), execvp(), execle(), execve() …
При успешно изпълнение на exec(), програмата на текущия процес се заменя с дадената.

```c
int main(void) {
    int result = execl(
        "/usr/bin/cat", // executable
        "cat", "/etc/issue", // arguments
        (char*)NULL // sentinel value, referring to end of argument list
    );
    
    if (result < 0) {
        err(1, "could not exec");
    }

    dprintf(1, "you will never read this\n"); // Unaccessible code.
}
```

### execlp - търсене в $PATH"
Вариантите execlp използват environment променливата PATH, за да търсят изпълнимия файл
```c
int main(void) {
    int result = execlp(
        "cat", // executable
        "cat", "/etc/issue", // arguments
        (char*)NULL // sentinel
    );

    if (result < 0) {
        err(1, "could not exec");
    }

    dprintf(1, "you will never read this\n");
}
```

### execv* - масив от аргументи
Вариантите execv* приемат масив от аргументи:
```c
int main(int argc, char* argv[]) {
    if (argc > 9) {
        errx(1, "cannot work with more than 8 arguments");
    }
    char* command_args[10];
    char cat[] = "cat";
    command_args[0] = &cat;

    for (int i = 1; i < argc; i++) {
        command_args[i] = argv[i];
    }

    command_args[argc] = NULL;

    execvp("cat", command_args);

    err(1, "could not exec cat");
}
```

## Създаване на процеси
В UNIX света създаването на процеси става чрез системното извикване fork(2). <br />

При извикване на fork(), текущият процес се клонира на родител и дете
- Семантично, цялата памет на процеса се копира
- Реално копието се извършва чрез copy-on-write (CoW)

Родителят и детето използват отделни региони във физическата памет и не могат да достъпват паметта по между си. <br />

Изпълнението на програмата в процеса-дете продължава от мястото, където е извикан fork(). <br />

Отделните процеси работят конкурентно и не се изчакват. <br />

Стойността, върната от fork(), е различна при родителя и детето:
- В детето, fork() връща 0
- В родителя, fork() връща число, по-голямо от 0: pid-а на дететo

Пример:
```c
pid_t pid = fork();
if (pid < 0) {
    err(1, "could not fork");
}

if (pid > 0) {
    dprintf(1, "I am your father\n");
} else {
    dprintf(1, "Nooooooooo!\n");
}
```

Пример:
```c
pid_t pid = fork();
if (pid < 0) {
    err(1, "could not fork");
}

if (pid > 0) {
    dprintf(1, "I am the parent\n");
    dprintf(1, "The child's pid is %d\n", pid);
} else {
    dprintf(1, "I am the child\n");
}
dprintf(1, "I am both\n");
```

Както видяхме, PID-ът на процеса-дете се връща от fork()
getpid(2) и getppid(2) връщат PID-а на текущия процес и на неговия родител:
```c 
pid_t my_pid = getpid();
pid_t parent_pid = getppid();

dprintf(
    1,
    "My pid is %d and my parent's pid is %d\n",
    my_pid, parent_pid
);
```

### Изчакване с wait()
Системното извикване wait(2) блокира, докато някое дете на текущия процес умре. <br />
Аргументът му е указател, сочещ към променлива, в която wait() ще запише статуса на завършилото дете. <br />
Стойността, върната от wait() е PID-а на детето. <br />

Всъщност, статусът, който wait() записва в аргумента си, кодира малко повече информация освен exit status-а на процеса-дете. <br />

Например, можем да разберем дали процесът е бил убит или е завършил нормално
- Макрото WIFEXITED(status) проверява дали статусът е такъв на нормално-завършил процес.

Можем и да извлечем истинският exit status на процеса
- Макрото WEXITSTATUS(status) извлича exit status-а

```c
for (int i = 0; i < num_tasks; i++) {
    pid_t child_pid = fork();

    if (child_pid < 0) { 
        err(1, "could not fork");
    }

    if (child_pid == 0) {
        do_task(i);
        exit(0); // The child does its work and exits
    }
}

for (int i = 0; i < num_tasks; i++) {
    int status;
    pid_t child_pid = wait(&status);
    if (child_pid < 0) { 
        err(1, "could not wait for child");
    }
    if (!WIFEXITED(status)) {
        warnx(1, "a task failed: child was killed!");
    } else if (WEXITSTATUS(status) != 0) {
        warnx(1, "a task failed (exit status != 0)!");
    }
}

dprintf(1, "all tasks completed successfully\n");
```

Със системното извикване waitpid(2) можем да изчакаме завършването на процес със конкретен PID. <br />
Има и малко повече възможности от wait()
- Може да провери дали процес е завършил, без да блокира.
- Може да чака за цяла група процеси.

### Наследяване на средата при fork()
Процесът-дете наследява цялата среда на родителя си
- Потребител (EUID, UID)
- Права
- Environment променливи
- Отворени файлови дескриптори

Наследяване на файлови дескриптори:
```c
int fd = open(
        "/tmp/test.txt",
        O_WRONLY|O_CREAT|O_TRUNC,
        0666
        );

if (fd < 0) { 
    err(1, "could not open file");

}
pid_t pid = fork();
if (pid < 0) {
    err(1, "could not fork");
}

for (int i = 0; i < 1000; i++) {
    if (pid == 0) {
        write(fd, "foo\n", 4);
    } else {
        write(fd, "bar\n", 4);
    }
}

close(fd);
```
В предния пример получихме файл, в който имаме 1000 реда “foo” и 1000 реда “bar”, в произволен ред. <br />
Двата процеса имат достъп до един и същ файлов дескриптор. <br />
Процесите се състезават, кой от тях да запише своя текст и да премести указателя на файловия дескриптор напред. <br />
В следващата тема ще се възползваме от наследяване на файлови дескриптори, за да правим по-интересни неща. <br />

# Todo
- Add diagrams
- More examples
    - Add examples for waitpid
