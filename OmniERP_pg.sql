-- =============================================================================
-- OmniERP_pg.sql - Master ERP & POS Schema DDL for PostgreSQL
-- Native PostgreSQL Types: ENUMs, BOOLEAN, NUMERIC(19,4), TIMESTAMPTZ
-- Strict Case Preservation with Double Quotes
-- Fully Idempotent Execution (DO $$ ... $$ and CREATE TABLE IF NOT EXISTS)
-- =============================================================================

-- -----------------------------------------------------------------------------
-- 1. PostgreSQL ENUM Types
-- -----------------------------------------------------------------------------
DO $$ 
BEGIN
    -- Customer status
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'CustomerStatus') THEN
        CREATE TYPE "CustomerStatus" AS ENUM ('ACTIVE', 'INACTIVE', 'BLOCKED');
    END IF;

    -- Restaurant/Cafe Table status
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'TableStatus') THEN
        CREATE TYPE "TableStatus" AS ENUM ('AVAILABLE', 'OCCUPIED', 'RESERVED', 'CLEANING', 'OUT_OF_SERVICE');
    END IF;

    -- Table Type classification
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'TableType') THEN
        CREATE TYPE "TableType" AS ENUM ('STANDARD', 'BOOTH', 'BAR', 'TERRACE', 'VIP');
    END IF;

    -- Item / Product classification
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'ItemType') THEN
        CREATE TYPE "ItemType" AS ENUM ('STANDARD', 'MODIFIER', 'COMBO', 'SERVICE');
    END IF;

    -- POS / Restaurant Order Type
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'OrderType') THEN
        CREATE TYPE "OrderType" AS ENUM ('DINE_IN', 'TAKE_OUT', 'DELIVERY', 'DRIVE_THRU');
    END IF;

    -- Order lifecycle status
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'OrderStatus') THEN
        CREATE TYPE "OrderStatus" AS ENUM ('OPEN', 'SENT', 'PREPARING', 'READY', 'DELIVERED', 'CLOSED', 'CANCELLED');
    END IF;

    -- Terminal / Station POS Node Type
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'NodeType') THEN
        CREATE TYPE "NodeType" AS ENUM ('CASHIER', 'SERVICE_STATION');
    END IF;

    -- Station Operation Mode
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'OperationMode') THEN
        CREATE TYPE "OperationMode" AS ENUM ('POS', 'RESTAURANT', 'TOUCH');
    END IF;

    -- Employee employment status
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'EmployeeStatus') THEN
        CREATE TYPE "EmployeeStatus" AS ENUM ('ACTIVE', 'INACTIVE', 'ON_LEAVE', 'TERMINATED');
    END IF;

    -- Permission Actions
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'PermissionAction') THEN
        CREATE TYPE "PermissionAction" AS ENUM ('CREATE', 'READ', 'UPDATE', 'DELETE', 'EXECUTE');
    END IF;

    -- Permission Mode
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'PermissionMode') THEN
        CREATE TYPE "PermissionMode" AS ENUM ('P', 'R');
    END IF;
END $$;

-- -----------------------------------------------------------------------------
-- 2. Company & Organizational Structure
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS "Company" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "CommercialName" VARCHAR(200) NOT NULL UNIQUE,
    "Address" VARCHAR(200),
    "Address2" VARCHAR(200),
    "City" INT,
    "State" INT,
    "ZipCode" INT,
    "Country" INT,
    "TaxID" VARCHAR(50),
    "Currency" INT,
    "Phone1" VARCHAR(50),
    "Phone2" VARCHAR(50),
    "Email" VARCHAR(50),
    "WebSite" VARCHAR(50),
    "FacebookProfile" VARCHAR(50),
    "InstagramProfile" VARCHAR(50),
    "XProfile" VARCHAR(50),
    "LogoFile" VARCHAR(50),
    "AttachmentsPath" VARCHAR(255),
    "ImagePath" VARCHAR(255),
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Stores" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "Address" VARCHAR(200),
    "Address2" VARCHAR(200),
    "City" INT,
    "State" INT,
    "ZipCode" INT,
    "Country" INT,
    "TaxID" VARCHAR(50),
    "Currency" INT,
    "Phone1" VARCHAR(50),
    "Phone2" VARCHAR(50),
    "Email" VARCHAR(50),
    "CountCustomer" INT NOT NULL DEFAULT 0,
    "WebSite" VARCHAR(50),
    "FacebookProfile" VARCHAR(50),
    "InstagramProfile" VARCHAR(50),
    "XProfile" VARCHAR(50),
    "LogoFile" VARCHAR(50),
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Departments" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Employees" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "FirstName" VARCHAR(50) NOT NULL,
    "MiddleName" VARCHAR(50),
    "LastName" VARCHAR(50) NOT NULL,
    "SecondLastName" VARCHAR(50),
    "Phone" VARCHAR(20),
    "DateOfBirth" DATE,
    "PlaceOfBirth" VARCHAR(50),
    "Comments" VARCHAR(200),
    "Status" "EmployeeStatus" NOT NULL DEFAULT 'ACTIVE',
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

