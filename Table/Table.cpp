#include "Table.hpp"
#include "DataTable.hpp"
#include "Repositories/Table.hpp"
#include <stdexcept>
#include <string>

namespace omnisphere::table {
Table::Table(std::shared_ptr<omnisphere::services::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Table::~Table() = default;

struct Table::Impl {
  std::shared_ptr<omnisphere::repositories::TableRepository> tableRepository;
  explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
      : tableRepository(
            std::make_shared<omnisphere::repositories::TableRepository>(
                database)) {}
};

omnisphere::models::Table
Table::Add(const omnisphere::dtos::CreateTable &table) const {
  try {
    if (pImpl->tableRepository->Create(table)) {
      omnisphere::dtos::GetTable getTable;
      getTable.Code = table.Code;
      return Get(getTable);
    } else {
      throw std::runtime_error("Error adding table");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddTable Exception] ") + e.what());
  }
}

omnisphere::models::Table
Table::Modify(const omnisphere::dtos::UpdateTable &table) const {
  try {
    if (pImpl->tableRepository->Update(table)) {
      omnisphere::dtos::GetTable getTable;
      getTable.Entry = table.Entry;
      return Get(getTable);
    } else {
      throw std::runtime_error("Error updating table");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[ModifyTable Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::models::Table> Table::GetAll() const {
  try {
    std::vector<omnisphere::models::Table> tables;
    omnisphere::types::DataTable data = pImpl->tableRepository->ReadAll();

    for (int i = 0; i < data.RowsCount(); i++) {
        tables.emplace_back(data[i]["Entry"], 
                            data[i]["Code"], 
                            data[i]["Name"],
                            data[i]["Capacity"],
                            data[i]["Type"],
                            data[i]["AreaEntry"],
                            data[i]["FloorEntry"],
                            data[i]["CreatedBy"], 
                            data[i]["CreateDate"],
                            data[i]["LastUpdatedBy"].GetOptional<int>(), 
                            data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return tables;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllTables Exception] ") +
                             e.what());
  }
}

omnisphere::models::Table
Table::Get(const omnisphere::dtos::GetTable &getTable) const {
  try {
    omnisphere::types::DataTable data = pImpl->tableRepository->Read(getTable);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Table doesn't exists");

    return omnisphere::models::Table(data[0]["Entry"], 
                                     data[0]["Code"], 
                                     data[0]["Name"],
                                     data[0]["Capacity"],
                                     data[0]["Type"],
                                     data[0]["AreaEntry"],
                                     data[0]["FloorEntry"],
                                     data[0]["CreatedBy"], 
                                     data[0]["CreateDate"],
                                     data[0]["LastUpdatedBy"].GetOptional<int>(), 
                                     data[0]["UpdateDate"].GetOptional<std::string>());
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetTable Exception] ") + e.what());
  }
}

std::vector<omnisphere::models::Table> Table::Search(const omnisphere::dtos::GetTable &getTable) const {
  try {
    std::vector<omnisphere::models::Table> tables;
    omnisphere::types::DataTable data = pImpl->tableRepository->Read(getTable);

    for (int i = 0; i < data.RowsCount(); i++) {
        tables.emplace_back(data[i]["Entry"], 
                            data[i]["Code"], 
                            data[i]["Name"],
                            data[i]["Capacity"],
                            data[i]["Type"],
                            data[i]["AreaEntry"],
                            data[i]["FloorEntry"],
                            data[i]["CreatedBy"], 
                            data[i]["CreateDate"],
                            data[i]["LastUpdatedBy"].GetOptional<int>(), 
                            data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return tables;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[SearchTables Exception] ") +
                             e.what());
  }
}
} // namespace omnisphere::table
