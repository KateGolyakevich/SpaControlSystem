#include "core/SpaSystem.h"
#include "core/FileManager.h"
#include <sstream>
#include <algorithm>
#include <iostream>

static int time_to_minutes(const std::string& time_str) {
    int hours = 0;
    int minutes = 0;
    char delimiter = ':';
    std::stringstream ss(time_str);
    ss >> hours >> delimiter >> minutes;
    return hours * 60 + minutes;
}

static bool is_time_overlapped(int start1, int end1, int start2, int end2) {
    return std::max(start1, start2) < std::min(end1, end2);
}

SpaSystem::SpaSystem(std::string data_dir)
    : data_directory(std::move(data_dir)) {
    load_all_data();
}

void SpaSystem::load_all_data() {
    try {
        FileManager::load_clients(data_directory + "/clients.txt", client_repo);
        FileManager::load_employees(data_directory + "/employees.txt", employee_repo);
        FileManager::load_rooms(data_directory + "/rooms.txt", room_repo);
        FileManager::load_services(data_directory + "/services.txt", services_list);
        FileManager::load_inventory(data_directory + "/inventory.txt", inventory_repo);
        FileManager::load_appointments(data_directory + "/appointments.txt", appointment_repo);
    } catch (const std::exception& e) {
        std::cerr << "Внимание при загрузке данных: " << e.what() << std::endl;
    }
}

void SpaSystem::save_all_data() {
    FileManager::save_clients(data_directory + "/clients.txt", client_repo);
    FileManager::save_employees(data_directory + "/employees.txt", employee_repo);
    FileManager::save_rooms(data_directory + "/rooms.txt", room_repo);
    FileManager::save_services(data_directory + "/services.txt", services_list);
    FileManager::save_inventory(data_directory + "/inventory.txt", inventory_repo);
    FileManager::save_appointments(data_directory + "/appointments.txt", appointment_repo);
}

std::shared_ptr<Service> SpaSystem::find_service_by_id(int id) const {
    for (const auto& service : services_list) {
        if (service->get_id() == id) {
            return service;
        }
    }
    return nullptr;
}

int SpaSystem::generate_appointment_id() {
    int max_id = 0;
    for (const auto& app : appointment_repo.get_all()) {
        if (app.get_id() > max_id) {
            max_id = app.get_id();
        }
    }
    return max_id + 1;
}

int SpaSystem::generate_service_id() {
    int max_id = 0;
    for (const auto& s : services_list) {
        if (s->get_id() > max_id) {
            max_id = s->get_id();
        }
    }
    return max_id + 1;
}

// АЛГОРИТМ ПРОВЕРКИ КОЛЛИЗИЙ РЕСУРСОВ (КИЛЛЕР-ФИЧА)
bool SpaSystem::check_collision(int employee_id, int room_id,
                               const std::string& date_time, int duration_minutes) const {
    std::string new_date = date_time.substr(0, 10);
    std::string new_time = date_time.substr(11, 5);

    int new_start = time_to_minutes(new_time);
    int new_end = new_start + duration_minutes;

    for (const auto& existing_app : appointment_repo.get_all()) {
        if (existing_app.get_is_completed()) {
            continue;
        }

        std::string ex_date_time = existing_app.get_date_time();
        std::string ex_date = ex_date_time.substr(0, 10);


        if (new_date != ex_date) {
            continue;
        }

        // Вычисляем длительность существующей процедуры
        auto existing_service = find_service_by_id(existing_app.get_service_id());
        int ex_duration = existing_service ? existing_service->get_duration() : 60;

        int ex_start = time_to_minutes(ex_date_time.substr(11, 5));
        int ex_end = ex_start + ex_duration;

        // Если интервалы пересекаются во времени
        if (is_time_overlapped(new_start, new_end, ex_start, ex_end)) {
            // Проверяем: занят ли мастер?
            if (existing_app.get_employee_id() == employee_id) {
                return true;
            }
            // Проверяем: занят ли кабинет/СПА-зона?
            if (existing_app.get_room_id() == room_id) {
                return true;
            }
        }
    }
    return false; // Ресурсы свободны!
}

