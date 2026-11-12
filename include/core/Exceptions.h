#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>

// Базовый класс для всех ошибок спа-салона (наследуется от стандартного std::exception)
class SpaException : public std::exception {
protected:
    std::string error_message;

public:
    explicit SpaException(std::string message) : error_message(std::move(message)) {}

    // Переопределение метода what() для получения текста ошибки
    const char* what() const noexcept override {
        return error_message.c_str();
    }
};

// Ошибка: Ресурсный конфликт (накладка во времени мастера или кабинета)
class ResourceCollisionException : public SpaException {
public:
    explicit ResourceCollisionException(const std::string& details)
        : SpaException("Конфликт бронирования: " + details) {}
};

// Ошибка: Недостаток расходных материалов на складе
class InsufficientInventoryException : public SpaException {
public:
    explicit InsufficientInventoryException(const std::string& item_name)
        : SpaException("Недостаточно товара на складе: " + item_name) {}
};

// Ошибка: Сущность с указанным ID не найдена в базе данных
class EntityNotFoundException : public SpaException {
public:
    explicit EntityNotFoundException(const std::string& entity_name, int id)
        : SpaException(entity_name + " с ID " + std::to_string(id) + " не найден(а)!") {}
};

// Ошибка: Проблемы при открытии или чтении файла базы данных
class FileProcessingException : public SpaException {
public:
    explicit FileProcessingException(const std::string& filename)
        : SpaException("Ошибка работы с файлом: " + filename) {}
};

#endif //EXCEPTIONS_H
