#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>
#include <Base/BaseModel.hpp>

namespace omnisphere::models {
class Permission : public BaseModel {
public:
    Permission(int _Entry, std::string _Code, std::string _Name, int _ModuleEntry,
               int _CreatedBy, std::string _CreateDate, std::optional<int> _LastUpdatedBy,
               std::optional<std::string> _UpdateDate)
        : BaseModel(_Entry, std::move(_Code), std::move(_Name), _CreatedBy, std::move(_CreateDate), _LastUpdatedBy, std::move(_UpdateDate)),
          ModuleEntry(_ModuleEntry) {}

    int ModuleEntry;
};
} // namespace omnisphere::models
