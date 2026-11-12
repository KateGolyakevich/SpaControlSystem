#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"
#include <ostream>

class Employee : public Person {
private:
    std::string specialty;
    double salary_rate;

public:
    // Конструкторы
    Employee();
    Employee(int id, const std::string& name, const std::string& phone_number,
             std::string specialty, double salary_rate);

    const std::string& get_specialty() const;
    double get_salary_rate() const;
    void set_specialty(const std::string& specialty);
    void set_salary_rate(double salaryRate);

    void print_info() const override;

    friend std::ostream& operator<<(std::ostream& os, const Employee& employee);
};
#endif //EMPLOYEE_H
