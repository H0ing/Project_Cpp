#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>

class Component {
private:
    // int id
    std::string name;
    int quantity;
    float price;
    Component* next;

public:
    Component(const std::string& name = "", int quantity = 0, float price = 0.0f);
    
    // Getters and Setters
    std::string getName() const;
    void setName(const std::string& newName);
    int getQuantity() const;
    void setQuantity(int newQuantity);
    float getPrice() const;
    void setPrice(float newPrice);
    Component* getNext() const;
    void setNext(Component* nextNode);
    
    // Validation methods
    static bool isValidName(const std::string& name);
    static bool isValidQuantity(int quantity);
    static bool isValidPrice(float price);
    
    // Utility
    static std::string toUpperCase(const std::string& str);
};

#endif