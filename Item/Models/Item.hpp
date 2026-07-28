#pragma once
#include <optional>
#include <string>
#include <regex>
#include <stdexcept>
#include <memory>
#include <boost/describe.hpp>
#include "User/Models/User.hpp"
#include "ItemBrand/Models/ItemBrand.hpp"
#include "ItemGroup/Models/ItemGroup.hpp"

namespace omnisphere::models
{
    class Item
    {
        public:
        Item() = default;

        Item(
            int _Entry,
            std::string _Code,
            std::string _Name,
            std::optional<std::string> _Description,
            std::optional<std::string> _Image,
            bool _IsActive,
            bool _PurchaseItem,
            bool _SellItem,
            bool _InventoryItem,
            double _Price,
            std::optional<int> _Brand,
            std::optional<int> _Group,
            double _OnHand,
            std::optional<double> _OnOrder,
            std::optional<double> _OnRequest,
            std::optional<double> _MinStock,
            std::optional<double> _MaxStock,
            std::optional<double> _MinOrder,
            std::optional<double> _MaxOrder,
            std::optional<double> _MinRequest,
            std::optional<double> _MaxRequest,
            int _CreatedBy,
            std::string _CreateDate,
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _LastUpdateDate
        )
            : Entry(_Entry),
              Code(std::move(_Code)),
              Name(std::move(_Name)),
              Description(std::move(_Description)),
              Image(std::move(_Image)),
              IsActive(_IsActive),
              PurchaseItem(_PurchaseItem),
              SellItem(_SellItem),
              InventoryItem(_InventoryItem),
              Price(_Price),
              Brand(_Brand),
              Group(_Group),
              OnHand(_OnHand),
              OnOrder(_OnOrder),
              OnRequest(_OnRequest),
              MinStock(_MinStock),
              MaxStock(_MaxStock),
              MinOrder(_MinOrder),
              MaxOrder(_MaxOrder),
              MinRequest(_MinRequest),
              MaxRequest(_MaxRequest),
              CreatedBy(_CreatedBy),
              CreateDate(std::move(_CreateDate)),
              LastUpdatedBy(_LastUpdatedBy),
              UpdateDate(std::move(_LastUpdateDate))
        {
            // Manual validation should be called after population
        }

        // BaseModel properties
        int Entry = 0;
        std::string Code = "";
        std::string Name = "";
        int CreatedBy = 0;
        std::string CreateDate = "";
        std::optional<int> LastUpdatedBy;
        std::optional<std::string> UpdateDate;
        std::shared_ptr<omnisphere::models::User> CreatedByUser;
        std::shared_ptr<omnisphere::models::User> LastUpdatedByUser;

        // Item properties
        std::optional<std::string> Description;
        std::optional<std::string> Image;
        bool IsActive = false;
        bool PurchaseItem = false;
        bool SellItem = false;
        bool InventoryItem = false;
        double Price = 0.0;
        std::optional<int> Brand;
        std::optional<int> Group;
        std::shared_ptr<omnisphere::models::ItemBrand> BrandObj;
        std::shared_ptr<omnisphere::models::ItemGroup> GroupObj;
        double OnHand = 0.0;
        std::optional<double> OnOrder;
        std::optional<double> OnRequest;
        std::optional<double> MinStock;
        std::optional<double> MaxStock;
        std::optional<double> MinOrder;
        std::optional<double> MaxOrder;
        std::optional<double> MinRequest;
        std::optional<double> MaxRequest;



        void ValidateItem() const
        {
            // Validations disabled as requested
        }

        private:
        void ValidateOptionalRange(const std::optional<double> &val,
                                   const char *field) const
        {
            if (val.has_value() && val.value() < 0)
                throw std::runtime_error(std::string(field) + "' must be >= 0.");
        }

        void ValidateMinMaxPair(const std::optional<double> &minV,
                                const std::optional<double> &maxV,
                                const char *minName, const char *maxName) const
        {
            if (minV.has_value() && maxV.has_value())
            {
                if (minV.value() > maxV.value())
                {
                    throw std::runtime_error(std::string("") + minName +
                                             " cannot be greater than " + maxName);
                }
            }
        }

        const std::regex alphaNumRegex{"^[A-Za-z0-9]+$"};
        const std::regex descriptionLengthRegex{R"(^.{3,200}$)"};
        const std::regex descriptionValidCharsRegex{
            R"(^[A-Za-z0-9\s\.,;:!¡¿\?\-_()]*$)"};
    };

    BOOST_DESCRIBE_STRUCT(Item, (), (Entry, Code, Name, Description, Image, IsActive, PurchaseItem, SellItem, InventoryItem, Price, Brand, Group, OnHand, OnOrder, OnRequest, MinStock, MaxStock, MinOrder, MaxOrder, MinRequest, MaxRequest, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate))

} // namespace omnisphere::models