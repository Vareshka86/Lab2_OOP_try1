/**
 * @file main.cpp
 * @brief Точка входа программы лабораторной работы №2.
 * @details Лабораторная работа выполняется поэтапно, и в каждой версии
 * программы добавляется новая часть:
 * - v0.1 — часть 1: проектирование класса Book, конструкторы и методы чтения;
 * - v0.2 — часть 2: методы изменения состояния, деструктор, счётчик объектов;
 * - v1.0 — часть 3: функция main() — тест класса (этап №3).
 *
 * В версии v0.2 программа показывает методы изменения (выдача, возврат,
 * списание) и их отказы, а также момент окончания жизни объектов и работу
 * статического счётчика книг.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.2
 */

#include "Author.h"
#include "Book.h"

#include <iostream>
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
 * @brief Выводит строку кода перед её выполнением.
 * @details Вызов печатается до выполнения, чтобы сообщение метода об отказе
 * оказалось под ним, а не над ним.
 * @param call Вызов, как он записан в программе.
 */
void showCall(const std::string& call)
{
    std::cout << call << '\n';
}

/**
 * @brief Выводит результат метода изменения.
 * @param result Значение, которое вернул метод.
 */
void showResult(bool result)
{
    std::cout << "  -> " << (result ? "выполнено" : "отклонено") << '\n';
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

/**
 * @brief Демонстрация части 2: методы изменения, деструктор и счётчик.
 * @details Все книги — локальные объекты этой функции, поэтому к её концу они
 * уничтожаются, и сообщения деструкторов видны до паузы в конце программы.
 */
void runDemo()
{
    printHeader("Создание книг");
    Book war("Война и мир", Author("Лев Толстой", 1828), 1869);
    Book master("Мастер и Маргарита", Author("Михаил Булгаков", 1891), 1967);

    printHeader("Методы изменения состояния");
    showCall("war.issueTo(\"Иванов И. И.\")");
    showResult(war.issueTo("Иванов И. И."));
    war.print();

    showCall("war.issueTo(\"Петров П. П.\")   // книга уже выдана");
    showResult(war.issueTo("Петров П. П."));
    showCall("war.writeOff()   // книга у читателя");
    showResult(war.writeOff());
    showCall("war.returnBook()");
    showResult(war.returnBook());
    showCall("war.writeOff()");
    showResult(war.writeOff());
    war.print();

    showCall("war.issueTo(\"Петров П. П.\")   // книга списана");
    showResult(war.issueTo("Петров П. П."));
    showCall("master.returnBook()   // книга не выдана");
    showResult(master.returnBook());
    showCall("master.issueTo(\"\")   // имя читателя пустое");
    showResult(master.issueTo(""));
    master.print();

    printHeader("Время жизни объектов и счётчик");
    std::cout << "Book::getCount() = " << Book::getCount() << '\n';
    std::cout << "{   // начало блока\n";
    {
        Book copy(master);
        Book empty;
        std::cout << "    Внутри блока Book::getCount() = " << Book::getCount() << '\n';
        std::cout << "}   // конец блока: объекты copy и empty уничтожаются\n";
    }
    std::cout << "После блока Book::getCount() = " << Book::getCount() << '\n';

    std::cout << "\nКонец функции runDemo(): уничтожаются war и master (в обратном порядке создания)\n";
}

} // namespace

/**
 * @brief Главная функция программы.
 * @details Запускает демонстрацию части 2 и после неё показывает, что все книги
 * уничтожены: счётчик равен 0.
 * @return 0 — при нормальном завершении.
 */
int main()
{
    setupConsole();
    std::cout << "Лабораторная работа №2 по ООП: класс Book (книга из библиотеки), v0.2\n";

    runDemo();
    std::cout << "\nПосле runDemo() Book::getCount() = " << Book::getCount() << '\n';

    waitForEnter();
    return 0;
}
