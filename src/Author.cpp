/**
 * @file Author.cpp
 * @brief Реализация класса Author.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#include "Author.h"
#include "current_year.h"

#include <stdexcept>

Author::Author()
    : name_("Автор не указан"), birthYear_(UNKNOWN_BIRTH_YEAR)
{
}

Author::Author(const std::string& name, int birthYear)
    : name_(name), birthYear_(birthYear)
{
    // Список инициализации уже записал значения в поля - теперь проверяем их.
    // Если данные неверны, исключение прерывает создание: объекта не будет.
    if (name_.empty())
    {
        throw std::invalid_argument("имя автора не может быть пустым");
    }
    if (birthYear_ != UNKNOWN_BIRTH_YEAR && (birthYear_ < 1 || birthYear_ > currentYear()))
    {
        throw std::invalid_argument("год рождения автора " + std::to_string(birthYear_) +
                                    " вне допустимого диапазона");
    }
}

const std::string& Author::getName() const
{
    return name_;
}

int Author::getBirthYear() const
{
    return birthYear_;
}

bool Author::hasBirthYear() const
{
    return birthYear_ != UNKNOWN_BIRTH_YEAR;
}

std::string Author::toString() const
{
    if (hasBirthYear())
    {
        return name_ + " (род. " + std::to_string(birthYear_) + ")";
    }
    return name_;
}
