#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <Module/Models/Module.hpp>
#include <Module/DTOs/Module.hpp>

namespace omnisphere::module
{
    class Module
    {
        public:
        explicit Module(std::shared_ptr<omnisphere::services::Database> database);
        ~Module();

        std::vector<omnisphere::models::Module> GetAll() const;
        omnisphere::models::Module Get(const omnisphere::dtos::GetModule& getModule) const;

        private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };
} // namespace omnisphere::module
