#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Department/Department.hpp>
#include <Department/Repositories/Department.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::department
{
    struct Department::Impl
    {
        std::shared_ptr<omnisphere::repositories::DepartmentRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : repository(std::make_shared<omnisphere::repositories::DepartmentRepository>(database)) {}
    };

    Department::Department(std::shared_ptr<omnisphere::services::Database> database)
        : pImpl(std::make_unique<Impl>(database)) {}
    Department::~Department() = default;

    omnisphere::models::Department Department::Add(const omnisphere::dtos::CreateDepartment& department) const
    {
        try
        {
            if (pImpl->repository->Create(department))
            {
                omnisphere::dtos::GetDepartment get;
                get.Code = department.Code;

                return Get(get);
            }
            throw std::runtime_error("Error adding department");
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[AddDepartment Exception] ") + e.what()); }
    }

    omnisphere::models::Department Department::Modify(const omnisphere::dtos::UpdateDepartment& department) const
    {
        try
        {
            if (pImpl->repository->Update(department))
            {
                omnisphere::dtos::GetDepartment get;
                get.Entry = department.Entry;

                return Get(get);
            }
            throw std::runtime_error("Error updating department");
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[ModifyDepartment Exception] ") + e.what()); }
    }

    std::vector<omnisphere::models::Department> Department::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::Department> list;
            omnisphere::types::DataTable data = pImpl->repository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                list.emplace_back(data[i]["Entry"], data[i]["Code"], data[i]["Name"],
                                  data[i]["IsActive"],
                                  data[i]["CreatedBy"], data[i]["CreateDate"],
                                  data[i]["LastUpdatedBy"].GetOptional<int>(),
                                  data[i]["UpdateDate"].GetOptional<std::string>());
            }

            return list;
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[GetAllDepartments Exception] ") + e.what()); }
    }

    omnisphere::models::Department Department::Get(const omnisphere::dtos::GetDepartment& getDepartment) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->repository->Read(getDepartment);

            if (data.RowsCount() == 0) throw std::runtime_error("Department doesn't exist");

            return omnisphere::models::Department(data[0]["Entry"], data[0]["Code"], data[0]["Name"],
                                                  data[0]["IsActive"],
                                                  data[0]["CreatedBy"], data[0]["CreateDate"],
                                                  data[0]["LastUpdatedBy"].GetOptional<int>(),
                                                  data[0]["UpdateDate"].GetOptional<std::string>());
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[GetDepartment Exception] ") + e.what()); }
    }

    bool Department::Remove(int entry) const
    {
        try

        { return pImpl->repository->Delete(entry); }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[RemoveDepartment Exception] ") + e.what()); }
    }
} // namespace omnisphere::department
