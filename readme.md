# block_engine

Кроссплатформенный движок для вычислений и любой управляющей логики.

## Основные особенности

- схема вычислений задается с помощью блоков и шин данных
- система событий позволяет легко встраивать движок для вычислений в любые приложения
- модуль работы с вводом/выводом позволяет эффективно работать с дескрипторами ОС и кастомными драйверами устройств,
учитывая их особенности и ограничения (не реализовано)
- плагинная система позволяет создавать новые типы блоков и гибко управлять логикой вычислений (не реализовано)

## Структура проекта

Проект организован с использованием принципов чистой архитектуры. Основные сущности проекта описываются в виде модельных классов, используемых в других модулях проекта, будь то графический редактор схем или движок для рассчетов.

### common

Модуль, хранящий модельные классы основных сущностей проекта: схем, блоков, шин. Здесь также содержится описание допустимых входов и выходов для каждого типа блоков, реализованное с помощью декларативного фреймворка, основанного на шаблонах.

### core

Модуль, реализующий движки для расчета схемы. Здесь на основе модельного блока создается логика блока с входами и выходами, соответствующими декларативному описанию блока. 

### gui

Модуль, реализующий графический редактор схем. Здесь на основе модельного блока создаются графические представления блока с входами и выходами, соответствующими декларативному описанию блока. 
