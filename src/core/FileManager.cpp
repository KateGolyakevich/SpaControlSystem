#include "core/FileManager.h"
#include "core/Exceptions.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

// 1. КЛИЕНТЫ
void FileManager::load_clients(const std::string& file_path, Repository<Client>& repo) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось открыть файл клиентов: " + file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_str, name, phone, discount_str, spent_str;

        if (std::getline(ss, id_str, ';') &&
            std::getline(ss, name, ';') &&
            std::getline(ss, phone, ';') &&
            std::getline(ss, discount_str, ';') &&
            std::getline(ss, spent_str, ';')) {

            int id = std::stoi(id_str);
            double discount = std::stod(discount_str);
            double spent = std::stod(spent_str);

            repo.add(Client(id, name, phone, discount, spent));
        }
    }
}

void FileManager::save_clients(const std::string& file_path, const Repository<Client>& repo) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось записать в файл клиентов: " + file_path);
    }

    for (const auto& client : repo.get_all()) {
        file << client << "\n";
    }
}

// 2. СОТРУДНИКИ (МАСТЕРА)
void FileManager::load_employees(const std::string& file_path, Repository<Employee>& repo) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось открыть файл сотрудников: " + file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_str, name, phone, specialty, rate_str;

        if (std::getline(ss, id_str, ';') &&
            std::getline(ss, name, ';') &&
            std::getline(ss, phone, ';') &&
            std::getline(ss, specialty, ';') &&
            std::getline(ss, rate_str, ';')) {

            int id = std::stoi(id_str);
            double rate = std::stod(rate_str);

            repo.add(Employee(id, name, phone, specialty, rate));
        }
    }
}

void FileManager::save_employees(const std::string& file_path, const Repository<Employee>& repo) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось записать в файл сотрудников: " + file_path);
    }

    for (const auto& employee : repo.get_all()) {
        file << employee << "\n";
    }
}

// 3. КАБИНЕТЫ
void FileManager::load_rooms(const std::string& file_path, Repository<Room>& repo) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось открыть файл кабинетов: " + file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_str, name, type;

        if (std::getline(ss, id_str, ';') &&
            std::getline(ss, name, ';') &&
            std::getline(ss, type, ';')) {

            repo.add(Room(std::stoi(id_str), name, type));
        }
    }
}

void FileManager::save_rooms(const std::string& file_path, const Repository<Room>& repo) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось записать в файл кабинетов: " + file_path);
    }

    for (const auto& room : repo.get_all()) {
        file << room << "\n";
    }
}

// 4. УСЛУГИ И СПА-ПАКЕТЫ (ПОЛИМОРФИЗМ)
void FileManager::load_services(const std::string& file_path, std::vector<std::shared_ptr<Service>>& services) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось открыть файл услуг: " + file_path);
    }

    services.clear();
    std::string line;

    struct DelayedPackage {
        int id;
        std::string title;
        double discount;
        std::string sub_ids_str;
    };
    std::vector<DelayedPackage> packages_to_build;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string type_tag;
        std::getline(ss, type_tag, ';');

        if (type_tag == "SERVICE") {
            // Чтение одиночной услуги
            std::string id_str, title, price_str, dur_str;
            std::getline(ss, id_str, ';');
            std::getline(ss, title, ';');
            std::getline(ss, price_str, ';');
            std::getline(ss, dur_str, ';');

            services.push_back(std::make_shared<Service>(
                std::stoi(id_str), title, std::stod(price_str), std::stoi(dur_str)
            ));
        } else if (type_tag == "PACKAGE") {
            // Чтение комплексного пакета
            std::string id_str, title, disc_str, sub_ids;
            std::getline(ss, id_str, ';');
            std::getline(ss, title, ';');
            std::getline(ss, disc_str, ';');
            std::getline(ss, sub_ids, ';');

            packages_to_build.push_back({
                std::stoi(id_str), title, std::stod(disc_str), sub_ids
            });
        }
    }

    // Собираем пакеты из уже загруженных одиночных услуг
    for (const auto& pkg_info : packages_to_build) {
        auto spa_pkg = std::make_shared<SpaPackage>(pkg_info.id, pkg_info.title, pkg_info.discount);
        std::stringstream sub_ss(pkg_info.sub_ids_str);
        std::string sub_id_str;

        while (std::getline(sub_ss, sub_id_str, ',')) {
            if (sub_id_str.empty()) continue;
            int target_id = std::stoi(sub_id_str);

            for (const auto& s : services) {
                if (s->get_id() == target_id) {
                    spa_pkg->add_service(*s);
                    break;
                }
            }
        }
        services.push_back(spa_pkg);
    }
}

