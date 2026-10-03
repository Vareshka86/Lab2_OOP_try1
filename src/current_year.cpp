/**
 * @file current_year.cpp
 * @brief Реализация функции currentYear().
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#include "current_year.h"

#include <ctime>

int currentYear()
{
    const std::time_t now = std::time(nullptr);     // секунды с 1 января 1970 года
    const std::tm* local = std::localtime(&now);    // разбор на год, месяц, день...
    return local->tm_year + 1900;                   // tm_year считает годы от 1900
}
