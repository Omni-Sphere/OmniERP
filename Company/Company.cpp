#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <Company/Company.hpp>
#include <Company/Repositories/Company.hpp>
#include <DataTable.hpp>

namespace omnisphere::services
{
    struct Company::Impl
    {
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : repository(database) {}

        omnisphere::repositories::Company repository;
    };

    Company::Company(std::shared_ptr<omnisphere::services::Database> database)
        : pimpl(std::make_unique<Impl>(database)) {}

    Company::~Company() = default;

    omnisphere::models::Company Company::Get(const omnisphere::dtos::GetCompany &_company) const
    {
        omnisphere::types::DataTable dataTable = pimpl->repository.Read(_company);

        if (dataTable.IsEmpty())
        {
            throw std::runtime_error("Business Configuration not found.");
        }

        return omnisphere::models::Company(
            dataTable[0]["Entry"],
            dataTable[0]["Code"],
            dataTable[0]["Name"],
            dataTable[0]["CommercialName"].GetOptional<std::string>(),
            dataTable[0]["Address"].GetOptional<std::string>(),
            dataTable[0]["Address2"].GetOptional<std::string>(),
            dataTable[0]["City"].GetOptional<int>(),
            dataTable[0]["State"].GetOptional<int>(),
            dataTable[0]["ZipCode"].GetOptional<int>(),
            dataTable[0]["Country"].GetOptional<int>(),
            dataTable[0]["TaxID"].GetOptional<std::string>(),
            dataTable[0]["Currency"],
            dataTable[0]["Phone1"].GetOptional<std::string>(),
            dataTable[0]["Phone2"].GetOptional<std::string>(),
            dataTable[0]["Email"].GetOptional<std::string>(),
            dataTable[0]["WebSite"].GetOptional<std::string>(),
            dataTable[0]["FacebookProfile"].GetOptional<std::string>(),
            dataTable[0]["InstagramProfile"].GetOptional<std::string>(),
            dataTable[0]["XProfile"].GetOptional<std::string>(),
            dataTable[0]["LogoFile"].GetOptional<std::string>(),
            dataTable[0]["AttachmentsPath"].GetOptional<std::string>(),
            dataTable[0]["ImagePath"].GetOptional<std::string>(),
            dataTable[0]["IsActive"].IsNull() ? "Y" : std::string(dataTable[0]["IsActive"]),
            1, // Placeholder for CreatedBy
            "2024-01-01", // Placeholder for CreateDate
            std::nullopt,
            std::nullopt
        );
    }

    bool Company::Add(const omnisphere::dtos::CreateCompany &_company) const
    {
        return pimpl->repository.Create(_company);
    }

    bool Company::Modify(const omnisphere::dtos::UpdateCompany &_company) const
    {
        return pimpl->repository.Update(_company);
    }

} // namespace omnisphere::services
