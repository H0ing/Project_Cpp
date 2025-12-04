#include <iostream>
#include "inventory.hpp"
#include "sales.hpp"
#include "auth.hpp"

void displayMenu() {
    std::cout << "\n=== INVENTORY MANAGEMENT SYSTEM ===" << std::endl;
    std::cout << "1. Add Component" << std::endl;
    std::cout << "2. Search Component" << std::endl;
    std::cout << "3. View All Components" << std::endl;
    std::cout << "4. Delete Component" << std::endl;
    std::cout << "5. Process Sale" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose option: ";
}

int getMenuOption() {
    std::string input;
    while (true) {
        displayMenu();
        std::getline(std::cin, input);
        
        try {
            int choice = std::stoi(input);
            if (choice >= 0 && choice <= 5) {
                return choice;
            } else {
                std::cout << "Please enter 0-5!" << std::endl;
            }
        } catch (...) {
            std::cout << "Please enter a valid number!" << std::endl;
        }
    }
}

int main() {
    // // Authentication - will initialize credentials if first time
    // Authentication auth;
    
    // // Now authenticate with the credentials (either newly created or loaded)
    // if (!auth.authenticate()) {
    //     return 1;
    // }
    
    Inventory inventory;
    Sales sales(&inventory);
    
    // Load existing data
    inventory.loadFromFile("component_list.txt");
    
    int choice;
    do {
        inventory.clearScreen();
        choice = getMenuOption();
        inventory.clearScreen();
        
        switch (choice) {
            case 1:
                std::cout << "[1] ADD COMPONENT\n\n";
                inventory.addComponent();
                break;
            case 2:
                std::cout << "[2] SEARCH COMPONENT\n\n";
                inventory.searchComponent();
                break;
            case 3:
                std::cout << "[3] VIEW ALL COMPONENTS\n\n";
                inventory.displayAll();
                break;
            case 4:
                std::cout << "[4] DELETE COMPONENT\n\n";
                inventory.deleteComponent();
                break;
            case 5:
                std::cout << "[5] PROCESS SALES\n\n";
                sales.processSale();
                break;
            case 0:
                inventory.saveToFile("component_list.txt");
                std::cout << "Data saved. Goodbye!" << std::endl;
                break;
        }
        
        if (choice != 0) {
            inventory.pressAnyKey();
        }
    } while (choice != 0);
    
    return 0;
}