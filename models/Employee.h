#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <iostream>

// Base class demonstrating encapsulation
class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& name, int age) : name(name), age(age) {}
    virtual ~Person() {} // Virtual destructor for proper cleanup

    std::string getName() const { return name; }
    int getAge() const { return age; }

    virtual void display() const {
        std::cout << "Name: " << name << ", Age: " << age;
    }
};

// Derived class demonstrating inheritance
class Employee : public Person {
private:
    int id;
    std::string departmentName; // Store the name from the JOIN

public:
    Employee(int id, const std::string& name, int age, const std::string& departmentName)
        : Person(name, age), id(id), departmentName(departmentName) {}

    int getId() const { return id; }
    std::string getDepartmentName() const { return departmentName; }

    // Override the display method to show all employee details
    void display() const override {
        std::cout << "ID: " << id << ", ";
        Person::display(); // Call base class method
        std::cout << ", Department: " << departmentName << std::endl;
    }
};

#endif // EMPLOYEE_H
