#include "inventory.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <iomanip> // Required for std::fixed and std::setprecision

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

Inventory::Inventory() : head(nullptr), tail(nullptr) {}

Inventory::~Inventory() {
    Component* current = head;
    while (current != nullptr) {
        Component* next = current->getNext();
        delete current;
        current = next;
    }
}

Component* Inventory::findComponent(const std::string& name) const {
    Component* current = head;
    std::string upperName = Component::toUpperCase(name);
    
    while (current != nullptr) {
        if (Component::toUpperCase(current->getName()) == upperName) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void Inventory::inputName(std::string& name, int option) const {
    while (true) {
        std::cout << (option == 2 ? "Enter the name of the component want to buy: " :
                        option == 4 ? "Enter the component want to delete: " :
                        "Enter the name of the component: ");
        
        std::getline(std::cin, name);
        
        // Convert to uppercase
        name = Component::toUpperCase(name);
        
        // Validation
        if (name.empty()) {
            std::cout << "Please input name of the component!!\n";
            continue;
        }
        if (name.length() > 25) {
            std::cout << "Name can be at most 25 characters!\n";
            continue;
        }
        if (!Component::isValidName(name)) {
            std::cout << "Please input only alphabetic characters and spaces!!\n";
            continue;
        }
        break;
    }
}

void Inventory::inputQuantity(int& quantity, int option) const {
    std::string input;
    while (true) {
        std::cout << (option == 2 ? "Enter the amount of quantity want to buy: " :
                        option == 3 ? "Enter the new amount of quantity: " :
                        "Enter the quantity of component: ");
        
        std::getline(std::cin, input);
        
        try {
            quantity = std::stoi(input);
            if (Component::isValidQuantity(quantity)) {
                break;
            } else {
                std::cout << "Quantity must be between 0 and 32767!\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Please enter a valid number!\n";
        }
    }
}

void Inventory::inputPrice(float& price) const {
    std::string input;
    while (true) {
        std::cout << "Enter the price of component ($): ";
        std::getline(std::cin, input);
        
        try {
            price = std::stof(input);
            if (Component::isValidPrice(price)) {
                break;
            } else {
                std::cout << "Price must be between 0 and 999999.99!\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Please enter a valid price!\n";
        }
    }
}

char Inventory::getYesNoOption(int caseType, char opt1, char opt2) const {
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

void Inventory::addComponent() {
    std::string name;
    int quantity;
    float price;
    
    // Input name with validation
    do {
        inputName(name, 1);
        if (findComponent(name) != nullptr) {
            std::cout << "Component already exists!\n";
            name.clear();
        }
    } while (name.empty());
    
    inputQuantity(quantity, 1);
    inputPrice(price);
    
    Component* newComponent = new Component(name, quantity, price);
    
    if (head == nullptr) {
        head = tail = newComponent;
    } else {
        tail->setNext(newComponent);
        tail = newComponent;
    }
    
    std::cout << "Component added successfully!\n";
}
void Inventory::print_space(int size) const{
    for (int i = 0; i < size; i++)
		std::cout<<" ";
    
}
void Inventory::print_underScore(int size) const{
   
    for(int i = 0; i<size; i++)
        std::cout<<"_";
    
}

void Inventory::displayAll() const {
    if (isEmpty()) {
        std::cout << "Inventory is empty!\n";
        return;
    }
    
    // Display header
    for(int i = 0; i<59; i++)
		std::cout<<"_";
	std::cout<<"\n";
	std::cout<<"|";
    print_space(30);
    std::cout<<"|";
    print_space(10);
    std::cout<<"|";
    print_space(15);
    std::cout<<"|"<<std::endl;
    std::cout<<"|  Name Component";
    print_space(14);
    std::cout<<"| ";
    std::cout<<"Quantity ";
    std::cout<<"|  Price ($)";
    print_space(4);
	std::cout<<"|"<<std::endl;
    std::cout<<"|";
    print_underScore(30);
	std::cout<<"|";
    print_underScore(10);
	std::cout<<"|";
    print_underScore(15);
	std::cout<<"|"<<std::endl;
    
    Component* current = head;
    while (current != nullptr) {
        std::string name = current->getName();
        int qty = current->getQuantity();
        float price = current->getPrice();
        
        // Format the output to match your original table design
		std::cout<<"|";
        print_space(30);
		std::cout<<"|";

        print_space(10);
		std::cout<<"|";
        print_space(15);
		std::cout<<"|\n";
        std::cout<<"|  "<<name;
        print_space(30-name.length()-2);
		std::cout<<"| ";
        std::cout<<qty;
        if (qty < 10)
            print_space(8);
        else if (qty < 100)
            print_space(7);
        else if (qty < 1000)
            print_space(6);
        else if (qty < 10000)
            print_space(5);
        else
            print_space(4);
		std::cout<< std::fixed << std::setprecision(2) <<"| "<<price;
  
        if (price < 10)
            print_space(9);
        else if (price < 100)
            print_space(8);
        else if (price < 1000)
            print_space(7);
        else if (price < 10000)
            print_space(6);
        else if (price < 100000)
            print_space(5);
        else
            print_space(4);
		std::cout<<" |\n";
		std::cout<<"|";
        print_underScore(30);
        printf("|");
        print_underScore(10);
		std::cout<<"|";
        print_underScore(15);
		std::cout<<"|\n";
		
		
        current = current->getNext();
    }
}


void Inventory::deleteComponent() {
    if (isEmpty()) {
        std::cout << "The system doesn't have any component!!\n";
        return;
    }
    
    std::string componentName;
    inputName(componentName, 0);
    
    Component* toDelete = findComponent(componentName);
    
    if (toDelete != nullptr) {
        // Display component info
        std::cout << "Name Component: " << toDelete->getName() << "\n";
        std::cout << "Quantity Component: " << toDelete->getQuantity() << "\n";
        std::cout << "Price Component: $" << toDelete->getPrice() << "\n";
        
        char confirm = getYesNoOption(8, 'Y', 'N');
        
        if (confirm == 'Y') {
            // Handle different deletion cases
            if (toDelete == head && toDelete == tail) {
                // Only one component in list
                head = tail = nullptr;
            } else if (toDelete == head) {
                // Delete first component
                head = toDelete->getNext();
            } else if (toDelete == tail) {
                // Delete last component
                Component* current = head;
                while (current->getNext() != tail) {
                    current = current->getNext();
                }
                current->setNext(nullptr);
                tail = current;
            } else {
                // Delete middle component
                Component* current = head;
                while (current->getNext() != toDelete) {
                    current = current->getNext();
                }
                current->setNext(toDelete->getNext());
            }
            
            delete toDelete;
            std::cout << "Component deleted successfully!\n";
        } else {
            std::cout << "Deletion cancelled. Component is safe!\n";
        }
    } else {
        std::cout << "Component not found in the system!\n";
    }
}

void Inventory::updateComponent(Component* component) {
    if (component == nullptr) return;
    
    char confirm = getYesNoOption(0, 'Y', 'N');
    if (confirm != 'Y') return;
    
    // Temporary storage for new values
    std::string newName;
    int newQuantity;
    float newPrice;
    
    // Update name
    char updateName = getYesNoOption(1, 'Y', 'N');
    if (updateName == 'Y') {
        do {
            inputName(newName, 0);
            if (newName == component->getName()) {
                std::cout << "You entered the same name! Please change it!\n";
            } else {
                component->setName(newName);
                break;
            }
        } while (true);
    }
    
    // Update quantity
    char updateQuantity = getYesNoOption(2, 'Y', 'N');
    if (updateQuantity == 'Y') {
        char operation = getYesNoOption(4, 'D', 'I');
        inputQuantity(newQuantity, 1);
        
        int currentQty = component->getQuantity();
        if (operation == 'I') {
            if (currentQty + newQuantity <= 32767) {
                component->setQuantity(currentQty + newQuantity);
            } else {
                std::cout << "Quantity would exceed maximum limit!\n";
            }
        } else { // Decrease
            if (currentQty - newQuantity >= 0) {
                component->setQuantity(currentQty - newQuantity);
            } else {
                std::cout << "Quantity cannot be negative!\n";
            }
        }
    }
    
    // Update price
    char updatePrice = getYesNoOption(3, 'Y', 'N');
    if (updatePrice == 'Y') {
        do {
            inputPrice(newPrice);
            if (newPrice == component->getPrice()) {
                std::cout << "You entered the same price! Please change it!\n";
            } else {
                component->setPrice(newPrice);
                break;
            }
        } while (true);
    }
    
    std::cout << "Component updated successfully!\n";
}

void Inventory::searchComponent() const {
    std::string searchName;
    inputName(searchName, 0);
    
    Component* found = findComponent(searchName);
    
    if (found != nullptr) {
        std::cout << "Component found in system!\n";
        std::cout << "Name: " << found->getName() << "\n";
        std::cout << "Quantity: " << found->getQuantity() << "\n";
        std::cout << "Price: $" << found->getPrice() << "\n";
        
        char option = getYesNoOption(6, 'U', 'V');
        if (option == 'U') {
            // For update, we need non-const version
            const_cast<Inventory*>(this)->updateComponent(found);
        }
    } else {
        std::cout << "Component not found in system!\n";
        char option = getYesNoOption(7, 'Y', 'N');
        if (option == 'Y') {
            const_cast<Inventory*>(this)->addComponent();
        }
    }
}


void Inventory::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file for saving!\n";
        return;
    }
    
    Component* current = head;
    while (current != nullptr) {
        file << current->getName() << ","
             << current->getQuantity() << ","
             << current->getPrice() << "\n";
        current = current->getNext();
    }
    
    file.close();
}

void Inventory::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return; // No file to load is not an error
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, qtyStr, priceStr;
        
        if (std::getline(ss, name, ',') &&
            std::getline(ss, qtyStr, ',') &&
            std::getline(ss, priceStr)) {
            
            try {
                int quantity = std::stoi(qtyStr);
                float price = std::stof(priceStr);
                
                Component* newComponent = new Component(name, quantity, price);
                
                if (head == nullptr) {
                    head = tail = newComponent;
                } else {
                    tail->setNext(newComponent);
                    tail = newComponent;
                }
            } catch (const std::exception& e) {
                std::cout << "Error parsing line: " << line << "\n";
            }
        }
    }
    
    file.close();
}

bool Inventory::isEmpty() const {
    return head == nullptr;
}

void Inventory::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Inventory::pressAnyKey() const {
    std::cout << "\nPress any key to continue...";
#ifdef _WIN32
    _getch();
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

Component* Inventory::findComponentForSale(const std::string& name) const {
    return findComponent(name);
}

void Inventory::processSaleQuantity(const std::string& name, int soldQuantity) {
    Component* comp = findComponent(name);
    if (comp != nullptr) {
        comp->setQuantity(comp->getQuantity() - soldQuantity);
    }
}