-- -----------------------------------------------------------------------------
-- 3. Restaurant & POS Physical Layout (Floors, Areas, Tables, Nodes)
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS "Floors" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Areas" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "Color" VARCHAR(50) NOT NULL DEFAULT '#6366F1',
    "Icon" VARCHAR(50) NOT NULL DEFAULT 'layout',
    "Capacity" INT NOT NULL CHECK ("Capacity" > 0),
    "FloorEntry" INT NOT NULL,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Tables" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "Capacity" INT NOT NULL CHECK ("Capacity" > 0),
    "Type" VARCHAR(20) NOT NULL DEFAULT 'STANDARD',
    "Status" "TableStatus" NOT NULL DEFAULT 'AVAILABLE',
    "AreaEntry" INT NOT NULL,
    "FloorEntry" INT NOT NULL,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Nodes" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL,
    "StoreEntry" INT NOT NULL DEFAULT 1,
    "NodeType" "NodeType" NOT NULL DEFAULT 'CASHIER',
    "OperationMode" "OperationMode" NOT NULL DEFAULT 'RESTAURANT',
    "CashDrawerPort" VARCHAR(50),
    "DefaultPrinter" VARCHAR(100),
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

-- -----------------------------------------------------------------------------
-- 4. Products & Catalog (ItemBrands, ItemGroups, Items)
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS "ItemBrands" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "ItemGroups" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Items" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(50) NOT NULL UNIQUE,
    "Name" VARCHAR(150) NOT NULL UNIQUE,
    "Description" VARCHAR(254),
    "Image" VARCHAR(255),
    "ItemType" "ItemType" NOT NULL DEFAULT 'STANDARD',
    "PurchaseItem" BOOLEAN NOT NULL DEFAULT TRUE,
    "SellItem" BOOLEAN NOT NULL DEFAULT TRUE,
    "InventoryItem" BOOLEAN NOT NULL DEFAULT TRUE,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "Price" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "TaxRate" NUMERIC(6, 4) NOT NULL DEFAULT 0.1600,
    "Brand" INT,
    "Group" INT,
    "OnHand" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "OnOrder" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "OnRequest" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "MinOrder" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "MinRequest" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "MinStock" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "MaxOrder" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "MaxRequest" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "MaxStock" NUMERIC(19, 4) NOT NULL DEFAULT 0.0000,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

-- -----------------------------------------------------------------------------
-- 5. Customers
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS "Customers" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(50) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "FirstName" VARCHAR(50) NOT NULL,
    "MiddleName" VARCHAR(50),
    "LastName" VARCHAR(50) NOT NULL,
    "SecondLastName" VARCHAR(50),
    "TaxID" VARCHAR(50),
    "Email" VARCHAR(100),
    "Phone" VARCHAR(50),
    "PaymentTerms" INT NOT NULL DEFAULT -1,
    "MaxDiscount" NUMERIC(19, 6) NOT NULL DEFAULT 0.000000 CHECK ("MaxDiscount" >= 0),
    "CreditLimit" NUMERIC(19, 6) DEFAULT 0.000000,
    "Status" "CustomerStatus" NOT NULL DEFAULT 'ACTIVE',
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

