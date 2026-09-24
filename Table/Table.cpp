#include "Table/Repositories/Table.hpp"
#include "Table/Table.hpp"
#include <OmniData/DataMapper.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::table {
Table::Table(std::shared_ptr<omnisphere::data::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Table::~Table() = default;

struct Table::Impl {
  std::shared_ptr<omnisphere::repositories::TableRepository> tableRepository;
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
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

std::vector<omnisphere::models::Table> Table::GetAll(const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->tableRepository->ReadAll(fields);
    return omnisphere::types::DataTableToModels<omnisphere::models::Table>(data);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllTables Exception] ") +
                             e.what());
  }
}

omnisphere::models::Table
Table::Get(const omnisphere::dtos::GetTable &getTable, const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->tableRepository->Read(getTable, fields);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Table doesn't exists");

    return omnisphere::types::FromDataRow<omnisphere::models::Table>(data[0]);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetTable Exception] ") + e.what());
  }
}

std::vector<omnisphere::models::Table>
Table::Search(const omnisphere::dtos::GetTable &getTable, const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->tableRepository->Read(getTable, fields);
    return omnisphere::types::DataTableToModels<omnisphere::models::Table>(data);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[SearchTables Exception] ") +
                             e.what());
  }
}

bool Table::Remove(int entry) const {
  try {
    return pImpl->tableRepository->Delete(entry);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[RemoveTable Exception] ") +
                             e.what());
  }
}
} // namespace omnisphere::table
