#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <string>
#include <vector>
#include "../db/sqlite3.h"
#include "../models/Employee.h"
#include "../models/Department.h"
#include "../models/Project.h"

// --- Connection & Generic Execution ---
bool openDatabase(const std::string& dbName, sqlite3** db);
void closeDatabase(sqlite3* db);
bool executeSql(sqlite3* db, const std::string& sql);

// --- Department CRUD ---
bool insertDepartment(sqlite3* db, int id, const std::string& name);
std::vector<Department> selectAllDepartments(sqlite3* db);

// --- Project CRUD ---
bool insertProject(sqlite3* db, int id, const std::string& name, int departmentId);
std::vector<Project> selectAllProjects(sqlite3* db);
std::vector<Project> selectProjectsByDepartment(sqlite3* db, int departmentId);

// --- Employee CRUD ---
bool insertEmployee(sqlite3* db, int id, const std::string& name, int age, int departmentId);
bool updateEmployeeName(sqlite3* db, int id, const std::string& newName);
bool deleteEmployee(sqlite3* db, int id);
std::vector<Employee> selectAllEmployees(sqlite3* db);
Employee selectEmployeeById(sqlite3* db, int id);
std::vector<Employee> selectEmployeesByName(sqlite3* db, const std::string& name);

// --- Relationship Management ---
bool assignEmployeeToProject(sqlite3* db, int employeeId, int projectId);
std::vector<Employee> selectEmployeesByProject(sqlite3* db, int projectId);

#endif // DB_UTILS_H
