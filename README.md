# Инструкция по сборке программы

1. Клонировать репозиторий 

```
https://github.com/aleh4man/YADRO_task.git
```

2. Перейти в папку YADRO_task

3. Создать папку build и перейти в неё

```
mkdir build
cd build
```

4. Собрать программу одним из следующих способов:

- напрямую через компилятор:
```
gcc ../src/main.cpp ../src/cc_exceptions.cpp ../src/computer_club.cpp -lstdc++
```

или

```
g++ ../src/main.cpp ../src/cc_exceptions.cpp ../src/computer_club.cpp
```

На выходе получится файл a.out (в Linux) или a.exe (в Windows)

- через утилиту CMake (версия не меньше 3.24.2):

```
cmake ..
cmake --build .
```

На выходе получится файл club (в Linux) или club.exe (в Windows)



После сборки программу можно запустить с помощью команды

./<имя исполняемого файла> <имя текстового файла>

На выходе получится файл output.txt
