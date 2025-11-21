#ifndef SALES_HPP
#define SALES_HPP

#include "component.hpp"
#include "inventory.hpp"
#include <vector>

class Sales {
private:
    struct SaleItem {
        std::string name;
        int quantity;
        float price;
        SaleItem* next;
        
        SaleItem(const std::string& n, int q, float p) : name(n), quantity(q), price(p), next(nullptr) {}
    };
    
    SaleItem* head;
    SaleItem* tail;
    Inventory* inventory;
    
    // Helper methods
    SaleItem* findInCart(const std::string& name) const;
    void inputNameForSale(std::string& name, int option) const;
    void inputQuantityForSale(int& quantity, int option) const;
    char getYesNoOption(int caseType, char opt1, char opt2) const;
    float calculateTotal() const;
    
public:
    Sales(Inventory* inv);
    ~Sales();
    
    void processSale();
    void addToCart();
    void removeFromCart();
    void displayCart() const;
    void completeSale();
};

#endif