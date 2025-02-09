# Yadro-test-task
Тестовое задание на участие в проекте “Система распределения расчетных задач на базе Linux”.

## Инструкция
### Сборка
```shell
gcc shm_writer.c -o writer && shm_reader.c -o reader
```
### Запуск
```shell
./writer

./reader <key>
```
### Запуск в Docker контейнере
```shell
docker build -t shared-memory-task .

docker run shared-memory-task
```
Скрипт `run.sh` последовательно выполняет программы, передавая ключ из вывода `writer` в аргумент `reader` , в консоль печатается вывод обоих программ.
