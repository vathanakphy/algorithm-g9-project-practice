#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "utils/db_utils.h"

// --- UI Helper Functions ---
void printMainMenu() {
    std::cout << "\n--- Company Management System ---\n";
    std::cout << "1. Manage Employees\n";
    std::cout << "2. Manage Departments\n";
    std::cout << "3. Manage Projects\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your choice: ";
}

void printEmployeeMenu() {
    std::cout << "\n-- Employee Menu --\n";
    std::cout << "1. List all employees\n";
    std::cout << "2. Find employee by ID\n";
    std::cout << "3. Add a new employee\n";
    std::cout << "4. Delete an employee\n";
    std::cout << "5. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void printProjectMenu() {
    std::cout << "\n-- Project Menu --\n";
    std::cout << "1. List all projects\n";
    std::cout << "2. List employees on a project\n";
    std::cout << "3. Assign an employee to a project\n";
    std::cout << "4. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// --- Handler Functions ---
void handleListAllDepartments(sqlite3* db) {
    std::vector<Department> depts = selectAllDepartments(db);
    std::cout << "\n--- All Departments ---\n";
    for (const auto& d : depts) {
        d.display();
    }
}

void handleEmployeeMenu(sqlite3* db) {
    int choice = 0;
    while (choice != 5) {
        printEmployeeMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            clearInputBuffer();
            continue;
        }

        if (choice == 1) {
            std::vector<Employee> employees = selectAllEmployees(db);
            std::cout << "\n--- All Employees ---\n";
            for (const auto& emp : employees) emp.display();
        } else if (choice == 2) {
            int id;
            std::cout << "Enter employee ID: ";
            std::cin >> id;
            Employee emp = selectEmployeeById(db, id);
            if(emp.getId() != 0) emp.display();
        } else if (choice == 3) {
            int id, age, deptId;
            std::string name;
            std::cout << "Enter new employee ID: "; std::cin >> id;
            std::cout << "Enter name: "; clearInputBuffer(); std::getline(std::cin, name);
            std::cout << "Enter age: "; std::cin >> age;
            std::cout << "Enter department ID: "; std::cin >> deptId;
            if (insertEmployee(db, id, name, age, deptId)) std::cout << "Employee added.\n";
        } else if (choice == 4) {
            int id;
            std::cout << "Enter employee ID to delete: "; std::cin >> id;
            if (deleteEmployee(db, id)) std::cout << "Employee deleted.\n";
        }
    }
}

void handleProjectMenu(sqlite3* db) {
    int choice = 0;
    while (choice != 4) {
        printProjectMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            clearInputBuffer();
            continue;
        }

        if (choice == 1) {
            std::vector<Project> projects = selectAllProjects(db);
            std::cout << "\n--- All Projects ---\n";
            for (const auto& p : projects) p.display();
        } else if (choice == 2) {
            int projectId;
            std::cout << "Enter project ID to see employees: ";
            std::cin >> projectId;
            std::vector<Employee> employees = selectEmployeesByProject(db, projectId);
            std::cout << "\n--- Employees on Project " << projectId << " ---\n";
            for (const auto& emp : employees) emp.display();
        } else if (choice == 3) {
            int empId, projId;
            std::cout << "Enter employee ID: "; std::cin >> empId;
            std::cout << "Enter project ID: "; std::cin >> projId;
            if (assignEmployeeToProject(db, empId, projId)) std::cout << "Assignment successful.\n";
        }
    }
}


// --- Main Application Loop ---
int main() {
    sqlite3 *db;
    if (!openDatabase("tech_company.db", &db)) return 1;

    int choice = 0;
    while (choice != 4) {
        printMainMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
                handleEmployeeMenu(db);
                break;
            case 2:
                handleListAllDepartments(db);
                break;
            case 3:
                handleProjectMenu(db);
                break;
            case 4:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    closeDatabase(db);
    return 0;
}