// БРОНИРОВАНИЕ СЕАНСА С ВАЛИДАЦИЕЙ
void SpaSystem::schedule_appointment(int client_id, int employee_id, int room_id,
                                     int service_id, const std::string& date_time) {
    // 1. Проверяем существование сущностей
    if (!client_repo.find_by_id(client_id)) {
        throw EntityNotFoundException("Клиент", client_id);
    }
    auto employee = employee_repo.find_by_id(employee_id);
    if (!employee) {
        throw EntityNotFoundException("Сотрудник", employee_id);
    }
    auto room = room_repo.find_by_id(room_id);
    if (!room) {
        throw EntityNotFoundException("Кабинет", room_id);
    }
    auto service = find_service_by_id(service_id);
    if (!service) {
        throw EntityNotFoundException("Услуга", service_id);
    }

    if (check_collision(employee_id, room_id, date_time, service->get_duration())) {
        throw ResourceCollisionException("Мастер (" + employee->get_name() +
                                         ") или кабинет (" + room->get_name() +
                                         ") уже заняты на выбранное время!");
    }

    // 3. Создаем новую запись и сохраняем
    int new_id = generate_appointment_id();
    appointment_repo.add(Appointment(new_id, client_id, employee_id, room_id, service_id, date_time, false));
    save_all_data();
}

// ЗАВЕРШЕНИЕ СЕАНСА (СПИСАНИЕ МАТЕРИАЛОВ И РАСЧЕТ ОПЛАТЫ)
void SpaSystem::complete_appointment(int appointment_id) {
    auto app = appointment_repo.find_by_id(appointment_id);
    if (!app) {
        throw EntityNotFoundException("Запись на прием", appointment_id);
    }
    if (app->get_is_completed()) {
        return; // Уже завершен
    }

    auto client = client_repo.find_by_id(app->get_client_id());
    auto employee = employee_repo.find_by_id(app->get_employee_id());
    auto room = room_repo.find_by_id(app->get_room_id());
    auto service = find_service_by_id(app->get_service_id());

    if (!client || !employee || !room || !service) {
        throw SpaException("Нарушена целостность данных для завершения сеанса!");
    }

    // 1. расчет стоимости с учетом скидки клиента
    double base_cost = service->calculate_cost();
    double final_cost = base_cost * (1.0 - client->get_discount_rate());

    // 2. Начисление трат клиенту (обновляет скидку в Client)
    client->add_payment(final_cost);

    // 3. Автоматическое списание базовых расходников со склада (оператор -=)
    auto towels = inventory_repo.find_by_id(7);
    if (towels) {
        *towels -= 1.0;
    }
    auto sheets = inventory_repo.find_by_id(3);
    if (sheets) {
        *sheets -= 1.0; //
    }

    // 4. Помечаем визит завершенным
    app->complete_appointment();

    // 5. Экспортируем чек в текстовый файл
    std::string receipt_name = "data/receipt_app_" + std::to_string(app->get_id()) + ".txt";
    FileManager::export_receipt(receipt_name, *app, *client, *employee, *room, *service, final_cost);

    save_all_data();
}

// КОНСТРУКТОР СПА-ПАКЕТА (КИЛЛЕР-ФИЧА)
void SpaSystem::create_spa_package(const std::string& title, double discount,
                                   const std::vector<int>& included_service_ids) {
    int new_id = generate_service_id();
    auto new_package = std::make_shared<SpaPackage>(new_id, title, discount);

    for (int s_id : included_service_ids) {
        auto base_service = find_service_by_id(s_id);
        if (base_service) {
            new_package->add_service(*base_service);
        }
    }

    services_list.push_back(new_package);
    FileManager::save_services(data_directory + "/services.txt", services_list);
}

// ГЕТТЕРЫ ДЛЯ ИНТЕРФЕЙСА
Repository<Client>& SpaSystem::get_client_repo() { return client_repo; }
Repository<Employee>& SpaSystem::get_employee_repo() { return employee_repo; }
Repository<Room>& SpaSystem::get_room_repo() { return room_repo; }
Repository<InventoryItem>& SpaSystem::get_inventory_repo() { return inventory_repo; }
Repository<Appointment>& SpaSystem::get_appointment_repo() { return appointment_repo; }
const std::vector<std::shared_ptr<Service>>& SpaSystem::get_services() const { return services_list; }
