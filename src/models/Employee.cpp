#include "models/Employee.h"
#include <iostream>
#include <utility>

Employee::Employee()
    : Person(0, "", ""), specialty(""), salary_rate(0.0) {}

Employee::Employee(int id, const std::string& name, const std::string& phone_number,
                   std::string specialty, double salary_rate)
    : Person(id, name, phone_number),
      specialty(std::move(specialty)),
      salary_rate(salary_rate) {}

const std::string& Employee::get_specialty() const {
    return specialty;
}

double Employee::get_salary_rate() const {
    return salary_rate;
}

void Employee::set_specialty(const std::string& new_specialty) {
    specialty = new_specialty;
}

void Employee::set_salary_rate(double new_rate) {
    salary_rate = new_rate;
}

void Employee::print_info() const {
    std::cout << "[Сотрудник ID: " << id << "] " << name
              << " | Должность: " << specialty
              << " | Тел: " << phone_number
              << " | Ставка за сеанс: " << salary_rate << " руб.\n";
}

std::ostream& operator<<(std::ostream& os, const Employee& employee) {
    os << employee.id << ";"
       << employee.name << ";"
       << employee.phone_number << ";"
       << employee.specialty << ";"
       << employee.salary_rate;
    return os;
}