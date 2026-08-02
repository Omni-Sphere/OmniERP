#include "Store/DTOs/SearchStore.hpp"
#include <Database.hpp>
#include <DataTable.hpp>
#include <DataMapper.hpp>
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

    std::optional<omnisphere::models::Store> Store::Get(const omnisphere::dtos::GetStore &filter, const std::vector<std::string>& fields) const
    {
        if (!filter.Entry.has_value() && !filter.Name.has_value() && !filter.Code.has_value())
            throw std::invalid_argument("Filter (Entry, Code, or Name) cannot be empty");

        omnisphere::types::DataTable dataTable = pimpl->repository.Read(filter, fields);

        if (dataTable.IsEmpty())
            return std::nullopt;

        return omnisphere::data::MapFromRow<omnisphere::models::Store>(dataTable[0]);
    }

    std::vector<omnisphere::models::Store> Store::GetAll(const std::vector<std::string>& fields) const
    {
        omnisphere::types::DataTable dataTable = pimpl->repository.ReadAll(fields);
        std::vector<omnisphere::models::Store> results;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            results.emplace_back(omnisphere::data::MapFromRow<omnisphere::models::Store>(dataTable[i]));
        }

        return results;
    }

    std::vector<omnisphere::models::Store> Store::Search(const std::vector<std::string>& fields, const omnisphere::dtos::SearchStore &filter) const
    {        
        omnisphere::types::DataTable dataTable = pimpl->repository.Search(fields, filter);
        std::vector<omnisphere::models::Store> results;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            results.emplace_back(omnisphere::data::MapFromRow<omnisphere::models::Store>(dataTable[i]));
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
