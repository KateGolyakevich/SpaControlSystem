#include "models/InventoryItem.h"
#include "core/Exceptions.h"
#include <iomanip>

InventoryItem::InventoryItem()
    : id(0), name(""), quantity(0.0), unit("") {}

InventoryItem::InventoryItem(int id, const std::string& name, double quantity, const std::string& unit)
    : id(id), name(name), quantity(quantity), unit(unit) {}

int InventoryItem::get_id() const {
    return id;
}

const std::string& InventoryItem::get_name() const {
    return name;
}

double InventoryItem::get_quantity() const {
    return quantity;
}

const std::string& InventoryItem::get_unit() const {
    return unit;
}

void InventoryItem::set_quantity(double new_quantity) {
    quantity = new_quantity;
}

InventoryItem& InventoryItem::operator-=(double amount) {
    if (amount > quantity) {
        throw InsufficientInventoryException(name);
    }
    quantity -= amount;
    return *this;
}

InventoryItem& InventoryItem::operator+=(double amount) {
    if (amount > 0.0) {
        quantity += amount;
    }
    return *this;
}

bool InventoryItem::operator<(const InventoryItem& other) const {
    return quantity < other.quantity;
}

std::ostream& operator<<(std::ostream& os, const InventoryItem& item) {
    os << item.id << ";"
       << item.name << ";"
       << std::fixed << std::setprecision(1) << item.quantity << ";"
       << item.unit;
    return os;
}
