/**
 * @file main.cpp
 * @brief Тест класса Book — функция main() (этап №3 лабораторной работы).
 * @details Лабораторная работа выполнялась поэтапно:
 * - v0.1 — часть 1: проектирование класса Book, конструкторы и методы чтения;
 * - v0.2 — часть 2: методы изменения состояния, деструктор, счётчик объектов;
 * - v1.0 — часть 3: функция main() — тест класса (этап №3).
 *
 * Тест состоит из шести этапов, которые требует задание:
 * 1. создание объектов разными конструкторами;
 * 2. вывод начального состояния;
 * 3. корректные операции;
 * 4. некорректные операции (методы изменения и конструкторы);
 * 5. повторный вывод состояния и проверка инвариантов;
 * 6. проверка независимости объектов.
 *
 * Каждая проверка сравнивает ожидаемый результат с полученным и печатает
 * `[OK]` или `[ОШИБКА]`; в конце выводится итог. Подробности — на странице
 * @ref testing.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 1.0
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

/// Вспомогательные функции и счётчики проверок, видимые только внутри этого файла.
namespace
{

/// Сколько проверок выполнено.
int g_checksTotal = 0;

/// Сколько проверок прошло успешно.
int g_checksPassed = 0;

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
 * @brief Выводит заголовок этапа теста.
 * @param title Текст заголовка.
 */
void printStage(const std::string& title)
{
    std::cout << "\n==================== " << title << " ====================\n";
}

/**
 * @brief Выводит имя переменной и карточку книги.
 * @param name Имя переменной в программе.
 * @param book Книга.
 */
void show(const std::string& name, const Book& book)
{
    std::cout << name << ": ";
    book.print();
}

/**
 * @brief Засчитывает одну проверку и печатает её результат.
 * @param description Что проверялось.
 * @param passed      true — ожидание совпало с результатом.
 */
void report(const std::string& description, bool passed)
{
    ++g_checksTotal;
    if (passed)
    {
        ++g_checksPassed;
    }
    std::cout << "  " << (passed ? "[OK]     " : "[ОШИБКА] ") << description << '\n';
}

/**
 * @brief Проверяет результат метода изменения.
 * @details Строку вызова нужно напечатать до самого вызова, чтобы сообщение
 * метода об отказе оказалось под ней.
 * @param expected Ожидаемый результат: true — операция должна выполниться.
 * @param actual   Что вернул метод.
 */
void expectResult(bool expected, bool actual)
{
    const std::string got = actual ? "выполнено" : "отклонено";
    const std::string want = expected ? "выполнено" : "отклонено";
    report("результат: " + got + " (ожидалось: " + want + ")", expected == actual);
}

/**
 * @brief Пытается создать книгу с неверными данными и проверяет, что это не удалось.
 * @param description Какие данные передаются (для вывода).
 * @param title       Название.
 * @param author      Автор.
 * @param year        Год издания.
 */
void expectBookRejected(const std::string& description, const std::string& title,
                        const Author& author, int year)
{
    std::cout << "Book(" << description << ")\n";
    const int countBefore = Book::getCount();
    try
    {
        Book wrong(title, author, year);
        report("книга создана, хотя данные неверны", false);
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "  исключение: " << error.what() << '\n';
        report("книга не создана, счётчик не изменился", Book::getCount() == countBefore);
    }
}

/**
 * @brief Пытается создать автора с неверными данными и проверяет, что это не удалось.
 * @param description Какие данные передаются (для вывода).
 * @param name        Имя.
 * @param birthYear   Год рождения.
 */
void expectAuthorRejected(const std::string& description, const std::string& name, int birthYear)
{
    std::cout << "Author(" << description << ")\n";
    try
    {
        Author wrong(name, birthYear);
        report("автор создан, хотя данные неверны", false);
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "  исключение: " << error.what() << '\n';
        report("автор не создан", true);
    }
}

/**
 * @brief Проверяет инварианты книги через её публичный интерфейс.
 * @param name Имя переменной (для вывода).
 * @param book Проверяемая книга.
 */
