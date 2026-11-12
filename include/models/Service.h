#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <ostream>

class Service {
protected:
    int id;
    std::string title;
    double price;
    int duration_minutes;

public:
    Service();
    Service(int id, const std::string& title, double price, int duration_minutes);
    virtual ~Service() = default;

    int get_id() const;
    const std::string& get_title() const;

    virtual double calculate_cost() const;
    virtual int get_duration() const;

    void set_title(const std::string& title);
    void set_price(double price);
    void set_duration_minutes(int duration_minutes);

    friend std::ostream& operator<<(std::ostream& os, const Service& service);
};

#endif //SERVICE_H
