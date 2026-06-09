#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Module/Module.hpp>
#include <Module/Repositories/Module.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::module
{
    struct Module::Impl
    {
        std::shared_ptr<omnisphere::repositories::ModuleRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
        : repository(std::make_shared<omnisphere::repositories::ModuleRepository>(database)) {}
    };

    Module::Module(std::shared_ptr<omnisphere::services::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
    Module::~Module() = default;

    std::vector<omnisphere::models::Module> Module::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::Module> list;
            omnisphere::types::DataTable data = pImpl->repository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                list.emplace_back(data[i]["Entry"], data[i]["Code"], data[i]["Name"],
                                              data[i]["DepartmentEntry"].GetOptional<int>(),
                                              data[i]["CreatedBy"], data[i]["CreateDate"],
                                              data[i]["LastUpdatedBy"].GetOptional<int>(),
                                              data[i]["UpdateDate"].GetOptional<std::string>());
            }

            return list;
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[GetAllModules Exception] ") + e.what()); }
    }

    omnisphere::models::Module Module::Get(const omnisphere::dtos::GetModule& getModule) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->repository->Read(getModule);

            if (data.RowsCount() == 0) throw std::runtime_error("Module doesn't exist");

            return omnisphere::models::Module(data[0]["Entry"], data[0]["Code"], data[0]["Name"],
                                                      data[0]["DepartmentEntry"].GetOptional<int>(),
                                                      data[0]["CreatedBy"], data[0]["CreateDate"],
                                                      data[0]["LastUpdatedBy"].GetOptional<int>(),
                                                      data[0]["UpdateDate"].GetOptional<std::string>());
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[GetModule Exception] ") + e.what()); }
    }
} // namespace omnisphere::module
