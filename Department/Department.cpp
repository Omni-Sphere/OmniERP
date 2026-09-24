#include "Department/Department.hpp"
#include "Department/Repositories/Department.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <OmniData/DataMapper.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::department {
struct Department::Impl {
  std::shared_ptr<omnisphere::repositories::DepartmentRepository> repository;
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
      : repository(
            std::make_shared<omnisphere::repositories::DepartmentRepository>(
                database)) {}
};

Department::Department(std::shared_ptr<omnisphere::data::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Department::~Department() = default;

omnisphere::models::Department
Department::Add(const omnisphere::dtos::CreateDepartment &department) const {
  try {
    if (pImpl->repository->Create(department)) {
      omnisphere::dtos::GetDepartment get;
      get.Code = department.Code;

      return Get(get);
    }
    throw std::runtime_error("Error adding department");
  } catch (const std::exception &e)

  {
    throw std::runtime_error(std::string("[AddDepartment Exception] ") +
                             e.what());
  }
}

omnisphere::models::Department
Department::Modify(const omnisphere::dtos::UpdateDepartment &department) const {
  try {
    if (pImpl->repository->Update(department)) {
      omnisphere::dtos::GetDepartment get;
      get.Entry = department.Entry;

      return Get(get);
    }
    throw std::runtime_error("Error updating department");
  } catch (const std::exception &e)

  {
    throw std::runtime_error(std::string("[ModifyDepartment Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::models::Department> Department::GetAll(const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->repository->ReadAll(fields);
    return omnisphere::types::DataTableToModels<omnisphere::models::Department>(data);
  } catch (const std::exception &e)
  {
    throw std::runtime_error(std::string("[GetAllDepartments Exception] ") +
                             e.what());
  }
}

omnisphere::models::Department
Department::Get(const omnisphere::dtos::GetDepartment &getDepartment, const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->repository->Read(getDepartment, fields);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Department doesn't exist");

    return omnisphere::types::FromDataRow<omnisphere::models::Department>(data[0]);
  } catch (const std::exception &e)
  {
    throw std::runtime_error(std::string("[GetDepartment Exception] ") +
                             e.what());
  }
}

bool Department::Remove(int entry) const {
  try

  {
    return pImpl->repository->Delete(entry);
  } catch (const std::exception &e)

  {
    throw std::runtime_error(std::string("[RemoveDepartment Exception] ") +
                             e.what());
  }
}
} // namespace omnisphere::department
