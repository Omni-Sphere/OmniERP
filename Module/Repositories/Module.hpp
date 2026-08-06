#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <Module/DTOs/Module.hpp>

namespace omnisphere::repositories
{
    class ModuleRepository
    {
        public:
        explicit ModuleRepository(std::shared_ptr<omnisphere::data::Database> Database);
        ~ModuleRepository();

        omnisphere::types::DataTable ReadAll() const;
        omnisphere::types::DataTable Read(const omnisphere::dtos::GetModule& getModule) const;

        private:
        std::shared_ptr<omnisphere::data::Database> database;
    };
} // namespace omnisphere::repositories
