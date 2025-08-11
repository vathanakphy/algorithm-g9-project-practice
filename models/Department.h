#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
#include <iostream>

class Department {
private:
    int id;
    std::string name;

public:
    // Constructor
    Department(int id, const std::string& name) : id(id), name(name) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }

    // Display method
    void display() const {
        std::cout << "Dept ID: " << id << ", Name: " << name << std::endl;
    }
};

#endif // DEPARTMENT_H
