#ifndef SPASYSTEM_H
#define SPASYSTEM_H

#include <string>
#include <vector>
#include <memory>
#include "core/Repository.h"
#include "core/Exceptions.h"
#include "models/Client.h"
#include "models/Employee.h"
#include "models/Room.h"
#include "models/Service.h"
#include "models/SpaPackage.h"
#include "models/InventoryItem.h"
#include "models/Appointment.h"

class SpaSystem {
private:
    std::string data_directory;

    Repository<Client> client_repo;
    Repository<Employee> employee_repo;
    Repository<Room> room_repo;
    Repository<InventoryItem> inventory_repo;
    Repository<Appointment> appointment_repo;

    std::vector<std::shared_ptr<Service>> services_list;

    int generate_appointment_id();
    int generate_service_id();

public:
    explicit SpaSystem(std::string data_dir = "data");

    void load_all_data();
    void save_all_data();

    void schedule_appointment(int client_id, int employee_id, int room_id,
                               int service_id, const std::string& date_time);

    bool check_collision(int employee_id, int room_id,
                         const std::string& date_time, int duration_minutes) const;

    void complete_appointment(int appointment_id);

    void create_spa_package(const std::string& title, double discount,
                            const std::vector<int>& included_service_ids);

    std::shared_ptr<Service> find_service_by_id(int id) const;

    Repository<Client>& get_client_repo();
    Repository<Employee>& get_employee_repo();
    Repository<Room>& get_room_repo();
    Repository<InventoryItem>& get_inventory_repo();
    Repository<Appointment>& get_appointment_repo();
    const std::vector<std::shared_ptr<Service>>& get_services() const;
};
#endif //SPASYSTEM_H
