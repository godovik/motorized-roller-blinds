## Сборка контроллера

### Необходимые компоненты
- контроллер `ESP-12` - 1шт
- датчик холла `A3144` - 2шт
- smd-резистор `0805 10K` - 6шт 
- smd-конденсатор` 0805 1UF` - 1шт 
- линейный стабилизатор `AMS1117` - 1шт
- `ULN2003 SOP-16` - 1шт

### Особенности
Компоненты необходимо распаять на плате согласно схеме `PCB_motorized-roller-blinds`.
Датчик холла `h1` должен быть поднят максимально далеко от поверхности платы.
Датчик холла `h2` должен быть опущен как можно ниже - длина ножек над повернхостью платы не должна превышать 5мм.
Для прошивки, к соответсвующим пинам рекомендую припаять однорядные пины типа "папа" на 90 градусов. 

## Cборка прошивки

1. Для самостоятельной сборки прошивки вам потребуется установить несколько библиотек в Arduino IDE:
- [EEPROM](https://github.com/PaulStoffregen/EEPROM)
- [GyverHub](https://github.com/GyverLibs/GyverHub)
- [GyverStepper](https://github.com/GyverLibs/GyverStepper)

2. Библиотеки из папки `libraries` необходимо скопировать в `~/Arduino/libraries`. 
Можно использовать симлинк: 
```
ln -s motorized-roller-blinds/libraries/ ~/Arduino/libraries/motorized-roller-blinds-libraries
```

3. Открыть `motorized-roller-blinds.ino` в Arduino IDE
4. Выполнить `Скетч -> Экспорт бинарного файла`

## Прошивка контроллера

Для прошивки контроллера потребуется Usb-TTL конвертер.

**Важно!** Пин `rx` на контроллере надо соединять с пином `tx` на конвертере, а пин `tx` с `rx`.

Для перевода контроллера в режим прошивки, надо будет замкнуть пины `flash` и `reset` на `gnd`.
Чтобы упростить эту процедуру, рекомендую разместить кнопки на макетной плате:

[[https://github.com/godovik/motorized-roller-blinds/blob/master/docs/img/flash-schema.png|alt=flash-schema]]


## Сборка устройства


## Настройка шторы

### Подключение к WiFi
1. После подключения питания, контроллер попытается подключиться к сети WiFi. Если ему это не удается, он переключается в режим точки доступа. Для подключения к ней откройте настройки WiFi на устройстве, обновите список сетей и подключитесь к сети `Roller Blind`.

2. После успешного поключения перейдите в GyverHub и обновите список устройств. В списке доступных устройств появится новое:
[[https://github.com/godovik/motorized-roller-blinds/blob/master/docs/img/new-device.png|alt=new-device]]

3. Перейдите на страницу устройства. Откройте меню устройства и выберите `Настройки WiFi`
[[https://github.com/godovik/motorized-roller-blinds/blob/master/docs/img/main-page.png|alt=main-page]]
[[https://github.com/godovik/motorized-roller-blinds/blob/master/docs/img/main-menu.png|alt=main-menu]]

4. Введите данные для подключения к вашему WiFi и нажмите `Подключиться`
[[https://github.com/godovik/motorized-roller-blinds/blob/master/docs/img/wifi-settings.png|alt=wifi-settings]]

Контроллер запомнит настройки, перезагрузится и попытается подключиться к WiFi

### Калибровка шторы


## Подключение электропривода к умному дому Yandex

