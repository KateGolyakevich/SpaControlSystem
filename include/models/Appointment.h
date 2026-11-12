#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include <ostream>

class Appointment {
private:
    int id;
    int client_id;
    int employee_id;
    int room_id;
    int service_id;
    std::string date_time;
    bool is_completed;

public:
    Appointment();
    Appointment(int id, int client_id, int employee_id, int room_id,
                int service_id, const std::string& date_time, bool is_completed = false);

    int get_id() const;
    int get_client_id() const;
    int get_employee_id() const;
    int get_room_id() const;
    int get_service_id() const;
    const std::string& get_date_time() const;
    bool get_is_completed() const;

    void complete_appointment();

    bool operator==(const Appointment& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Appointment& appointment);
};

#endif //APPOINTMENT_H
