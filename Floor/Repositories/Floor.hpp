#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DTOs/CreateFloor.hpp"
#include "DTOs/UpdateFloor.hpp"
#include "DTOs/GetFloor.hpp"
#include "Database.hpp"

namespace omnisphere::repositories {
    class FloorRepository {
    private:
        std::shared_ptr<omnisphere::services::Database> database;
        int GetCurrentSequence() const;
        bool UpdateFloorSequence() const;
    public:
        explicit FloorRepository(std::shared_ptr<omnisphere::services::Database> Database);
        ~FloorRepository();
        bool Create(const omnisphere::dtos::CreateFloor &floor) const;
        bool Update(const omnisphere::dtos::UpdateFloor &floor) const;
        omnisphere::types::DataTable ReadAll() const;
        omnisphere::types::DataTable Read(const omnisphere::dtos::GetFloor &getFloor) const;
    };
};
