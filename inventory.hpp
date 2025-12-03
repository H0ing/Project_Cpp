#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "component.hpp"
#include <string>

class Inventory {
private:
    Component* head;
    Component* tail;
    
    // Helper methods
    Component* findComponent(const std::string& name) const;
    void inputName(std::string& name, int option) const;
    void inputQuantity(int& quantity, int option) const;
    void inputPrice(float& price) const;
    char getYesNoOption(int caseType, char opt1, char opt2) const;
    
public:
    Inventory();
    ~Inventory();
    
    // Core operations
    void addComponent();
    void searchComponent() const;
    void updateComponent(Component* component);
    void deleteComponent();
    void displayAll() const;
    
    // File operations
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    // Sales related
    Component* findComponentForSale(const std::string& name) const;
    void processSaleQuantity(const std::string& name, int soldQuantity);
    
    // Utility
    bool isEmpty() const;
    void clearScreen() const;
    void pressAnyKey() const;
    void print_space(int size) const;
    void print_underScore(int size) const;
    
    // For sales class access
    Component* getHead() const { return head; }
};

#endif