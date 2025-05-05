# Параметры запуска

Пример аргументов в `tasks.json`.

```
"args": [
    "-fdiagnostics-color=always",
    "-g",
    "${file}",
    "-o",
    "${workspaceFolder}\\executable\\${fileBasenameNoExtension}.exe",
    "-I", "${workspaceFolder}\\include",
    "-L", "${workspaceFolder}\\lib",
    "-I", "C:\\Users\\pentryyy\\AppData\\Local\\Programs\\Python\\Python311\\include",
    "-I", "C:\\Users\\pentryyy\\AppData\\Local\\Programs\\Python\\Python311\\Lib\\site-packages\\numpy\\_core\\include",
    "-L", "C:\\Users\\pentryyy\\AppData\\Local\\Programs\\Python\\Python311\\libs",
    "-lpython311",
    "-lfftw3-3"
],
```

Сборка `.lib` для 32 bit библиотеки.

```
lib /machine:x86 /def:libfftw3-3.def
```

Сборка `.lib` для 64 bit библиотеки.

```
lib /machine:x64 /def:libfftw3-3.def
```
