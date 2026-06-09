#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Employee/Employee.hpp>
#include <Employee/Repositories/Employee.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::employee
{
    struct Employee::Impl
    {
        std::shared_ptr<omnisphere::repositories::EmployeeRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
        : repository(std::make_shared<omnisphere::repositories::EmployeeRepository>(database)) {}
    };

    Employee::Employee(std::shared_ptr<omnisphere::services::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
    Employee::~Employee() = default;

    omnisphere::models::Employee Employee::Add(const omnisphere::dtos::CreateEmployee& employee) const
    {
        try
        {
            if (pImpl->repository->Create(employee))
            {
                omnisphere::dtos::GetEmployee get;
                get.Code = employee.Code;

                return Get(get);
            }
            throw std::runtime_error("Error adding employee");
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[AddEmployee Exception] ") + e.what()); }
    }

    omnisphere::models::Employee Employee::Modify(const omnisphere::dtos::UpdateEmployee& employee) const
    {
        try
        {
            if (pImpl->repository->Update(employee))
            {
                omnisphere::dtos::GetEmployee get;
                get.Entry = employee.Entry;

                return Get(get);
            }
            throw std::runtime_error("Error updating employee");
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[ModifyEmployee Exception] ") + e.what()); }
    }

    std::vector<omnisphere::models::Employee> Employee::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::Employee> list;
            omnisphere::types::DataTable data = pImpl->repository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                list.emplace_back(data[i]["Entry"], data[i]["Code"], data[i]["Name"],
                                              data[i]["FirstName"], data[i]["SecondName"].GetOptional<std::string>(),
                                              data[i]["LastName"], data[i]["SecondLastName"].GetOptional<std::string>(),
                                              data[i]["Phone"].GetOptional<std::string>(), data[i]["DateOfBirth"].GetOptional<std::string>(),
                                              data[i]["PlaceOfBirth"].GetOptional<std::string>(), data[i]["Comments"].GetOptional<std::string>(),
                                              data[i]["IsActive"],
                                              data[i]["CreatedBy"], data[i]["CreateDate"],
                                              data[i]["LastUpdatedBy"].GetOptional<int>(),
                                              data[i]["UpdateDate"].GetOptional<std::string>());
            }

            return list;
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[GetAllEmployees Exception] ") + e.what()); }
    }

    omnisphere::models::Employee Employee::Get(const omnisphere::dtos::GetEmployee& getEmployee) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->repository->Read(getEmployee);

            if (data.RowsCount() == 0) throw std::runtime_error("Employee doesn't exist");

            return omnisphere::models::Employee(data[0]["Entry"], data[0]["Code"], data[0]["Name"],
                                                        data[0]["FirstName"], data[0]["SecondName"].GetOptional<std::string>(),
                                                        data[0]["LastName"], data[0]["SecondLastName"].GetOptional<std::string>(),
                                                        data[0]["Phone"].GetOptional<std::string>(), data[0]["DateOfBirth"].GetOptional<std::string>(),
                                                        data[0]["PlaceOfBirth"].GetOptional<std::string>(), data[0]["Comments"].GetOptional<std::string>(),
                                                        data[0]["IsActive"],
                                                        data[0]["CreatedBy"], data[0]["CreateDate"],
                                                        data[0]["LastUpdatedBy"].GetOptional<int>(),
                                                        data[0]["UpdateDate"].GetOptional<std::string>());
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[GetEmployee Exception] ") + e.what()); }
    }

    bool Employee::Remove(int entry) const
    {
        try
        { return pImpl->repository->Delete(entry); }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[RemoveEmployee Exception] ") + e.what()); }
    }
} // namespace omnisphere::employee
