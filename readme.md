# Задачи проекта

## Базовые задачи
1. Клиент
    1. Возможность отправлять на сервер бинарник и файл с данными, указав при этом количество запусков
    2. Возможность получить результаты исполнения бинарника на данном файле
    3. Возможность посмотреть самое быстрое, самое медленное и среднее время исполнения бинарника
2. Сервер
    1. Основной процесс, контролирующий работу сервера, на котором взаимподействие с пользовательтелями (получение и отправка данных), а так же менеджемент потоков (о них далее)
    2. Несколько потоков, испольнящих бинарник пользователя на входных данных с замером времени

## Основные задачи
1. Клиент
    1. Добавление флага на таймаут: возможность снизить время ожидания, если код работает слишком долго и получить извещение об этом
    2. Возможность посмотреть полную статистику по всем исполнениям бинарника
    3. Возможность остановить процесс с сохранением результатов на некоторое время или без
2. Сервер
    1. Должен удалять старые данные пользователей через какое-то время
3. Админ
    1. По запросу получить статистику по всем пользователям за час/сутки
    2. Приостановить исполнение сервера с сохранением состояния исполнения во всех потоках
    3. Мгновенно остановить исполнение сервера
    4. Восстановить работу сервера (при остановке с сохранением восстановить данные)

## Продвинутые задачи
1. Клиент
    1. Возможность узнать возможности программы
    2. Должен на любой запрос получить адекватный ответ
    3. Возможность получить данные по настоящему исполнению своего бинарника/место в очереди ожидания
    4. Возможность восстановить исполнение сохранённого процесса
    5. Перед отправкой на сервер файлов проводится проверка, не посылались ли данные файлы ранее, чтобы не отправлять файлы по новой
2. Сервер
    1. Основной процесс должен заниматься рациональным менеджементом исполнения: стараться, чтобы старые клиенты не слишком долго ждали
    2. При отключении клиента ждать некоторое время его возвращения, иначе прекращать выполнение запроса

# Сборка

### Сборка примера на измерение времени

```
g++ user.cpp tests.cpp main.cpp -o tmp
```

### Сборка клиента

```
gcc ./shared/copy.c ./client/client.c -o client.exe
```

Первый аргумент отвечает за имеющийся флаг (пока есть только флаг 's' - send), следующие аргументы задают путь до бинарника и до файла с тестами соответственно.

### Сборка сервера

```
gcc ./shared/copy.c ./server/user.c ./server/parser.c ./server/server.c -o client.exe
```

Не забывайте передать аргументов имя "канала" связи.
