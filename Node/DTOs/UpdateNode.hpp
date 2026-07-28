#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <Base/BaseUpdateDTO.hpp>
#include <Node/Enums/NodeType.hpp>
#include <Node/Enums/OperationMode.hpp>
#include <optional>
#include <string>

namespace omnisphere::dtos
{
    struct UpdateNode : public omnisphere::dtos::BaseUpdateDTO
    {
        using omnisphere::dtos::BaseUpdateDTO::BaseUpdateDTO;
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<omnisphere::enums::NodeType> NodeType;
        std::optional<omnisphere::enums::OperationMode> OperationMode;
        std::optional<int> CashLimit;
        std::optional<std::string> IPAddress;
        std::optional<bool> ExtendedLog;
        std::optional<std::string> IsActive;
    };
};
