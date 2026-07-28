#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <Base/BaseCreateDTO.hpp>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>

#include <boost/describe.hpp>

namespace omnisphere::dtos
{
    struct CreateItem : public omnisphere::dtos::BaseCreateDTO
    {
        using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;

        CreateItem(std::string _Code, std::string _Name,
                   std::optional<std::string> _Description,
                   std::optional<std::string> _Image, bool _PurchaseItem,
                   bool _SellItem, bool _InventoryItem, double _Price,
                   std::optional<int> _Brand, std::optional<int> _Group,
                   std::optional<double> _MinStock, std::optional<double> _MaxStock,
                   std::optional<double> _MinOrder, std::optional<double> _MaxOrder,
                   std::optional<double> _MinRequest,
                   std::optional<double> _MaxRequest, int _CreatedBy,
                   std::string _CreateDate)
            : omnisphere::dtos::BaseCreateDTO(std::move(_Code), std::move(_Name),
                                              _CreatedBy, std::move(_CreateDate)),
            Description(std::move(_Description)), Image(std::move(_Image)),
            PurchaseItem(_PurchaseItem), SellItem(_SellItem),
            InventoryItem(_InventoryItem), Price(_Price), Brand(_Brand),
            Group(_Group), MinStock(_MinStock), MaxStock(_MaxStock),
            MinOrder(_MinOrder), MaxOrder(_MaxOrder), MinRequest(_MinRequest),
            MaxRequest(_MaxRequest)
        {
            //ValidateItem();
        }

        std::optional<std::string> Description;
        std::optional<std::string> Image;
        bool PurchaseItem;
        bool SellItem;
        bool InventoryItem;
        double Price;
        std::optional<int> Brand;
        std::optional<int> Group;
        std::optional<double> MinStock;
        std::optional<double> MaxStock;
        std::optional<double> MinOrder;
        std::optional<double> MaxOrder;
        std::optional<double> MinRequest;
        std::optional<double> MaxRequest;

        void ValidateItem() const
        {
        }
    };

    BOOST_DESCRIBE_STRUCT(CreateItem, (), (Code, Name, Description, Image, PurchaseItem, SellItem, InventoryItem, Price, Brand, Group, MinStock, MaxStock, MinOrder, MaxOrder, MinRequest, MaxRequest, CreatedBy, CreateDate))
} // namespace omnisphere::dtos
