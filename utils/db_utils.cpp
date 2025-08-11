#include "db_utils.h"
#include <iostream>
#include <vector>

// --- Connection & Generic Execution ---
bool openDatabase(const std::string& dbName, sqlite3** db) {
    if (sqlite3_open(dbName.c_str(), db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(*db) << std::endl;
        return false;
    }
    return true;
}

void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
}

bool executeSql(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// --- Department Functions ---
bool insertDepartment(sqlite3* db, int id, const std::string& name) {
    std::string sql = "INSERT INTO departments (ID, NAME) VALUES (" + std::to_string(id) + ", '" + name + "');";
    return executeSql(db, sql);
}

std::vector<Department> selectAllDepartments(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, NAME FROM departments;";
    std::vector<Department> departments;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            departments.emplace_back(
                sqlite3_column_int(stmt, 0),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))
            );
        }
    }
    sqlite3_finalize(stmt);
    return departments;
}

// --- Project Functions ---
bool insertProject(sqlite3* db, int id, const std::string& name, int departmentId) {
    std::string sql = "INSERT INTO projects (ID, NAME, DEPARTMENT_ID) VALUES (" + std::to_string(id) + ", '" + name + "', " + std::to_string(departmentId) + ");";
    return executeSql(db, sql);
}

std::vector<Project> selectAllProjects(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, NAME, DEPARTMENT_ID FROM projects;";
    std::vector<Project> projects;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            projects.emplace_back(
                sqlite3_column_int(stmt, 0),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                sqlite3_column_int(stmt, 2)
            );
        }
    }
    sqlite3_finalize(stmt);
    return projects;
}

// --- Employee Functions ---
bool insertEmployee(sqlite3* db, int id, const std::string& name, int age, int departmentId) {
    std::string sql = "INSERT INTO employees (ID, NAME, AGE, DEPARTMENT_ID) VALUES (" + std::to_string(id) + ", '" + name + "', " + std::to_string(age) + ", " + std::to_string(departmentId) + ");";
    return executeSql(db, sql);
}

bool updateEmployeeName(sqlite3* db, int id, const std::string& newName) {
    std::string sql = "UPDATE employees SET NAME = '" + newName + "' WHERE ID = " + std::to_string(id) + ";";
    return executeSql(db, sql);
}

bool deleteEmployee(sqlite3* db, int id) {
    executeSql(db, "DELETE FROM employee_projects WHERE EMPLOYEE_ID = " + std::to_string(id) + ";");
    return executeSql(db, "DELETE FROM employees WHERE ID = " + std::to_string(id) + ";");
}

std::vector<Employee> selectAllEmployees(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT e.ID, e.NAME, e.AGE, d.NAME FROM employees e JOIN departments d ON e.DEPARTMENT_ID = d.ID;";
    std::vector<Employee> employees;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            employees.emplace_back(
                sqlite3_column_int(stmt, 0),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                sqlite3_column_int(stmt, 2),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))
            );
        }
    }
    sqlite3_finalize(stmt);
    return employees;
}

Employee selectEmployeeById(sqlite3* db, int id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT e.ID, e.NAME, e.AGE, d.NAME FROM employees e JOIN departments d ON e.DEPARTMENT_ID = d.ID WHERE e.ID = ?;";
    Employee employee(0, "", 0, ""); // Default empty employee
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            employee = Employee(
                sqlite3_column_int(stmt, 0),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                sqlite3_column_int(stmt, 2),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))
            );
        } else {
            std::cerr << "Employee with ID " << id << " not found." << std::endl;
        }
    }
    sqlite3_finalize(stmt);
    return employee;
}

std::vector<Employee> selectEmployeesByName(sqlite3* db, const std::string& name) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT e.ID, e.NAME, e.AGE, d.NAME FROM employees e JOIN departments d ON e.DEPARTMENT_ID = d.ID WHERE e.NAME LIKE ?;";
    std::vector<Employee> employees;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        std::string searchTerm = "%" + name + "%";
        sqlite3_bind_text(stmt, 1, searchTerm.c_str(), -1, SQLITE_TRANSIENT);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            employees.emplace_back(
                sqlite3_column_int(stmt, 0),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                sqlite3_column_int(stmt, 2),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))
            );
        }
    }
    sqlite3_finalize(stmt);
    return employees;
}

// --- Relationship Functions ---
bool assignEmployeeToProject(sqlite3* db, int employeeId, int projectId) {
    std::string sql = "INSERT INTO employee_projects (EMPLOYEE_ID, PROJECT_ID) VALUES (" + std::to_string(employeeId) + ", " + std::to_string(projectId) + ");";
    return executeSql(db, sql);
}

std::vector<Employee> selectEmployeesByProject(sqlite3* db, int projectId) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT e.ID, e.NAME, e.AGE, d.NAME FROM employees e "
                      "JOIN departments d ON e.DEPARTMENT_ID = d.ID "
                      "JOIN employee_projects ep ON e.ID = ep.EMPLOYEE_ID "
                      "WHERE ep.PROJECT_ID = ?;";
    std::vector<Employee> employees;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, projectId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            employees.emplace_back(
                sqlite3_column_int(stmt, 0),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                sqlite3_column_int(stmt, 2),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))
            );
        }
    }
    sqlite3_finalize(stmt);
    return employees;
}
