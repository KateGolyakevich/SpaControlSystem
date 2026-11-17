#include "models/SpaPackage.h"
#include <iomanip>
#include <numeric>

SpaPackage::SpaPackage()
    : Service(0, "", 0.0, 0), combo_discount(0.15) {}

SpaPackage::SpaPackage(int id, const std::string& title, double combo_discount)
    : Service(id, title, 0.0, 0), combo_discount(combo_discount) {}

void SpaPackage::add_service(const Service& service) {
    package_services.push_back(service);
}

const std::vector<Service>& SpaPackage::get_package_services() const {
    return package_services;
}

double SpaPackage::get_combo_discount() const {
    return combo_discount;
}

void SpaPackage::set_combo_discount(double discount) {
    if (discount >= 0.0 && discount < 1.0) {
        combo_discount = discount;
    }
}

double SpaPackage::calculate_cost() const {
    double total = 0.0;
    for (const auto& service : package_services) {
        total += service.calculate_cost();
    }
    return total * (1.0 - combo_discount);
}

int SpaPackage::get_duration() const {
    int total_duration = 0;
    for (const auto& service : package_services) {
        total_duration += service.get_duration();
    }
    return total_duration;
}

std::ostream& operator<<(std::ostream& os, const SpaPackage& spa_package) {
    os << "PACKAGE;"
       << spa_package.id << ";"
       << spa_package.title << ";"
       << std::fixed << std::setprecision(2) << spa_package.combo_discount << ";";

    for (size_t i = 0; i < spa_package.package_services.size(); ++i) {
        os << spa_package.package_services[i].get_id();
        if (i + 1 < spa_package.package_services.size()) {
            os << ",";
        }
    }
    return os;
}
