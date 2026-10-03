/**
 * @file Book.cpp
 * @brief Реализация класса Book.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.2
 */

#include "Book.h"
#include "current_year.h"

#include <iostream>
#include <stdexcept>

// Определение статического поля: оно одно на всю программу, общее для всех книг.
// В C++14 статическое поле объявляется в классе, а определяется здесь, в .cpp.
int Book::count_ = 0;

Book::Book()
    : title_("Без названия"),
      author_(),
      year_(currentYear()),
      isIssued_(false),
      readerName_(),
      issueCount_(0),
      isWrittenOff_(false)
{
    ++count_;
    std::cout << "  [+] Создана книга «" << title_ << "» (конструктор по умолчанию). Книг сейчас: "
              << count_ << '\n';
}

Book::Book(const std::string& title, const Author& author, int year)
    : title_(title),
      author_(author),
      year_(year),
      isIssued_(false),
      readerName_(),
      issueCount_(0),
      isWrittenOff_(false)
{
    // Инвариант 1: название не пустое
    if (title_.empty())
    {
        throw std::invalid_argument("название книги не может быть пустым");
    }

    // Инвариант 2: год издания от MIN_YEAR до текущего года и позже рождения автора
    if (year_ < MIN_YEAR || year_ > currentYear())
    {
        throw std::invalid_argument("год издания " + std::to_string(year_) +
                                    " вне диапазона " + std::to_string(MIN_YEAR) + "-" +
                                    std::to_string(currentYear()));
    }
    if (author_.hasBirthYear() && year_ <= author_.getBirthYear())
    {
        throw std::invalid_argument("год издания " + std::to_string(year_) +
                                    " не позже года рождения автора " +
                                    std::to_string(author_.getBirthYear()));
    }

    // Счётчик увеличивается только после всех проверок: если конструктор бросил
    // исключение, объект не создан и считать его нельзя (деструктор для него
    // тоже не вызывается)
    ++count_;
    std::cout << "  [+] Создана книга «" << title_ << "» (конструктор с параметрами). Книг сейчас: "
              << count_ << '\n';
}

Book::Book(const Book& other)
    : title_(other.title_),
      author_(other.author_),
      year_(other.year_),
      isIssued_(other.isIssued_),
      readerName_(other.readerName_),
      issueCount_(other.issueCount_),
      isWrittenOff_(other.isWrittenOff_)
{
    // Копия корректной книги тоже корректна - проверять нечего. Но это новый
    // объект, поэтому счётчик увеличивается
    ++count_;
    std::cout << "  [+] Создана книга «" << title_ << "» (конструктор копирования). Книг сейчас: "
              << count_ << '\n';
}

Book::~Book()
{
    --count_;
    std::cout << "  [-] Уничтожена книга «" << title_ << "» (деструктор). Книг сейчас: "
              << count_ << '\n';
}

int Book::getCount()
{
    return count_;
}

const std::string& Book::getTitle() const
{
    return title_;
}

const Author& Book::getAuthor() const
{
    return author_;
}

int Book::getYear() const
{
    return year_;
}

bool Book::isIssued() const
{
    return isIssued_;
}

const std::string& Book::getReaderName() const
{
    return readerName_;
}

int Book::getIssueCount() const
{
    return issueCount_;
}

bool Book::isWrittenOff() const
{
    return isWrittenOff_;
}

void Book::print() const
{
    std::cout << "Книга «" << title_ << "»\n"
              << "  Автор:        " << author_.toString() << '\n'
              << "  Год издания:  " << year_ << '\n'
              << "  Положение:    ";
    if (isWrittenOff_)
    {
        std::cout << "списана";
    }
    else if (isIssued_)
    {
        std::cout << "выдана читателю: " << readerName_;
    }
    else
    {
        std::cout << "на полке";
    }
    std::cout << '\n'
              << "  Число выдач:  " << issueCount_ << '\n';
}

bool Book::issueTo(const std::string& reader)
{
    // Сначала все проверки: при отказе ни одно поле не должно измениться
    if (isWrittenOff_)
    {
        std::cout << "  Отказ: книга «" << title_ << "» списана, её нельзя выдать.\n";
        return false;
    }
    if (isIssued_)
    {
        std::cout << "  Отказ: книга «" << title_ << "» уже у читателя (" << readerName_ << ").\n";
        return false;
    }
    if (reader.empty())
    {
        std::cout << "  Отказ: не указано имя читателя.\n";
        return false;
    }

    isIssued_ = true;
    readerName_ = reader;
    ++issueCount_;
    return true;
}

bool Book::returnBook()
{
    if (!isIssued_)
    {
        std::cout << "  Отказ: книга «" << title_ << "» не выдана — возвращать нечего.\n";
        return false;
    }

    isIssued_ = false;
    readerName_.clear(); // книга на полке ни на кого не записана (инвариант 3)
    return true;
}

bool Book::writeOff()
{
    if (isWrittenOff_)
    {
        std::cout << "  Отказ: книга «" << title_ << "» уже списана.\n";
        return false;
    }
    if (isIssued_)
    {
        std::cout << "  Отказ: книга «" << title_ << "» у читателя (" << readerName_
                  << "), сначала её нужно вернуть.\n";
        return false;
    }

    isWrittenOff_ = true;
    return true;
}
