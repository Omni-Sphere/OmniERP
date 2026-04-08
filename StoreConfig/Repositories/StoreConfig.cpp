#include "StoreConfig.hpp"
#include "SQLParams.hpp"

namespace omnisphere::repositories 
{
    StoreConfig::StoreConfig(std::shared_ptr<omnisphere::services::Database> database) : Database(database)
    {}

    StoreConfig::~StoreConfig() = default;

    template <typename T>
    void StoreConfig::AddInsertParam(const std::string &field, const T &value, 
                                     std::vector<std::string> &insertClauses, 
                                     std::vector<omnisphere::types::SQLParam> &params) const
    {
        using U = std::decay_t<T>;

        if constexpr (omnisphere::types::is_optional<U>::value)
        {
            if (value.has_value())
            {
                insertClauses.push_back(field);
                params.push_back(omnisphere::types::MakeSQLParam(value.value()));
            }
        }
        else
        {
            insertClauses.push_back(field);
            params.push_back(omnisphere::types::MakeSQLParam(value));
        }
    }

    template <typename T>
    void StoreConfig::AddSetParam(const std::string &field, const T &value, 
                                  std::vector<std::string> &setClauses, 
                                  std::vector<omnisphere::types::SQLParam> &params) const
    {
        using U = std::decay_t<T>;

        if constexpr (omnisphere::types::is_optional<U>::value)
        {
            if (value.has_value())
            {
                setClauses.push_back(field + " = ?");
                params.push_back(omnisphere::types::MakeSQLParam(value.value()));
            }
        }
        else
        {
            setClauses.push_back(field + " = ?");
            params.push_back(omnisphere::types::MakeSQLParam(value));
        }
    }

    bool StoreConfig::Create(const omnisphere::dtos::CreateStoreConfig &_storeConfig) const 
    {
        try 
        {
            std::string sQuery = "INSERT INTO StoresConfig (Entry, Code, Name, Address) VALUES (?, ?, ?, ?)";
            std::vector<omnisphere::types::SQLParam> params = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(_storeConfig.Code),
                omnisphere::types::MakeSQLParam(_storeConfig.Name),
                omnisphere::types::MakeSQLParam(_storeConfig.Address)
            };

            if (!Database->RunPrepared(sQuery, params))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateStoreSequence())
                throw std::runtime_error("[UpdateStoreSequence exception]");

            Database->CommitTransaction();

            return true;
        } 
        catch (const std::exception &e) 
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[CreateStoreConfig Exception] ") + e.what());
        }
    }

    bool StoreConfig::Update(const omnisphere::dtos::UpdateStoreConfig &_storeConfig) const 
    {
        try 
        {
            std::string sQuery = "UPDATE StoresConfig SET ";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> setClauses;

            AddSetParam("Code", _storeConfig.Code, setClauses, params);
            AddSetParam("Name", _storeConfig.Name, setClauses, params);
            AddSetParam("Address", _storeConfig.Address, setClauses, params);

            if (setClauses.empty()) return true;

            for (size_t i = 0; i < setClauses.size(); ++i) 
            {
                sQuery += setClauses[i];
                if (i < setClauses.size() - 1)
                    sQuery += ", ";
            }

            sQuery += " WHERE Entry = ?";
            params.push_back(omnisphere::types::MakeSQLParam(_storeConfig.Entry));

            if (!Database->RunPrepared(sQuery, params))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        } 
        catch (const std::exception &e) 
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateStoreConfig Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable StoreConfig::Read(const omnisphere::dtos::GetStoreConfig &_storeConfig) const 
    {
        try 
        {
            std::string sQuery = "SELECT * FROM StoresConfig WHERE 1 = 1";
            std::vector<omnisphere::types::SQLParam> params;

            if (_storeConfig.Entry.has_value())
            {
                sQuery += " AND Entry = ?";
                params.push_back(omnisphere::types::MakeSQLParam(_storeConfig.Entry.value()));
            }

            if (_storeConfig.Code.has_value())
            {
                sQuery += " AND Code = ?";
                params.push_back(omnisphere::types::MakeSQLParam(_storeConfig.Code.value()));
            }

            if (_storeConfig.Name.has_value())
            {
                sQuery += " AND Name = ?";
                params.push_back(omnisphere::types::MakeSQLParam(_storeConfig.Name.value()));
            }

            return Database->FetchPrepared(sQuery, params);
        } 
        catch (const std::exception &e) 
        {
            throw std::runtime_error(std::string("[ReadStoreConfig Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable StoreConfig::Read(const omnisphere::dtos::SearchStoreConfig &_storeConfig) const 
    {
        try 
        {
            std::string sQuery = "SELECT * FROM StoresConfig WHERE 1 = 1";
            std::vector<omnisphere::types::SQLParam> params;

            if (_storeConfig.Code.has_value())
            {
                sQuery += " AND Code LIKE ?";
                params.push_back(omnisphere::types::MakeSQLParam("%" + _storeConfig.Code.value() + "%"));
            }

            if (_storeConfig.Name.has_value())
            {
                sQuery += " AND Name LIKE ?";
                params.push_back(omnisphere::types::MakeSQLParam("%" + _storeConfig.Name.value() + "%"));
            }

            return Database->FetchPrepared(sQuery, params);
        } 
        catch (const std::exception &e) 
        {
            throw std::runtime_error(std::string("[SearchStoreConfig Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable StoreConfig::ReadAll() const 
    {
        try 
        {
            return Database->FetchResults("SELECT * FROM StoresConfig");
        } 
        catch (const std::exception &e) 
        {
            throw std::runtime_error(std::string("[ReadAllStoreConfig Exception] ") + e.what());
        }
    }

    int StoreConfig::GetCurrentSequence() const 
    {
        try 
        {
            const std::string query = "SELECT ISNULL(StoreSequence, 0) + 1 StoreSequence FROM Sequences WHERE SeqEntry = 1";
            omnisphere::types::DataTable dataTable = Database->FetchResults(query);
            return dataTable[0]["StoreSequence"];
        } 
        catch (const std::exception &e) 
        {
            throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what());
        }
    }

    bool StoreConfig::UpdateStoreSequence() const 
    {
        try 
        {
            const std::string query = "UPDATE Sequences SET StoreSequence = ISNULL(StoreSequence, 0) + 1";
            if (!Database->RunStatement(query))
                throw std::runtime_error("[RunStatement exception]");
            return true;
        } 
        catch (const std::exception &e) 
        {
            throw std::runtime_error(std::string("[UpdateStoreSequence Exception] ") + e.what());
        }
    }
}
