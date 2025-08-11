#include <iostream>
#include "utils/db_utils.h"

int main() {
    sqlite3 *db;
    if (!openDatabase("tech_company.db", &db)) return 1;

    std::cout << "Seeding database: tech_company.db" << std::endl;

    // Drop tables to ensure a clean slate
    executeSql(db, "DROP TABLE IF EXISTS employee_projects;");
    executeSql(db, "DROP TABLE IF EXISTS projects;");
    executeSql(db, "DROP TABLE IF EXISTS employees;");
    executeSql(db, "DROP TABLE IF EXISTS departments;");
    std::cout << "Old tables dropped." << std::endl;

    // --- Create Tables ---
    const char* createDepartments = "CREATE TABLE departments (ID INTEGER PRIMARY KEY, NAME TEXT NOT NULL UNIQUE);";
    const char* createEmployees = "CREATE TABLE employees (ID INTEGER PRIMARY KEY, NAME TEXT NOT NULL, AGE INTEGER, DEPARTMENT_ID INTEGER, FOREIGN KEY(DEPARTMENT_ID) REFERENCES departments(ID));";
    const char* createProjects = "CREATE TABLE projects (ID INTEGER PRIMARY KEY, NAME TEXT NOT NULL, DEPARTMENT_ID INTEGER, FOREIGN KEY(DEPARTMENT_ID) REFERENCES departments(ID));";
    const char* createEmployeeProjects = "CREATE TABLE employee_projects (EMPLOYEE_ID INTEGER, PROJECT_ID INTEGER, FOREIGN KEY(EMPLOYEE_ID) REFERENCES employees(ID), FOREIGN KEY(PROJECT_ID) REFERENCES projects(ID), PRIMARY KEY(EMPLOYEE_ID, PROJECT_ID));";

    executeSql(db, createDepartments);
    executeSql(db, createEmployees);
    executeSql(db, createProjects);
    executeSql(db, createEmployeeProjects);
    std::cout << "New tables created successfully." << std::endl;

    // --- Seed Data ---
    insertDepartment(db, 1, "Engineering");
    insertDepartment(db, 2, "Marketing");
    insertDepartment(db, 3, "Product");

    insertEmployee(db, 101, "Alice", 30, 1); // Eng
    insertEmployee(db, 102, "Bob", 28, 2);   // Mkt
    insertEmployee(db, 103, "Charlie", 35, 1); // Eng
    insertEmployee(db, 104, "Diana", 42, 3);  // Prod

    insertProject(db, 1, "Project Phoenix", 1); // Eng
    insertProject(db, 2, "Market Research Q3", 2); // Mkt
    insertProject(db, 3, "UI/UX Overhaul", 3); // Prod

    assignEmployeeToProject(db, 101, 1); // Alice on Phoenix
    assignEmployeeToProject(db, 103, 1); // Charlie on Phoenix
    assignEmployeeToProject(db, 102, 2); // Bob on Market Research
    assignEmployeeToProject(db, 104, 3); // Diana on UI/UX
    assignEmployeeToProject(db, 101, 3); // Alice also on UI/UX

    std::cout << "Initial data inserted." << std::endl;
    closeDatabase(db);
    std::cout << "Seeding complete." << std::endl;
    return 0;
}
