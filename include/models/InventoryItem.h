#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H
#include <string>
#include <ostream>

class InventoryItem {
private:
    int id;
    std::string name;
    double quantity;
    std::string unit;

public:
    InventoryItem();
    InventoryItem(int id, const std::string& name, double quantity, const std::string& unit);

    int get_id() const;
    const std::string& get_name() const;
    double get_quantity() const;
    const std::string& get_unit() const;

    void set_quantity(double quantity);

    InventoryItem& operator-=(double amount);

    InventoryItem& operator+=(double amount);

    bool operator<(const InventoryItem& other) const;

    friend std::ostream& operator<<(std::ostream& os, const InventoryItem& item);
};
#endif //INVENTORYITEM_H
