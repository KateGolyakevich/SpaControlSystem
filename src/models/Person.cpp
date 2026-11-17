#include "models/Person.h"
#include <utility>

Person::Person(int id, std::string name, std::string phone)
    : id(id), name(std::move(name)), phone_number(std::move(phone)) {}

int Person::get_id() const {
    return id;
}

const std::string& Person::get_name() const {
    return name;
}

const std::string& Person::get_phone_number() const {
    return phone_number;
}

void Person::set_name(const std::string& new_name) {
    name = new_name;
}

void Person::set_phone_number(const std::string& new_phone) {
    phone_number = new_phone;
}