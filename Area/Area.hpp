#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <vector>

#include <Area/DTOs/CreateArea.hpp>
#include <Area/DTOs/GetArea.hpp>
#include <Area/DTOs/UpdateArea.hpp>
#include <Area/Models/Area.hpp>

#include <Database.hpp>

namespace omnisphere::area
{
    class Area
    {
        struct Impl;
        std::unique_ptr<Impl> pImpl;

        public:
        explicit Area(std::shared_ptr<omnisphere::services::Database> database);

        ~Area();
        omnisphere::models::Area Add(const omnisphere::dtos::CreateArea &area) const;
        omnisphere::models::Area Modify(const omnisphere::dtos::UpdateArea &area) const;
        std::vector<omnisphere::models::Area> GetAll() const;
        omnisphere::models::Area Get(const omnisphere::dtos::GetArea &getArea) const;
        std::vector<omnisphere::models::Area> Search(const omnisphere::dtos::GetArea &getArea) const;
        bool Remove(int entry) const;
    };
} // namespace omnisphere::area