-- -----------------------------------------------------------------------------
-- 6. Access Control & Security (Modules, Permissions, Roles, RoleModules, RolePermissions, UserPermissions)
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS "Modules" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "DepartmentEntry" INT NOT NULL DEFAULT -1,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Permissions" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "Action" "PermissionAction" NOT NULL DEFAULT 'READ',
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "Roles" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL UNIQUE,
    "DepartmentEntry" INT,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "RoleModules" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL,
    "RoleEntry" INT NOT NULL,
    "ModuleEntry" INT NOT NULL,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "RolePermissions" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL,
    "RoleEntry" INT NOT NULL,
    "PermissionEntry" INT NOT NULL,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS "UserPermissions" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL,
    "UserEntry" INT NOT NULL,
    "PermissionEntry" INT NOT NULL,
    "IsActive" BOOLEAN NOT NULL DEFAULT TRUE,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

-- -----------------------------------------------------------------------------
-- 7. Sequences Table (Business Consecutive Generation)
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS "Sequences" (
    "Entry" SERIAL PRIMARY KEY,
    "Code" VARCHAR(20) NOT NULL UNIQUE,
    "Name" VARCHAR(100) NOT NULL,
    "UserSequence" INT NOT NULL DEFAULT 1,
    "StoreSequence" INT NOT NULL DEFAULT 1,
    "CustomerSequence" INT NOT NULL DEFAULT 1,
    "AreaSequence" INT NOT NULL DEFAULT 1,
    "FloorSequence" INT NOT NULL DEFAULT 1,
    "DeparmentSequence" INT NOT NULL DEFAULT 1,
    "EmployeeSequence" INT NOT NULL DEFAULT 1,
    "ItemSequence" INT NOT NULL DEFAULT 1,
    "ItemBrandSequence" INT NOT NULL DEFAULT 1,
    "ItemGroupSequence" INT NOT NULL DEFAULT 1,
    "RoleSequence" INT NOT NULL DEFAULT 1,
    "UserModuleSequence" INT NOT NULL DEFAULT 1,
    "UserPermissionSequence" INT NOT NULL DEFAULT 1,
    "TableSequence" INT NOT NULL DEFAULT 1,
    "NodeSequence" INT NOT NULL DEFAULT 1,
    "CreatedBy" INT NOT NULL DEFAULT 0,
    "CreateDate" TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "LastUpdatedBy" INT,
    "UpdateDate" TIMESTAMPTZ
);

-- Seed Sequences initial row if not exists
INSERT INTO "Sequences" (
    "Entry", "Code", "Name", "UserSequence", "StoreSequence", "CustomerSequence",
    "AreaSequence", "FloorSequence", "DeparmentSequence", "EmployeeSequence",
    "ItemSequence", "ItemBrandSequence", "ItemGroupSequence", "RoleSequence",
    "UserModuleSequence", "UserPermissionSequence", "TableSequence", "NodeSequence",
    "CreatedBy", "CreateDate"
) VALUES (
    1, 'SEQ-MASTER', 'Correlativos Maestros ERP', 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    0, CURRENT_TIMESTAMP
) ON CONFLICT ("Entry") DO NOTHING;

-- -----------------------------------------------------------------------------
-- 8. Relational Performance Indexes
-- -----------------------------------------------------------------------------
CREATE INDEX IF NOT EXISTS "IX_Tables_AreaEntry" ON "Tables" ("AreaEntry");
CREATE INDEX IF NOT EXISTS "IX_Tables_FloorEntry" ON "Tables" ("FloorEntry");
CREATE INDEX IF NOT EXISTS "IX_Tables_Status" ON "Tables" ("Status");
CREATE INDEX IF NOT EXISTS "IX_Tables_IsActive" ON "Tables" ("IsActive");

CREATE INDEX IF NOT EXISTS "IX_Items_Group" ON "Items" ("Group");
CREATE INDEX IF NOT EXISTS "IX_Items_Brand" ON "Items" ("Brand");
CREATE INDEX IF NOT EXISTS "IX_Items_ItemType" ON "Items" ("ItemType");
CREATE INDEX IF NOT EXISTS "IX_Items_IsActive" ON "Items" ("IsActive");

CREATE INDEX IF NOT EXISTS "IX_Customers_Status" ON "Customers" ("Status");
CREATE INDEX IF NOT EXISTS "IX_Customers_IsActive" ON "Customers" ("IsActive");

CREATE INDEX IF NOT EXISTS "IX_Areas_FloorEntry" ON "Areas" ("FloorEntry");
CREATE INDEX IF NOT EXISTS "IX_Areas_IsActive" ON "Areas" ("IsActive");
