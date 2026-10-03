/**
 * @file main.cpp
 * @brief Точка входа программы лабораторной работы №2.
 * @details Лабораторная работа выполняется поэтапно, и в каждой версии
 * программы добавляется новая часть:
 * - v0.1 — часть 1: проектирование класса Book, конструкторы и методы чтения;
 * - v0.2 — часть 2: методы изменения состояния, деструктор, счётчик объектов;
 * - v1.0 — часть 3: функция main() — тест класса (этап №3).
 *
 * В версии v0.1 программа создаёт книги тремя способами и выводит их карточки,
 * а также показывает, что конструктор не даёт создать книгу с неверными данными.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#include "Author.h"
#include "Book.h"

#include <iostream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // подключать только основную часть WinAPI
#include <windows.h>        // SetConsoleOutputCP, SetConsoleCP
#endif

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/**
 * @brief Настраивает консоль Windows на кодировку UTF-8.
 * @details Исходные файлы сохранены в UTF-8. Без этой настройки
 * русский текст в консоли Windows выводится «кракозябрами».
 * В Linux/macOS консоль уже работает в UTF-8, поэтому там функция
 * ничего не делает (код внутри `#ifdef _WIN32` не компилируется).
 */
void setupConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

/**
 * @brief Выводит заголовок раздела.
 * @param title Текст заголовка.
 */
void printHeader(const std::string& title)
{
    std::cout << "\n=== " << title << " ===\n";
}

/**
 * @brief Ждёт нажатия Enter перед закрытием окна.
 * @details Если программу запустили двойным щелчком, без этой паузы окно
 * консоли закроется сразу после вывода.
 */
void waitForEnter()
{
    std::cout << "\nНажмите Enter, чтобы закрыть программу...";
    std::string line;
    std::getline(std::cin, line);
}

} // namespace

/**
 * @brief Главная функция программы.
 * @details Создаёт три книги разными конструкторами и выводит их, затем
 * пытается создать книгу с неверными данными и показывает сообщение об ошибке.
 * @return 0 — при нормальном завершении.
 */
int main()
{
    setupConsole();
    std::cout << "Лабораторная работа №2 по ООП: класс Book (книга из библиотеки), v0.1\n";

    printHeader("Три способа создать книгу");

    std::cout << "\n1) Book first;   // конструктор по умолчанию\n";
    Book first;
    first.print();

    std::cout << "\n2) Book second(\"Война и мир\", Author(\"Лев Толстой\", 1828), 1869);"
                 "   // параметризованный конструктор\n";
    Book second("Война и мир", Author("Лев Толстой", 1828), 1869);
    second.print();

    std::cout << "\n3) Book third(second);   // конструктор копирования\n";
    Book third(second);
    third.print();

    printHeader("Конструктор не создаёт некорректную книгу");

    std::cout << "\nBook wrong(\"Мастер и Маргарита\", Author(\"Михаил Булгаков\", 1891), 1850);\n";
    try
    {
        Book wrong("Мастер и Маргарита", Author("Михаил Булгаков", 1891), 1850);
        wrong.print(); // сюда программа не дойдёт: конструктор бросит исключение
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "Книга не создана: " << error.what() << '\n';
    }

    waitForEnter();
    return 0;
}
