#include "sales.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

Sales::Sales(Inventory *inv) : head(nullptr), tail(nullptr), inventory(inv) {}

Sales::~Sales()
{
    Component *current = head;
    while (current != nullptr)
    {
        Component *next = current->getNext();
        delete current;
        current = next;
    }
}
void Sales::print_space(int size) const
{
    for (int i = 0; i < size; i++)
        std::cout << " ";
}
void Sales::print_underScore(int size) const
{

    for (int i = 0; i < size; i++)
        std::cout << "_";
}
Component *Sales::findInCart(const std::string &name) const
{
    Component *current = head;
    std::string upperName = Component::toUpperCase(name);

    while (current != nullptr)
    {
        if (Component::toUpperCase(current->getName()) == upperName)
        {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void Sales::inputNameForSale(std::string &name, int option) const
{
    while (true)
    {
        std::cout << (option == 2 ? "Enter the name of the component want to buy: " : option == 4 ? "Enter the component want to delete from cart: "
                                                                                                  : "Enter component name: ");

        std::getline(std::cin, name);
        name = Component::toUpperCase(name);

        if (name.empty())
        {
            std::cout << "Please input a name!!\n";
            continue;
        }
        if (name.length() > 25)
        {
            std::cout << "Name can be at most 25 characters!\n";
            continue;
        }
        break;
    }
}

void Sales::inputQuantityForSale(int &quantity, int option) const
{
    std::string input;
    while (true)
    {
        std::cout << (option == 2 ? "Enter the amount of quantity want to buy: " : option == 3 ? "Enter the new quantity: "
                                                                                               : "Enter quantity: ");

        std::getline(std::cin, input);

        try
        {
            quantity = std::stoi(input);
            if (quantity > 0)
            {
                break;
            }
            else
            {
                std::cout << "Quantity must be positive!\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Please enter a valid number!\n";
        }
    }
}

char Sales::getYesNoOption(int caseType, char opt1, char opt2) const
{
    std::string input;

    const char *prompts[] = {
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
        "Do you want to delete other component from the list you buy (Y/N): "};

    while (true)
    {
        if (caseType >= 0 && caseType <= 13)
        {
            std::cout << prompts[caseType];
        }
        else
        {
            std::cout << "Enter " << opt1 << "/" << opt2 << ": ";
        }

        std::getline(std::cin, input);

        if (input.length() == 1)
        {
            char choice = std::toupper(input[0]);
            if (choice == opt1 || choice == opt2)
            {
                return choice;
            }
        }
        std::cout << "Please enter only '" << opt1 << "' or '" << opt2 << "'!\n";
    }
}

float Sales::calculateTotal() const
{
    float total = 0.0f;
    Component *current = head;
    while (current != nullptr)
    {
        total += current->getQuantity() * current->getPrice();
        current = current->getNext();
    }
    return total;
}

void Sales::addToCart()
{
    std::string name;
    inputNameForSale(name, 2);

    // Check if component exists in inventory
    Component *inventoryComp = inventory->findComponentForSale(name);
    if (inventoryComp == nullptr)
    {
        std::cout << "This component doesn't exist in inventory!\n";
        return;
    }

    // Check if already in cart
    Component *cartItem = findInCart(name);
    if (cartItem != nullptr)
    {
        std::cout << "This component is already in your cart!\n";
        char change = getYesNoOption(10, 'Y', 'N'); // Case 10: Change quantity?
        if (change == 'Y')
        {
            int newQty;
            inputQuantityForSale(newQty, 3);
            if (newQty <= inventoryComp->getQuantity())
            {
                cartItem->setQuantity(newQty);
                std::cout << "Quantity updated successfully!\n";
            }
            else
            {
                std::cout << "Not enough stock! Available: " << inventoryComp->getQuantity() << "\n";
            }
        }
        return;
    }

    // Add new item to cart
    int quantity;
    inputQuantityForSale(quantity, 2);

    if (quantity > inventoryComp->getQuantity())
    {
        std::cout << "Not enough stock! Available: " << inventoryComp->getQuantity() << "\n";
        return;
    }

    // Create a new component for the cart
    Component *newItem = new Component(name, quantity, inventoryComp->getPrice());

    if (head == nullptr)
    {
        head = tail = newItem;
    }
    else
    {
        tail->setNext(newItem);
        tail = newItem;
    }

    std::cout << "Added to cart successfully!\n";
}

void Sales::removeFromCart()
{
    if (head == nullptr)
    {
        std::cout << "Your cart is empty!\n";
        return;
    }

    std::string name;
    inputNameForSale(name, 4);

    Component *toDelete = findInCart(name);
    if (toDelete == nullptr)
    {
        std::cout << "This component is not in your cart!\n";
        return;
    }

    char confirm = getYesNoOption(8, 'Y', 'N'); // Case 8: Confirm delete
    if (confirm != 'Y')
    {
        std::cout << "Removal cancelled.\n";
        return;
    }

    // Remove from cart
    if (toDelete == head && toDelete == tail)
    {
        head = tail = nullptr;
    }
    else if (toDelete == head)
    {
        head = toDelete->getNext();
    }
    else if (toDelete == tail)
    {
        Component *current = head;
        while (current->getNext() != tail)
        {
            current = current->getNext();
        }
        current->setNext(nullptr);
        tail = current;
    }
    else
    {
        Component *current = head;
        while (current->getNext() != toDelete)
        {
            current = current->getNext();
        }
        current->setNext(toDelete->getNext());
    }

    delete toDelete;
    std::cout << "Removed from cart successfully!\n";
}

void Sales::displayCart() const
{
    if (head == nullptr)
    {
        std::cout << "Your cart is empty!\n";
        return;
    }

    // Display header
    for (int i = 0; i < 59; i++)
        std::cout << "_";
    std::cout << "\n";
    std::cout << "|";
    print_space(30);
    std::cout << "|";
    print_space(10);
    std::cout << "|";
    print_space(15);
    std::cout << "|" << std::endl;
    std::cout << "|  Name Component";
    print_space(14);
    std::cout << "| ";
    std::cout << "Quantity ";
    std::cout << "|  Price ($)";
    print_space(4);
    std::cout << "|" << std::endl;
    std::cout << "|";
    print_underScore(30);
    std::cout << "|";
    print_underScore(10);
    std::cout << "|";
    print_underScore(15);
    std::cout << "|" << std::endl;

    Component *current = head;
    while (current != nullptr)
    {
        std::string name = current->getName();
        int qty = current->getQuantity();
        float price = current->getPrice();

        // Format the output to match your original table design
        std::cout << "|";
        print_space(30);
        std::cout << "|";

        print_space(10);
        std::cout << "|";
        print_space(15);
        std::cout << "|\n";
        std::cout << "|  " << name;
        print_space(30 - name.length() - 2);
        std::cout << "| ";
        std::cout << qty;
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
        std::cout << std::fixed << std::setprecision(2) << "| " << price;

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
        std::cout << " |\n";
        std::cout << "|";
        print_underScore(30);
        printf("|");
        print_underScore(10);
        std::cout << "|";
        print_underScore(15);
        std::cout << "|\n";

        current = current->getNext();
    }
    std::cout << "\nTOTAL PRICE: $" << std::fixed << std::setprecision(2) << calculateTotal() << "\n";
}

void Sales::processSale()
{
    // Clear any existing cart items
    Component *current = head;
    while (current != nullptr)
    {
        Component *next = current->getNext();
        delete current;
        current = next;
    }
    head = tail = nullptr;

    std::cout << "=== PROCESS SALE ===\n";

    // Add items to cart
    char option;
    do
    {
        addToCart();
        option = getYesNoOption(9, 'C', 'S'); // Case 9: Continue or Stop
    } while (option == 'C');

    // Review and complete sale
    while (true)
    {
        if (head == nullptr)
        {
            std::cout << "You didn't buy anything!\n";
            break;
        }

        displayCart();
        option = getYesNoOption(11, 'Y', 'N'); // Case 11: Accept purchase?

        if (option == 'Y')
        {
            completeSale();
            break;
        }
        else
        {
            option = getYesNoOption(12, 'A', 'D'); // Case 12: Add or Delete
            if (option == 'A')
            {
                addToCart();
            }
            else
            {
                removeFromCart();
            }
        }
    }
}

void Sales::completeSale()
{
    // Update inventory quantities
    Component *current = head;
    while (current != nullptr)
    {
        inventory->processSaleQuantity(current->getName(), current->getQuantity());
        current = current->getNext();
    }

    std::cout << "\n"
              << std::string(50, '=') << "\n";
    std::cout << "         SALE COMPLETED SUCCESSFULLY!\n";
    std::cout << std::string(50, '=') << "\n";

    displayCart();
    std::cout << "\nThank you for your purchase!\n";

    // Clear cart after sale
    current = head;
    while (current != nullptr)
    {
        Component *next = current->getNext();
        delete current;
        current = next;
    }
    head = tail = nullptr;
}