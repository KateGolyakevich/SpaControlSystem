#include "models/Appointment.h"

Appointment::Appointment()
    : id(0), client_id(0), employee_id(0), room_id(0),
      service_id(0), date_time(""), is_completed(false) {}

Appointment::Appointment(int id, int client_id, int employee_id, int room_id,
                         int service_id, const std::string& date_time, bool is_completed)
    : id(id), client_id(client_id), employee_id(employee_id), room_id(room_id),
      service_id(service_id), date_time(date_time), is_completed(is_completed) {}

int Appointment::get_id() const {
    return id;
}

int Appointment::get_client_id() const {
    return client_id;
}

int Appointment::get_employee_id() const {
    return employee_id;
}

int Appointment::get_room_id() const {
    return room_id;
}

int Appointment::get_service_id() const {
    return service_id;
}

const std::string& Appointment::get_date_time() const {
    return date_time;
}

bool Appointment::get_is_completed() const {
    return is_completed;
}

void Appointment::complete_appointment() {
    is_completed = true;
}

bool Appointment::operator==(const Appointment& other) const {
    if (id == other.id) {
        return true;
    }
    bool time_match = (date_time == other.date_time);
    bool master_match = (employee_id == other.employee_id);
    bool room_match = (room_id == other.room_id);

    return time_match && (master_match || room_match);
}

std::ostream& operator<<(std::ostream& os, const Appointment& appointment) {
    os << appointment.id << ";"
       << appointment.client_id << ";"
       << appointment.employee_id << ";"
       << appointment.room_id << ";"
       << appointment.service_id << ";"
       << appointment.date_time << ";"
       << (appointment.is_completed ? 1 : 0);
    return os;
}