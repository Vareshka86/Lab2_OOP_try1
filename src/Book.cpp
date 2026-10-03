/**
 * @file Book.cpp
 * @brief Реализация класса Book.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#include "Book.h"
#include "current_year.h"

#include <iostream>
#include <stdexcept>

Book::Book()
    : title_("Без названия"),
      author_(),
      year_(currentYear()),
      isIssued_(false),
      readerName_(),
      issueCount_(0),
      isWrittenOff_(false)
{
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
    // Копия корректной книги тоже корректна - проверять нечего
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
