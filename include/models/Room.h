#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <ostream>

class Room {
private:
    int id;
    std::string name;
    std::string type;

public:
    Room();
    Room(int id, const std::string& name, const std::string& type);

    int get_id() const;
    const std::string& get_name() const;
    const std::string& get_type() const;

    void set_name(const std::string& name);
    void set_type(const std::string& type);

    bool operator==(const Room& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Room& room);
};

#endif //ROOM_H
