#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include <vector>
#include <memory>
#include "Repository.h"
#include "models/Client.h"
#include "models/Employee.h"
#include "models/Room.h"
#include "models/Service.h"
#include "models/SpaPackage.h"
#include "models/InventoryItem.h"
#include "models/Appointment.h"

class FileManager {
public:
    static void load_clients(const std::string& file_path, Repository<Client>& repo);
    static void save_clients(const std::string& file_path, const Repository<Client>& repo);

    static void load_employees(const std::string& file_path, Repository<Employee>& repo);
    static void save_employees(const std::string& file_path, const Repository<Employee>& repo);

    static void load_rooms(const std::string& file_path, Repository<Room>& repo);
    static void save_rooms(const std::string& file_path, const Repository<Room>& repo);

    static void load_services(const std::string& file_path, std::vector<std::shared_ptr<Service>>& services);
    static void save_services(const std::string& file_path, const std::vector<std::shared_ptr<Service>>& services);

    static void load_inventory(const std::string& file_path, Repository<InventoryItem>& repo);
    static void save_inventory(const std::string& file_path, const Repository<InventoryItem>& repo);

    static void load_appointments(const std::string& file_path, Repository<Appointment>& repo);
    static void save_appointments(const std::string& file_path, const Repository<Appointment>& repo);

    static void export_receipt(const std::string& file_path,
                               const Appointment& app,
                               const Client& client,
                               const Employee& emp,
                               const Room& room,
                               const Service& service,
                               double final_price);
};
#endif //FILEMANAGER_H
