#ifndef SALES_HPP
#define SALES_HPP

#include "component.hpp"
#include "inventory.hpp"
#include <vector>

class Sales {
private:
    
    
    Component* head;
    Component* tail;
    Inventory* inventory;
    
    // Helper methods
    Component* findInCart(const std::string& name) const;
    void inputNameForSale(std::string& name, int option) const;
    void inputQuantityForSale(int& quantity, int option) const;
    char getYesNoOption(int caseType, char opt1, char opt2) const;
    float calculateTotal() const;
    void print_space(int size) const;
    void print_underScore(int size) const;
    
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