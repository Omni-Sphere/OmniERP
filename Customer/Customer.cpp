#include <Database.hpp>
#include <DataTable.hpp>
#include <Customer/Customer.hpp>
#include <Customer/Repositories/Customer.hpp>

namespace omnisphere::services
{
    struct Customer::Impl
    {
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : repository(database) {}

        omnisphere::repositories::Customer repository;
    };

    Customer::Customer(std::shared_ptr<omnisphere::services::Database> database)
        : pimpl(std::make_unique<Impl>(database)) {}

    Customer::~Customer() = default;

    static omnisphere::models::Customer MapToModel(omnisphere::types::DataTable::Row& row)
    {
        return omnisphere::models::Customer(
            row["Entry"],
            row["Code"],
            row["Name"],
            row["FirstName"],
            row["MiddleName"].GetOptional<std::string>(),
            row["LastName"],
            row["SecondLastName"].GetOptional<std::string>(),
            row["TaxID"].GetOptional<std::string>(),
            row["Email"].GetOptional<std::string>(),
            row["Phone"].GetOptional<std::string>(),
            row["PaymentTerms"],
            row["MaxDiscount"],
            row["CreditLimit"].GetOptional<double>(),
            row["IsActive"],
            row["CreatedBy"],
            row["CreateDate"],
            row["LastUpdatedBy"].GetOptional<int>(),
            row["UpdateDate"].GetOptional<std::string>()
        );
    }

    std::optional<omnisphere::models::Customer> Customer::Get(int entry) const
    {
        omnisphere::types::DataTable dataTable = pimpl->repository.Read(entry);

        if (dataTable.IsEmpty())
        {
            return std::nullopt;
        }

        return MapToModel(dataTable[0]);
    }

    std::vector<omnisphere::models::Customer> Customer::GetAll() const
    {
        omnisphere::types::DataTable dataTable = pimpl->repository.ReadAll();
        std::vector<omnisphere::models::Customer> results;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            results.emplace_back(MapToModel(dataTable[i]));
        }

        return results;
    }

    bool Customer::Add(const omnisphere::dtos::CreateCustomer &_customer) const
    {
        return pimpl->repository.Create(_customer);
    }

    bool Customer::Modify(const omnisphere::dtos::UpdateCustomer &_customer) const
    {
        return pimpl->repository.Update(_customer);
    }

} // namespace omnisphere::services
