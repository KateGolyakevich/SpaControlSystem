#include "models/Client.h"
#include <iostream>
#include <iomanip>

Client::Client()
    : Person(0, "", ""), discount_rate(0.0), total_spent(0.0) {}

Client::Client(int id, const std::string& name, const std::string& phone,
               double discount_rate, double total_spent)
    : Person(id, name, phone), discount_rate(discount_rate), total_spent(total_spent) {
    update_discount();
}

void Client::update_discount() {
    if (total_spent >= 50000.0) {
        discount_rate = 0.15;
    } else if (total_spent >= 25000.0) {
        discount_rate = 0.10;
    } else if (total_spent >= 10000.0) {
        discount_rate = 0.05;
    }
}

double Client::get_discount_rate() const {
    return discount_rate;
}

double Client::get_total_spent() const {
    return total_spent;
}

void Client::add_payment(double amount) {
    if (amount > 0.0) {
        total_spent += amount;
        update_discount();
    }
}

void Client::print_info() const {
    std::cout << "[Клиент ID: " << id << "] " << name
              << " | Тел: " << phone_number
              << " | Скидка: " << (discount_rate * 100) << "%"
              << " | Потрачено всего: " << total_spent << " руб.\n";
}

std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << client.id << ";"
       << client.name << ";"
       << client.phone_number << ";"
       << std::fixed << std::setprecision(2) << client.discount_rate << ";"
       << client.total_spent;
    return os;
}