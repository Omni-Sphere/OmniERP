#pragma once

#include "BaseModel.hpp"
#include "Node/Enums/OperationMode.hpp"
#include "Node/Enums/NodeType.hpp"
#include <optional>
#include <string>

namespace omnisphere::models
{
    class Node : public omnisphere::models::BaseModel
    {
        public:
        Node(
            int _Entry,
            std::string _Code,
            std::string _Name,
            omnisphere::enums::NodeType _NodeType,
            omnisphere::enums::OperationMode _OperationMode,
            std::optional<int>  _CashLimit,
            std::optional<std::string> _IPAddress,
            bool _IsActive,
            int _CreatedBy,
            std::string _CreateDate,
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _UpdateDate
        )
        : omnisphere::models::BaseModel(
            _Entry,
            std::move(_Code),
            std::move(_Name),
            _CreatedBy,
            std::move(_CreateDate),
            _LastUpdatedBy,
            std::move(_UpdateDate)
        ),
        NodeType(_NodeType),
        OperationMode(_OperationMode),
        CashLimit(_CashLimit),
        IPAddress(_IPAddress),
        IsActive(_IsActive)
        { }

        const omnisphere::enums::NodeType NodeType;
        const omnisphere::enums::OperationMode OperationMode;
        const std::optional<int> CashLimit;
        const std::optional<std::string> IPAddress;
        const bool IsActive;
    };
}