#include "models/Service.h"
#include <iomanip>

Service::Service()
    : id(0), title(""), price(0.0), duration_minutes(0) {}

Service::Service(int id, const std::string& title, double price, int duration_minutes)
    : id(id), title(title), price(price), duration_minutes(duration_minutes) {}

int Service::get_id() const {
    return id;
}

const std::string& Service::get_title() const {
    return title;
}

double Service::calculate_cost() const {
    return price;
}

int Service::get_duration() const {
    return duration_minutes;
}

void Service::set_title(const std::string& new_title) {
    title = new_title;
}

void Service::set_price(double new_price) {
    price = new_price;
}

void Service::set_duration_minutes(int new_duration) {
    duration_minutes = new_duration;
}

std::ostream& operator<<(std::ostream& os, const Service& service) {
    os << "SERVICE;"
       << service.id << ";"
       << service.title << ";"
       << std::fixed << std::setprecision(2) << service.price << ";"
       << service.duration_minutes;
    return os;
}
