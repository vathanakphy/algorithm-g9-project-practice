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


// --- Employee CRUD ---

bool insertEmployee(sqlite3* db, int id, const std::string& name, int age, const std::string& department) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO employees (ID, NAME, AGE, DEPARTMENT) VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, age);
    sqlite3_bind_text(stmt, 4, department.c_str(), -1, SQLITE_STATIC);

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!result) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    }
    return result;
}

bool updateEmployeeName(sqlite3* db, int id, const std::string& newName) {
    std::string sql = "UPDATE employees SET NAME = '" + newName + "' WHERE ID = " + std::to_string(id) + ";";
    return executeSql(db, sql);
}

bool deleteEmployee(sqlite3* db, int id) {
    std::string sql = "DELETE FROM employees WHERE ID = " + std::to_string(id) + ";";
    return executeSql(db, sql);
}

// Callback function to populate a vector of Employee objects
static int selectCallback(void* data, int argc, char** argv, char** azColName) {
    auto* employees = static_cast<std::vector<Employee>*>(data);
    int id = argv[0] ? std::stoi(argv[0]) : 0;
    std::string name = argv[1] ? argv[1] : "";
    int age = argv[2] ? std::stoi(argv[2]) : 0;
    std::string department = argv[3] ? argv[3] : "";
    employees->emplace_back(id, name, age, department);
    return 0;
}

std::vector<Employee> selectAllEmployees(sqlite3* db) {
    const char* sql = "SELECT * FROM employees;";
    std::vector<Employee> employees;
    sqlite3_exec(db, sql, selectCallback, &employees, nullptr);
    return employees;
}

Employee selectEmployeeById(sqlite3* db, int id) {
    std::string sql = "SELECT * FROM employees WHERE ID = " + std::to_string(id) + ";";
    std::vector<Employee> employees;
    sqlite3_exec(db, sql.c_str(), selectCallback, &employees, nullptr);

    if (employees.empty()) {
        std::cerr << "Employee with ID " << id << " not found." << std::endl;
        return Employee(0, "", 0, ""); // Return an empty/invalid employee
    }
    return employees[0];
}

// New function to search by name
std::vector<Employee> selectEmployeesByName(sqlite3* db, const std::string& name) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM employees WHERE NAME LIKE ?;";
    std::vector<Employee> employees;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return employees;
    }

    // Bind the search term with wildcards for partial matching
    std::string searchTerm = "%" + name + "%";
    sqlite3_bind_text(stmt, 1, searchTerm.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement and build the vector
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name_text = sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);
        const unsigned char* dept_text = sqlite3_column_text(stmt, 3);

        employees.emplace_back(
            id,
            std::string(reinterpret_cast<const char*>(name_text)),
            age,
            std::string(reinterpret_cast<const char*>(dept_text))
        );
    }

    sqlite3_finalize(stmt);
    return employees;
}
