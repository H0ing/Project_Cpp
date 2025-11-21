#include "sales.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

Sales::Sales(Inventory* inv) : head(nullptr), tail(nullptr), inventory(inv) {}

Sales::~Sales() {
    SaleItem* current = head;
    while (current != nullptr) {
        SaleItem* next = current->next;
        delete current;
        current = next;
    }
}

Sales::SaleItem* Sales::findInCart(const std::string& name) const {
    SaleItem* current = head;
    std::string upperName = Component::toUpperCase(name);
    
    while (current != nullptr) {
        if (Component::toUpperCase(current->name) == upperName) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void Sales::inputNameForSale(std::string& name, int option) const {
    while (true) {
        std::cout << (option == 2 ? "Enter the name of the component want to buy: " :
                        option == 4 ? "Enter the component want to delete from cart: " :
                        "Enter the name: ");
        
        std::getline(std::cin, name);
        name = Component::toUpperCase(name);
        
        if (name.empty()) {
            std::cout << "Please input name!!\n";
            continue;
        }
        if (name.length() > 25) {
            std::cout << "Name can be at most 25 characters!\n";
            continue;
        }
        break;
    }
}

void Sales::inputQuantityForSale(int& quantity, int option) const {
    std::string input;
    while (true) {
        std::cout << (option == 2 ? "Enter the amount of quantity want to buy: " :
                        option == 3 ? "Enter the new quantity: " :
                        "Enter the quantity: ");
        
        std::getline(std::cin, input);
        
        try {
            quantity = std::stoi(input);
            if (quantity > 0) {
                break;
            } else {
                std::cout << "Quantity must be positive!\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Please enter a valid number!\n";
        }
    }
}

char Sales::getYesNoOption(int caseType, char opt1, char opt2) const {
    std::string input;
    
    const char* prompts[] = {
        "Enter Y/N to make change information this component: ",
        "Enter Y/N to make change name this component: ",
        "Enter Y/N to make change quantity this component: ",
        "Enter Y/N to make change price this component: ",
        "Do you want to (D)ecrease or (I)ncrease: ",
        "Do you want to change price (Y/N): ",
        "Enter the option (U)pdate, (V)iew: ",
        "Do you want to add this component (Y/N): ",
        "Are you sure want to delete this component (Y/N): ",
        "Do you want buy other component (C)ontinue, (S)top: ",
        "Do you want to make change the quantity you buy (Y/N): ",
        "Are you accept for this buy (Y/N): ",
        "Do you want to buy other component or take some component out [(A)dd or (D)elete]: ",
        "Do you want to delete other component from the list you buy (Y/N): "
    };
    
    while (true) {
        std::cout << prompts[caseType];
        std::getline(std::cin, input);
        
        if (input.length() == 1) {
            char choice = std::toupper(input[0]);
            if (choice == opt1 || choice == opt2) {
                return choice;
            }
        }
        std::cout << "Please enter only '" << opt1 << "' or '" << opt2 << "'!\n";
    }
}

float Sales::calculateTotal() const {
    float total = 0.0f;
    SaleItem* current = head;
    while (current != nullptr) {
        total += current->quantity * current->price;
        current = current->next;
    }
    return total;
}

void Sales::addToCart() {
    std::string name;
    inputNameForSale(name, 2);
    
    // Check if component exists in inventory
    Component* inventoryComp = inventory->findComponentForSale(name);
    if (inventoryComp == nullptr) {
        std::cout << "This component doesn't exist in inventory!\n";
        return;
    }
    
    // Check if already in cart
    SaleItem* cartItem = findInCart(name);
    if (cartItem != nullptr) {
        std::cout << "This component is already in your cart!\n";
        char change = getYesNoOption(10, 'Y', 'N');
        if (change == 'Y') {
            int newQty;
            inputQuantityForSale(newQty, 3);
            if (newQty <= inventoryComp->getQuantity()) {
                cartItem->quantity = newQty;
            } else {
                std::cout << "Not enough stock! Available: " << inventoryComp->getQuantity() << "\n";
            }
        }
        return;
    }
    
    // Add new item to cart
    int quantity;
    inputQuantityForSale(quantity, 2);
    
    if (quantity > inventoryComp->getQuantity()) {
        std::cout << "Not enough stock! Available: " << inventoryComp->getQuantity() << "\n";
        return;
    }
    
    SaleItem* newItem = new SaleItem(name, quantity, inventoryComp->getPrice());
    
    if (head == nullptr) {
        head = tail = newItem;
    } else {
        tail->next = newItem;
        tail = newItem;
    }
    
    std::cout << "Added to cart successfully!\n";
}

void Sales::removeFromCart() {
    if (head == nullptr) {
        std::cout << "Your cart is empty!\n";
        return;
    }
    
    std::string name;
    inputNameForSale(name, 4);
    
    SaleItem* toDelete = findInCart(name);
    if (toDelete == nullptr) {
        std::cout << "This component is not in your cart!\n";
        return;
    }
    
    // Remove from cart
    if (toDelete == head && toDelete == tail) {
        head = tail = nullptr;
    } else if (toDelete == head) {
        head = toDelete->next;
    } else if (toDelete == tail) {
        SaleItem* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        current->next = nullptr;
        tail = current;
    } else {
        SaleItem* current = head;
        while (current->next != toDelete) {
            current = current->next;
        }
        current->next = toDelete->next;
    }
    
    delete toDelete;
    std::cout << "Removed from cart successfully!\n";
}

void Sales::displayCart() const {
    if (head == nullptr) {
        std::cout << "Your cart is empty!\n";
        return;
    }
    
    std::cout << "\n___________________ ITEMS IN YOUR CART ___________________\n";
    std::cout << "|                              |            |                 |\n";
    std::cout << "|        NAME COMPONENT        |  QUANTITY  |    PRICE ($)    |\n";
    std::cout << "|______________________________|____________|_________________|\n";
    
    SaleItem* current = head;
    while (current != nullptr) {
        std::string name = current->name;
        int qty = current->quantity;
        float price = current->price;
        
        std::cout << "| " << name;
        for (int i = name.length(); i < 28; i++) std::cout << " ";
        
        std::cout << "| " << qty;
        if (qty < 10) std::cout << "         ";
        else if (qty < 100) std::cout << "        ";
        else if (qty < 1000) std::cout << "       ";
        else std::cout << "      ";
        
        std::cout << "| $" << price;
        printf("%8.2f", price);
        if (price < 10) std::cout << "       ";
        else if (price < 100) std::cout << "      ";
        else if (price < 1000) std::cout << "     ";
        else std::cout << "    ";
        std::cout << "|\n";
        std::cout << "|______________________________|____________|_________________|\n";
        
        current = current->next;
    }
    
    std::cout << "\nTOTAL PRICE: $" << std::fixed << std::setprecision(2) << calculateTotal() << "\n";
}

void Sales::processSale() {
    head = tail = nullptr; // Reset cart
    
    std::cout << "=== PROCESS SALES ===\n";
    
    // Add items to cart
    char option;
    do {
        addToCart();
        option = getYesNoOption(9, 'C', 'S');
    } while (option == 'C');
    
    // Review and complete sale
    while (true) {
        if (head == nullptr) {
            std::cout << "You didn't buy anything!\n";
            break;
        }
        
        displayCart();
        option = getYesNoOption(11, 'Y', 'N');
        
        if (option == 'Y') {
            completeSale();
            break;
        } else {
            option = getYesNoOption(12, 'A', 'D');
            if (option == 'A') {
                addToCart();
            } else {
                removeFromCart();
            }
        }
    }
}

void Sales::completeSale() {
    // Update inventory quantities
    SaleItem* current = head;
    while (current != nullptr) {
        inventory->processSaleQuantity(current->name, current->quantity);
        current = current->next;
    }
    
    std::cout << "\n=== SALE COMPLETED ===\n";
    displayCart();
    std::cout << "Thank you for your purchase!\n";
    
    // Clear cart
    current = head;
    while (current != nullptr) {
        SaleItem* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
}