void expectInvariants(const std::string& name, const Book& book)
{
    const bool titleOk = !book.getTitle().empty();                              // инвариант 1
    const bool yearOk = book.getYear() >= Book::MIN_YEAR &&
                        (!book.getAuthor().hasBirthYear() ||
                         book.getYear() > book.getAuthor().getBirthYear());     // инвариант 2
    const bool readerOk = book.isIssued() == !book.getReaderName().empty();     // инвариант 3
    const bool writeOffOk = !(book.isWrittenOff() && book.isIssued());          // инвариант 4
    const bool countOk = book.getIssueCount() >= 0;                             // инвариант 5
    report(name + ": все пять инвариантов соблюдены",
           titleOk && yearOk && readerOk && writeOffOk && countOk);
}

/**
 * @brief Снимок состояния книги одной строкой — чтобы сравнить «до» и «после».
 * @param book Книга.
 * @return Значения всех полей, доступных через методы чтения.
 */
std::string stateOf(const Book& book)
{
    return book.getTitle() + " | " + book.getAuthor().toString() + " | " +
           std::to_string(book.getYear()) + " | " + (book.isIssued() ? "выдана" : "не выдана") +
           " | " + book.getReaderName() + " | " + std::to_string(book.getIssueCount()) + " | " +
           (book.isWrittenOff() ? "списана" : "не списана");
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
 * @brief Тест класса Book из шести этапов.
 * @details Все книги — локальные объекты этой функции: при выходе из неё они
 * уничтожаются, и сообщения деструкторов видны до паузы в конце программы.
 */
void runTests()
{
    printStage("Этап 1. Создание объектов");
    std::cout << "Book empty;   // конструктор по умолчанию\n";
    Book empty;
    std::cout << "Book war(\"Война и мир\", Author(\"Лев Толстой\", 1828), 1869);   // с параметрами\n";
    Book war("Война и мир", Author("Лев Толстой", 1828), 1869);
    std::cout << "Book warCopy(war);   // конструктор копирования\n";
    Book warCopy(war);
    std::cout << "Book master(\"Мастер и Маргарита\", Author(\"Михаил Булгаков\", 1891), 1967);\n";
    Book master("Мастер и Маргарита", Author("Михаил Булгаков", 1891), 1967);
    report("создано 4 книги тремя разными конструкторами, Book::getCount() = " +
               std::to_string(Book::getCount()),
           Book::getCount() == 4);

    printStage("Этап 2. Начальное состояние");
    show("empty", empty);
    show("war", war);
    show("warCopy", warCopy);
    show("master", master);
    report("копия совпадает с оригиналом", stateOf(warCopy) == stateOf(war));

    printStage("Этап 3. Корректные операции");
    std::cout << "war.issueTo(\"Иванов И. И.\")\n";
    expectResult(true, war.issueTo("Иванов И. И."));
    std::cout << "war.returnBook()\n";
    expectResult(true, war.returnBook());
    std::cout << "war.issueTo(\"Петров П. П.\")\n";
    expectResult(true, war.issueTo("Петров П. П."));
    report("war: у читателя Петров П. П., выдач 2",
           war.isIssued() && war.getReaderName() == "Петров П. П." && war.getIssueCount() == 2);
    std::cout << "empty.writeOff()\n";
    expectResult(true, empty.writeOff());
    report("empty: списана", empty.isWrittenOff());

    printStage("Этап 4. Некорректные операции");
    const std::string warBefore = stateOf(war);
    const std::string emptyBefore = stateOf(empty);
    const std::string masterBefore = stateOf(master);

    std::cout << "war.issueTo(\"Сидоров С. С.\")   // книга уже у читателя\n";
    expectResult(false, war.issueTo("Сидоров С. С."));
    std::cout << "war.writeOff()   // книга у читателя\n";
    expectResult(false, war.writeOff());
    std::cout << "empty.issueTo(\"Иванов И. И.\")   // книга списана\n";
    expectResult(false, empty.issueTo("Иванов И. И."));
    std::cout << "empty.writeOff()   // книга уже списана\n";
    expectResult(false, empty.writeOff());
    std::cout << "empty.returnBook()   // книга не выдана\n";
    expectResult(false, empty.returnBook());
    std::cout << "master.returnBook()   // книга не выдана\n";
    expectResult(false, master.returnBook());
    std::cout << "master.issueTo(\"\")   // имя читателя пустое\n";
    expectResult(false, master.issueTo(""));

    std::cout << "\nПопытки создать некорректные объекты:\n";
    expectBookRejected("\"\", Author(\"Лев Толстой\", 1828), 1869", "",
                       Author("Лев Толстой", 1828), 1869);
    expectBookRejected("\"Азбука\", Author(\"Иван Фёдоров\"), 1400", "Азбука",
                       Author("Иван Фёдоров"), 1400);
    expectBookRejected("\"Книга будущего\", Author(\"Автор\"), 3000", "Книга будущего",
                       Author("Автор"), 3000);
    expectBookRejected("\"Мастер и Маргарита\", Author(\"Михаил Булгаков\", 1891), 1850",
                       "Мастер и Маргарита", Author("Михаил Булгаков", 1891), 1850);
    expectAuthorRejected("\"\", 1900", "", 1900);
    expectAuthorRejected("\"Автор\", -5", "Автор", -5);

    printStage("Этап 5. Повторный вывод состояния");
    show("war", war);
    show("empty", empty);
    show("master", master);
    report("war не изменилась после отклонённых операций", stateOf(war) == warBefore);
    report("empty не изменилась после отклонённых операций", stateOf(empty) == emptyBefore);
    report("master не изменилась после отклонённых операций", stateOf(master) == masterBefore);
    expectInvariants("empty", empty);
    expectInvariants("war", war);
    expectInvariants("warCopy", warCopy);
    expectInvariants("master", master);

    printStage("Этап 6. Независимость объектов");
    const std::string copyBefore = stateOf(warCopy);
    const std::string masterBefore2 = stateOf(master);
    const std::string emptyBefore2 = stateOf(empty);
    std::cout << "Меняем только war: war.returnBook(); war.writeOff();\n";
    expectResult(true, war.returnBook());
    expectResult(true, war.writeOff());
    show("war", war);
    show("warCopy", warCopy);
    report("warCopy (копия war) не изменилась", stateOf(warCopy) == copyBefore);
    report("master не изменилась", stateOf(master) == masterBefore2);
    report("empty не изменилась", stateOf(empty) == emptyBefore2);

    std::cout << "\nМеняем только warCopy: warCopy.issueTo(\"Сидоров С. С.\")\n";
    const std::string warBefore2 = stateOf(war);
    expectResult(true, warCopy.issueTo("Сидоров С. С."));
    report("war не изменилась", stateOf(war) == warBefore2);

    printStage("Итог");
    std::cout << "Проверок пройдено: " << g_checksPassed << " из " << g_checksTotal << '\n';
    std::cout << "Книг сейчас: " << Book::getCount() << '\n';
    std::cout << "\nКонец функции runTests(): книги уничтожаются в порядке, обратном созданию\n";
}

} // namespace

/**
 * @brief Главная функция программы: запуск теста класса Book.
 * @details После теста проверяет, что все книги уничтожены (счётчик равен 0).
 * @return 0 — все проверки пройдены, 1 — есть ошибки.
 */
int main()
{
    setupConsole();
    std::cout << "Лабораторная работа №2 по ООП: тест класса Book (книга из библиотеки), v1.0\n";

    runTests();

    std::cout << "\nПосле runTests() Book::getCount() = " << Book::getCount() << '\n';
    report("все книги уничтожены, счётчик равен 0", Book::getCount() == 0);
    std::cout << "\nВсего проверок пройдено: " << g_checksPassed << " из " << g_checksTotal << '\n';

    waitForEnter();
    return g_checksPassed == g_checksTotal ? 0 : 1;
}
