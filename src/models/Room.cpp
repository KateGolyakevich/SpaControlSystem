#include "models/Room.h"

Room::Room()
    : id(0), name(""), type("") {}

Room::Room(int id, const std::string& name, const std::string& type)
    : id(id), name(name), type(type) {}

int Room::get_id() const {
    return id;
}

const std::string& Room::get_name() const {
    return name;
}

const std::string& Room::get_type() const {
    return type;
}

void Room::set_name(const std::string& new_name) {
    name = new_name;
}

void Room::set_type(const std::string& new_type) {
    type = new_type;
}

bool Room::operator==(const Room& other) const {
    return id == other.id;
}


std::ostream& operator<<(std::ostream& os, const Room& room) {
    os << room.id << ";"
       << room.name << ";"
       << room.type;
    return os;
}
