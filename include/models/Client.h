#ifndef CLIENT_H
#define CLIENT_H

#include "models/Person.h"
#include <ostream>

class Client : public Person {
private:
    double discount_rate;
    double total_spent;

    void update_discount();

public:
    Client();
    Client(int id, const std::string& name, const std::string& phone,
           double discount_rate = 0.0, double total_spent = 0.0);

    double get_discount_rate() const;
    double get_total_spent() const;

    void add_payment(double amount);

    void print_info() const override;

    friend std::ostream& operator<<(std::ostream& os, const Client& client);
};
#endif //CLIENT_H
