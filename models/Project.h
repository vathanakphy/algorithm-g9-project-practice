#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <iostream>

class Project {
private:
    int id;
    std::string name;
    int departmentId;

public:
    // Constructor
    Project(int id, const std::string& name, int departmentId)
        : id(id), name(name), departmentId(departmentId) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getDepartmentId() const { return departmentId; }

    // Display method
    void display() const {
        std::cout << "Project ID: " << id << ", Name: " << name << ", Owning Dept ID: " << departmentId << std::endl;
    }
};

#endif // PROJECT_H
