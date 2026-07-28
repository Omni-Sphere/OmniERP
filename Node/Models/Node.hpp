#pragma once

#include "Node/Enums/OperationMode.hpp"
#include "Node/Enums/NodeType.hpp"
#include <optional>
#include <string>
#include <memory>
#include <boost/describe.hpp>
#include "User/Models/User.hpp"

namespace omnisphere::models
{
    class Node
    {
        public:
        Node() = default;

        Node(
            int _Entry,
            std::string _Code,
            std::string _Name,
            omnisphere::enums::NodeType _NodeType,
            omnisphere::enums::OperationMode _OperationMode,
            std::optional<int> _CashLimit,
            std::optional<std::string> _IPAddress,
            bool _ExtendedLog,
            bool _IsActive,
            int _CreatedBy,
            std::string _CreateDate,
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _UpdateDate
        )
        : Entry(_Entry),
          Code(std::move(_Code)),
          Name(std::move(_Name)),
          NodeType(_NodeType),
          OperationMode(_OperationMode),
          CashLimit(_CashLimit),
          IPAddress(std::move(_IPAddress)),
          ExtendedLog(_ExtendedLog),
          IsActive(_IsActive),
          CreatedBy(_CreatedBy),
          CreateDate(std::move(_CreateDate)),
          LastUpdatedBy(_LastUpdatedBy),
          UpdateDate(std::move(_UpdateDate))
        { }

        int Entry = 0;
        std::string Code = "";
        std::string Name = "";
        omnisphere::enums::NodeType NodeType = omnisphere::enums::NodeType::Cashier;
        omnisphere::enums::OperationMode OperationMode = omnisphere::enums::OperationMode::POS;
        std::optional<int> CashLimit;
        std::optional<std::string> IPAddress;
        bool ExtendedLog = false;
        bool IsActive = true;

        int CreatedBy = 0;
        std::string CreateDate = "";
        std::optional<int> LastUpdatedBy;
        std::optional<std::string> UpdateDate;

        std::shared_ptr<omnisphere::models::User> CreatedByUser;
        std::shared_ptr<omnisphere::models::User> LastUpdatedByUser;
    };

    BOOST_DESCRIBE_STRUCT(Node, (), (Entry, Code, Name, NodeType, OperationMode, CashLimit, IPAddress, ExtendedLog, IsActive, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate))
}