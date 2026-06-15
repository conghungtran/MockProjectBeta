#include "pch.h"
#include "Printer.h"
#include <iostream>
#include<atlstr.h>

using namespace PrinterHub::Core;

IMPLEMENT_DYNAMIC(Printer, CObject)

// Constructor
Printer::Printer(
      std::string ID
    , std::string model
    , PrinterBrand brand
    , PrinterStatus status
    , std::string purchaseDate
    , int warrantyMonth

)
    : ID(ID)
    , model(model)
    , brand(brand)
    , status(status)
    , purchaseDate(purchaseDate)
    , warrantyMonth(warrantyMonth)
{

}

//Printer::Printer() {}

// Destructor
Printer::~Printer()
{
}

// Copy constructor
Printer::Printer(const Printer& other)
    : ID(other.ID)
    , model(other.model)
    , brand(other.brand)
    , status(other.status)
    , purchaseDate(other.purchaseDate)
    , warrantyMonth(other.warrantyMonth)
{
}


// Assignment operator
Printer& Printer::operator=(const Printer& other)
{
    if (this != &other) {
        ID = other.ID;
        model = other.model;
        brand = other.brand;
        status = other.status;
        purchaseDate = other.purchaseDate;
        warrantyMonth = other.warrantyMonth;
    }
    return *this;
}


// Trong operator<< của bạn
std::ostream& operator<<(std::ostream& os, const Printer& p)
{
    os << "Printer{ID=" << p.getId()
        << ", Model=" << p.getModel()
        << ", Brand=" << static_cast<int>(p.getBrand())  // ✅ Dùng static_cast
        << ", Status=" << static_cast<int>(p.getStatus())  // ✅ Dùng static_cast
        << ", PurchaseDate=" << p.getPurchaseDate()
        << ", WarrantyMonth=" << p.getWarrantyMonth() << "}";
    return os;
}

void Printer::setAll(const std::string& mdl,
    PrinterBrand b, PrinterStatus s,
    const std::string& date, int months)
{
    model = mdl;
    brand = b;
    status = s;
    purchaseDate = date;
    warrantyMonth = months;
}

