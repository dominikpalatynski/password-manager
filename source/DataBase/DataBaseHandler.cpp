#include <iostream>
#include <memory>
#include <sstream>

#include "DataBaseHandler.hpp"

DataBaseHandler::DataBaseHandler(const std::string &dbName)
    : database_(nullptr)
{
    int result = sqlite3_open(dbName.c_str(), &database_);
    if (result != SQLITE_OK)
    {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(database_)));
    }
}

DataBaseHandler::~DataBaseHandler()
{
    if (database_ != nullptr)
    {
        sqlite3_close(database_);
    }
}

void DataBaseHandler::executeQuery(const std::string &query)
{
    char *errMsg = nullptr;
    int result = sqlite3_exec(database_, query.c_str(), nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK)
    {
        throw std::runtime_error("Query execution failed: " + std::string(errMsg));
    }
}

void DataBaseHandler::createTable(const std::string &tableName, const std::string &columns)
{
    std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns + ");";
    executeQuery(query);
}

void DataBaseHandler::insertData(const std::string &tableName, const std::vector<std::string> &columns, const std::vector<std::string> &values)
{
    if (columns.size() != values.size())
    {
        throw std::invalid_argument("Columns and values sizes do not match.");
    }

    std::ostringstream queryStream;
    queryStream << "INSERT INTO " << tableName << " (";
    for (size_t i = 0; i < columns.size(); ++i)
    {
        queryStream << columns[i];
        if (i < columns.size() - 1)
        {
            queryStream << ", ";
        }
    }
    queryStream << ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i)
    {
        queryStream << "'" << values[i] << "'";
        if (i < values.size() - 1)
        {
            queryStream << ", ";
        }
    }
    queryStream << ");";

    std::string query = queryStream.str();
    executeQuery(query);
}

void DataBaseHandler::updateData(
    const std::string &tableName,
    const std::string &setRecord,
    const std::string &setValue,
    const std::string &findRecord,
    const std::string &findValue)
{
    std::string query = "UPDATE " + tableName + " SET " + setRecord + " ='" + setValue + "' WHERE " + findRecord + " = '" + findValue + "';";
    executeQuery(query);
}

void DataBaseHandler::selectData(
    const std::string &tableName,
    const std::string &findRecord,
    const std::string &findValue)
{
    std::string query = "SELECT " + findRecord + " FROM " + tableName + " WHERE " + findValue;

    executeQuery(query);
}
