#include <Database.hpp>
#include <DataTable.hpp>
#include <Store/Store.hpp>
#include <Store/Repositories/Store.hpp>

namespace omnisphere::services
{
    struct Store::Impl
    {
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : repository(database) {}

        omnisphere::repositories::Store repository;
    };

    Store::Store(std::shared_ptr<omnisphere::services::Database> database)
        : pimpl(std::make_unique<Impl>(database)) {}

    Store::~Store() = default;

    static omnisphere::models::Store MapToModel(omnisphere::types::DataTable::Row& row)
    {
        return omnisphere::models::Store(
            row["Entry"],
            row["Code"],
            row["Name"],
            row["GuestCustomer"],
            row["Address"].GetOptional<std::string>(),
            row["Address2"].GetOptional<std::string>(),
            row["City"].GetOptional<int>(),
            row["State"].GetOptional<int>(),
            row["ZipCode"].GetOptional<int>(),
            row["Country"].GetOptional<int>(),
            row["Currency"],
            row["Phone1"].GetOptional<std::string>(),
            row["Phone2"].GetOptional<std::string>(),
            row["Email"].GetOptional<std::string>(),
            row["Website"].GetOptional<std::string>(),
            row["Facebook"].GetOptional<std::string>(),
            row["Instagram"].GetOptional<std::string>(),
            row["X"].GetOptional<std::string>(),
            row["Whatsapp"].GetOptional<std::string>(),
            row["IsActive"],
            row["CreatedBy"],
            row["CreateDate"],
            row["LastUpdatedBy"].GetOptional<int>(),
            row["UpdateDate"].GetOptional<std::string>()
        );
    }

    std::optional<omnisphere::models::Store> Store::Get(int entry) const
    {
        omnisphere::types::DataTable dataTable = pimpl->repository.Read(entry);

        if (dataTable.IsEmpty())
        {
            return std::nullopt;
        }

        return MapToModel(dataTable[0]);
    }

    std::vector<omnisphere::models::Store> Store::GetAll() const
    {
        omnisphere::types::DataTable dataTable = pimpl->repository.ReadAll();
        std::vector<omnisphere::models::Store> results;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            results.emplace_back(MapToModel(dataTable[i]));
        }

        return results;
    }

    bool Store::Add(const omnisphere::dtos::CreateStore &_store) const
    {
        return pimpl->repository.Create(_store);
    }

    bool Store::Modify(const omnisphere::dtos::UpdateStore &_store) const
    {
        return pimpl->repository.Update(_store);
    }

} // namespace omnisphere::services
