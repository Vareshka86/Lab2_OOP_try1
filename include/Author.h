/**
 * @file Author.h
 * @brief Класс Author — автор книги.
 * @details Пользовательский тип данных для поля класса Book: требование
 * лабораторной работы — одно из полей класса должно быть объектом другого класса.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>

/**
 * @brief Автор книги: имя и год рождения.
 * @details Класс проверяет корректность своих данных при создании объекта.
 *
 * Инварианты (условия корректности):
 * - имя не пустое;
 * - год рождения равен UNKNOWN_BIRTH_YEAR (неизвестен) или лежит в
 *   диапазоне [1; текущий год].
 *
 * После создания автор не меняется: у класса нет методов изменения,
 * потому что имя и год рождения человека не меняются.
 */
class Author
{
public:
    /// Значение поля «год рождения», если год неизвестен.
    static const int UNKNOWN_BIRTH_YEAR = 0;

    /**
     * @brief Конструктор по умолчанию: автор не указан.
     * @details Создаёт корректный объект: имя «Автор не указан», год рождения
     * неизвестен.
     */
    Author();

    /**
     * @brief Параметризованный конструктор: автор с именем и годом рождения.
     * @param name      Имя автора, например «Лев Толстой».
     * @param birthYear Год рождения; UNKNOWN_BIRTH_YEAR (по умолчанию) — неизвестен.
     * @exception std::invalid_argument Если имя пустое или год рождения вне
     *            допустимого диапазона. Объект в этом случае не создаётся.
     */
    explicit Author(const std::string& name, int birthYear = UNKNOWN_BIRTH_YEAR);

    /**
     * @brief Возвращает имя автора.
     * @return Константная ссылка на имя (строка не копируется).
     */
    const std::string& getName() const;

    /**
     * @brief Возвращает год рождения.
     * @return Год рождения или UNKNOWN_BIRTH_YEAR, если он неизвестен.
     */
    int getBirthYear() const;

    /**
     * @brief Проверяет, известен ли год рождения.
     * @return true — год известен, false — неизвестен.
     */
    bool hasBirthYear() const;

    /**
     * @brief Возвращает описание автора одной строкой.
     * @return Например «Лев Толстой (род. 1828)» или «Автор не указан».
     */
    std::string toString() const;

private:
    std::string name_; ///< Имя автора (не пустое)
    int birthYear_;    ///< Год рождения или UNKNOWN_BIRTH_YEAR
};

#endif // AUTHOR_H
