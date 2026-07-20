#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <optional>
#include <string>
#include <memory>
#include <boost/describe.hpp>
#include "User/Models/User.hpp"

namespace omnisphere::models
{
    class ItemBrand
    {
        public:
        int Entry;
        std::string Code;
        std::string Name;
        bool IsActive = true;
        int CreatedBy;
        std::string CreateDate;
        std::optional<int> LastUpdatedBy;
        std::optional<std::string> UpdateDate;

        std::shared_ptr<omnisphere::models::User> CreatedByUser;
        std::shared_ptr<omnisphere::models::User> LastUpdatedByUser;
    };

    BOOST_DESCRIBE_STRUCT(ItemBrand, (), (Entry, Code, Name, IsActive, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate))
} // namespace omnisphere::models