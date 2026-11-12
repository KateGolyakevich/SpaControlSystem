#ifndef SPAPACKAGE_H
#define SPAPACKAGE_H

#include <vector>
#include <ostream>
#include "Service.h"

class SpaPackage : public Service {
private:
    std::vector<Service> package_services;
    double combo_discount;

public:
    SpaPackage();
    SpaPackage(int id, const std::string& title, double combo_discount = 0.15);

    void add_service(const Service& service);
    const std::vector<Service>& get_package_services() const;
    double get_combo_discount() const;
    void set_combo_discount(double discount);

    double calculate_cost() const override;
    int get_duration() const override;

    friend std::ostream& operator<<(std::ostream& os, const SpaPackage& spa_package);
};
#endif //SPAPACKAGE_H
