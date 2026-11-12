#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

class Person {
protected:
    int id;
    std::string name;
    std::string phone_number;

public:
    Person(int id, std::string name, std::string phone);

    virtual ~Person() = default;

    int get_id() const;
    const std::string& get_name() const;
    const std::string& get_phone_number() const;

    void set_name(const std::string& name);
    void set_phone_number(const std::string& phone);

    virtual void print_info() const = 0;
};
#endif //PERSON_H
