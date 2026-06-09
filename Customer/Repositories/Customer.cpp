#include <Database.hpp>
#include <DataTable.hpp>
#include <Customer/Repositories/Customer.hpp>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    Customer::Customer(std::shared_ptr<omnisphere::services::Database> database) : Database(database) {}

    Customer::~Customer() = default;

    template <typename T>
    void Customer::AddInsertParam(const std::string &field, const T &value,
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
    void Customer::AddSetParam(const std::string &field, const T &value,
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

    bool Customer::Create(const omnisphere::dtos::CreateCustomer &_customer) const
    {
        try
        {
            std::vector<std::string> fields;
            std::vector<omnisphere::types::SQLParam> params;

            AddInsertParam("Entry", GetCurrentSequence(), fields, params);
            AddInsertParam("Code", _customer.Code, fields, params);
            AddInsertParam("Name", _customer.Name, fields, params);
            AddInsertParam("FirstName", _customer.FirstName, fields, params);
            AddInsertParam("MiddleName", _customer.MiddleName, fields, params);
            AddInsertParam("LastName", _customer.LastName, fields, params);
            AddInsertParam("SecondLastName", _customer.SecondLastName, fields, params);
            AddInsertParam("TaxID", _customer.TaxID, fields, params);
            AddInsertParam("Email", _customer.Email, fields, params);
            AddInsertParam("Phone", _customer.Phone, fields, params);
            AddInsertParam("PaymentTerms", _customer.PaymentTerms, fields, params);
            AddInsertParam("MaxDiscount", _customer.MaxDiscount, fields, params);
            AddInsertParam("CreditLimit", _customer.CreditLimit, fields, params);
            AddInsertParam("IsActive", std::string("Y"), fields, params);
            AddInsertParam("CreatedBy", _customer.CreatedBy, fields, params);
            AddInsertParam("CreateDate", _customer.CreateDate, fields, params);

            std::string query = "INSERT INTO Customers (";
            std::string values = " VALUES (";

            for (size_t i = 0; i < fields.size(); ++i)
            {
                query += fields[i];
                values += "?";

                if (i < fields.size() - 1)
                {
                    query += ", ";
                    values += ", ";
                }
            }
            query += ")" + values + ")";

            if (!Database->RunPrepared(query, params))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateCustomerSequence())
                throw std::runtime_error("[UpdateCustomerSequence exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[CreateCustomer Exception] ") + e.what());
        }
    }

    bool Customer::Update(const omnisphere::dtos::UpdateCustomer &_customer) const
    {
        try
        {
            std::string sQuery = "UPDATE Customers SET ";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> setClauses;

            AddSetParam("Code", _customer.Code, setClauses, params);
            AddSetParam("Name", _customer.Name, setClauses, params);
            AddSetParam("FirstName", _customer.FirstName, setClauses, params);
            AddSetParam("MiddleName", _customer.MiddleName, setClauses, params);
            AddSetParam("LastName", _customer.LastName, setClauses, params);
            AddSetParam("SecondLastName", _customer.SecondLastName, setClauses, params);
            AddSetParam("TaxID", _customer.TaxID, setClauses, params);
            AddSetParam("Email", _customer.Email, setClauses, params);
            AddSetParam("Phone", _customer.Phone, setClauses, params);
            AddSetParam("PaymentTerms", _customer.PaymentTerms, setClauses, params);
            AddSetParam("MaxDiscount", _customer.MaxDiscount, setClauses, params);
            AddSetParam("CreditLimit", _customer.CreditLimit, setClauses, params);
            AddSetParam("IsActive", _customer.IsActive, setClauses, params);
            AddSetParam("LastUpdatedBy", _customer.LastUpdatedBy, setClauses, params);
            AddSetParam("UpdateDate", _customer.UpdateDate, setClauses, params);

            if (setClauses.empty())
                return true;

            for (size_t i = 0; i < setClauses.size(); ++i)
            {
                sQuery += setClauses[i];

                if (i < setClauses.size() - 1)
                    sQuery += ", ";
            }

            sQuery += " WHERE Entry = ?";
            params.push_back(omnisphere::types::MakeSQLParam(_customer.Entry));

            if (!Database->RunPrepared(sQuery, params))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateCustomer Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Customer::Read(int entry) const
    {
        try
        {
            std::string sQuery = "SELECT * FROM Customers WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> params;
            params.push_back(omnisphere::types::MakeSQLParam(entry));

            return Database->FetchPrepared(sQuery, params);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadCustomer Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Customer::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM Customers WHERE IsActive = 'Y'";

            return Database->FetchResults(query);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadAllCustomer Exception] ") + e.what());
        }
    }

    int Customer::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT ISNULL(CustSequence, 0) + 1 CustSequence FROM Sequences WHERE SeqEntry = 1";
            omnisphere::types::DataTable dataTable = Database->FetchResults(query);

            if (dataTable.IsEmpty())
                return 1;
            return dataTable[0]["CustSequence"];
        }
        catch (...)
        {
            // fallback if column doesn't exist yet, we will fallback to max entry + 1
            try
            {
                omnisphere::types::DataTable dataTable = Database->FetchResults("SELECT ISNULL(MAX(Entry), 0) + 1 CustSequence FROM Customers");

                return dataTable[0]["CustSequence"];
            }
            catch (...)
            {
                return 1;
            }
        }
    }

    bool Customer::UpdateCustomerSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET CustSequence = ISNULL(CustSequence, 0) + 1 WHERE SeqEntry = 1";

            if (!Database->RunStatement(query))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (...)
        {
            return true; // ignore sequence errors if we are falling back
        }
    }
}