void FileManager::save_services(const std::string& file_path, const std::vector<std::shared_ptr<Service>>& services) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось записать в файл услуг: " + file_path);
    }

    for (const auto& service_ptr : services) {
        // Проверяем через dynamic_cast, пакет это или обычная услуга
        auto pkg_ptr = std::dynamic_pointer_cast<SpaPackage>(service_ptr);
        if (pkg_ptr) {
            file << *pkg_ptr << "\n";
        } else {
            file << *service_ptr << "\n";
        }
    }
}

// 5. СКЛАД
void FileManager::load_inventory(const std::string& file_path, Repository<InventoryItem>& repo) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось открыть файл склада: " + file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_str, name, qty_str, unit;

        if (std::getline(ss, id_str, ';') &&
            std::getline(ss, name, ';') &&
            std::getline(ss, qty_str, ';') &&
            std::getline(ss, unit, ';')) {

            repo.add(InventoryItem(std::stoi(id_str), name, std::stod(qty_str), unit));
        }
    }
}

void FileManager::save_inventory(const std::string& file_path, const Repository<InventoryItem>& repo) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось записать в файл склада: " + file_path);
    }

    for (const auto& item : repo.get_all()) {
        file << item << "\n";
    }
}

// 6. ЖУРНАЛ ЗАПИСЕЙ (БРОНИРОВАНИЯ)
void FileManager::load_appointments(const std::string& file_path, Repository<Appointment>& repo) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось открыть файл записей: " + file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_str, c_id, e_id, r_id, s_id, dt, comp_str;

        if (std::getline(ss, id_str, ';') &&
            std::getline(ss, c_id, ';') &&
            std::getline(ss, e_id, ';') &&
            std::getline(ss, r_id, ';') &&
            std::getline(ss, s_id, ';') &&
            std::getline(ss, dt, ';') &&
            std::getline(ss, comp_str, ';')) {

            repo.add(Appointment(
                std::stoi(id_str),
                std::stoi(c_id),
                std::stoi(e_id),
                std::stoi(r_id),
                std::stoi(s_id),
                dt,
                (comp_str == "1")
            ));
        }
    }
}

void FileManager::save_appointments(const std::string& file_path, const Repository<Appointment>& repo) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось записать в файл записей: " + file_path);
    }

    for (const auto& app : repo.get_all()) {
        file << app << "\n";
    }
}

// 7. ЭКСПОРТ ЧЕКА
void FileManager::export_receipt(const std::string& file_path,
                                const Appointment& app,
                                const Client& client,
                                const Employee& emp,
                                const Room& room,
                                const Service& service,
                                double final_price) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw FileProcessingException("Не удалось создать файл чека: " + file_path);
    }

    file << "========================================\n";
    file << "          СПА-КОМПЛЕКС \"AuraSPA\"       \n";
    file << "             КАССОВЫЙ ЧЕК               \n";
    file << "========================================\n";
    file << "Номер чека (брони): #" << app.get_id() << "\n";
    file << "Дата и время:       " << app.get_date_time() << "\n";
    file << "----------------------------------------\n";
    file << "Клиент:             " << client.get_name() << "\n";
    file << "Телефон:            " << client.get_phone_number() << "\n";
    file << "Персональная скидка:" << (client.get_discount_rate() * 100) << "%\n";
    file << "----------------------------------------\n";
    file << "Услуга / Программа: " << service.get_title() << "\n";
    file << "Длительность:       " << service.get_duration() << " мин.\n";
    file << "Специалист:         " << emp.get_name() << " (" << emp.get_specialty() << ")\n";
    file << "Кабинет / Зона:     " << room.get_name() << "\n";
    file << "----------------------------------------\n";
    file << "Базовая стоимость:  " << std::fixed << std::setprecision(2) << service.calculate_cost() << " руб.\n";
    file << "ИТОГО К ОПЛАТЕ:     " << final_price << " руб.\n";
    file << "========================================\n";
    file << "         Спасибо, что выбрали нас!      \n";
    file << "========================================\n";
}
