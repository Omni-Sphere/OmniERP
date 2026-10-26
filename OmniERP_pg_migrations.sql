-- =============================================================================
-- OmniERP_pg_migrations.sql - Incremental Migrations & Alterations Script
-- For existing databases (DEV, TEST, PROD)
-- Native PostgreSQL Types: ENUMs, BOOLEAN, NUMERIC(19,4), TIMESTAMPTZ
-- Strict Case Preservation with Double Quotes
-- =============================================================================

-- -----------------------------------------------------------------------------
-- 1. PostgreSQL ENUM Types (Idempotent creation & value additions)
-- -----------------------------------------------------------------------------
DO $$ 
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'CustomerStatus') THEN
        CREATE TYPE "CustomerStatus" AS ENUM ('ACTIVE', 'INACTIVE', 'BLOCKED');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'TableStatus') THEN
        CREATE TYPE "TableStatus" AS ENUM ('AVAILABLE', 'OCCUPIED', 'RESERVED', 'CLEANING', 'OUT_OF_SERVICE');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'TableType') THEN
        CREATE TYPE "TableType" AS ENUM ('STANDARD', 'BOOTH', 'BAR', 'TERRACE', 'VIP');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'ItemType') THEN
        CREATE TYPE "ItemType" AS ENUM ('STANDARD', 'MODIFIER', 'COMBO', 'SERVICE');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'OrderType') THEN
        CREATE TYPE "OrderType" AS ENUM ('DINE_IN', 'TAKE_OUT', 'DELIVERY', 'DRIVE_THRU');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'OrderStatus') THEN
        CREATE TYPE "OrderStatus" AS ENUM ('OPEN', 'SENT', 'PREPARING', 'READY', 'DELIVERED', 'CLOSED', 'CANCELLED');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'NodeType') THEN
        CREATE TYPE "NodeType" AS ENUM ('CASHIER', 'SERVICE_STATION');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'OperationMode') THEN
        CREATE TYPE "OperationMode" AS ENUM ('POS', 'RESTAURANT', 'TOUCH');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'EmployeeStatus') THEN
        CREATE TYPE "EmployeeStatus" AS ENUM ('ACTIVE', 'INACTIVE', 'ON_LEAVE', 'TERMINATED');
    END IF;

    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'PermissionAction') THEN
        CREATE TYPE "PermissionAction" AS ENUM ('CREATE', 'READ', 'UPDATE', 'DELETE', 'EXECUTE');
    END IF;
END $$;

-- -----------------------------------------------------------------------------
-- 2. Migración IsCanceled (Soft Delete Estándar)
-- -----------------------------------------------------------------------------
ALTER TABLE IF EXISTS "Company" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Stores" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Departments" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Employees" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Floors" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Areas" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Tables" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Nodes" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "ItemBrands" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "ItemGroups" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Items" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Customers" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Modules" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Permissions" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;
ALTER TABLE IF EXISTS "Roles" ADD COLUMN IF NOT EXISTS "IsCanceled" BOOLEAN NOT NULL DEFAULT false;

-- -----------------------------------------------------------------------------
-- 3. Índices de Rendimiento
-- -----------------------------------------------------------------------------
CREATE INDEX IF NOT EXISTS "IX_Tables_AreaEntry" ON "Tables" ("AreaEntry");
CREATE INDEX IF NOT EXISTS "IX_Tables_FloorEntry" ON "Tables" ("FloorEntry");
CREATE INDEX IF NOT EXISTS "IX_Tables_Status" ON "Tables" ("Status");
CREATE INDEX IF NOT EXISTS "IX_Tables_IsActive" ON "Tables" ("IsActive");

CREATE INDEX IF NOT EXISTS "IX_Items_Group" ON "Items" ("Group");
CREATE INDEX IF NOT EXISTS "IX_Items_Brand" ON "Items" ("Brand");
CREATE INDEX IF NOT EXISTS "IX_Items_ItemType" ON "Items" ("ItemType");
CREATE INDEX IF NOT EXISTS "IX_Items_IsActive" ON "Items" ("IsActive");
CREATE INDEX IF NOT EXISTS "IX_Items_IsCanceled" ON "Items" ("IsCanceled");

CREATE INDEX IF NOT EXISTS "IX_Customers_Status" ON "Customers" ("Status");
CREATE INDEX IF NOT EXISTS "IX_Customers_IsActive" ON "Customers" ("IsActive");
CREATE INDEX IF NOT EXISTS "IX_Customers_IsCanceled" ON "Customers" ("IsCanceled");

CREATE INDEX IF NOT EXISTS "IX_Areas_FloorEntry" ON "Areas" ("FloorEntry");
CREATE INDEX IF NOT EXISTS "IX_Areas_IsActive" ON "Areas" ("IsActive");
