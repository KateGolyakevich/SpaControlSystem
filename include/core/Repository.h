#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <algorithm>
#include "Exceptions.h"

template <typename T>
class Repository {
private:
    std::vector<T> items;

public:
    Repository() = default;

    void add(const T& item) {
        items.push_back(item);
    }

    bool remove_by_id(int id) {
        auto it = std::remove_if(items.begin(), items.end(), [id](const T& item) {
            return item.get_id() == id;
        });

        if (it != items.end()) {
            items.erase(it, items.end());
            return true;
        }
        return false;
    }

    T* find_by_id(int id) {
        for (auto& item : items) {
            if (item.get_id() == id) {
                return &item;
            }
        }
        return nullptr;
    }

    const T* find_by_id(int id) const {
        for (const auto& item : items) {
            if (item.get_id() == id) {
                return &item;
            }
        }
        return nullptr;
    }

    std::vector<T>& get_all() {
        return items;
    }

    const std::vector<T>& get_all() const {
        return items;
    }

    size_t size() const {
        return items.size();
    }

    void clear() {
        items.clear();
    }
};

template <typename T, typename Predicate>
std::vector<T> filter_items(const std::vector<T>& collection, Predicate predicate) {
    std::vector<T> filtered_result;
    for (const auto& item : collection) {
        if (predicate(item)) {
            filtered_result.push_back(item);
        }
    }
    return filtered_result;
}

template <typename T, typename Predicate>
const T* find_first_match(const std::vector<T>& collection, Predicate predicate) {
    for (const auto& item : collection) {
        if (predicate(item)) {
            return &item;
        }
    }
    return nullptr;
}

#endif //REPOSITORY_H
