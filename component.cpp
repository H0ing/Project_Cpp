#include "component.hpp"
#include <algorithm>
#include <cctype>

Component::Component(const std::string &name, int quantity, float price)
    : name(name), quantity(quantity), price(price), next(nullptr) {}

std::string Component::getName() const { return name; }
void Component::setName(const std::string &newName) { name = newName; }
int Component::getQuantity() const { return quantity; }
void Component::setQuantity(int newQuantity) { quantity = newQuantity; }
float Component::getPrice() const { return price; }
void Component::setPrice(float newPrice) { price = newPrice; }
Component *Component::getNext() const { return next; }
void Component::setNext(Component *nextNode) { next = nextNode; }

bool Component::isValidName(const std::string &name)
{
    if (name.empty() || name.length() > 25)
        return false;
    return std::all_of(name.begin(), name.end(), [](char c)
                       { return std::isalpha(c) || std::isspace(c); });
}

bool Component::isValidQuantity(int quantity)
{
    return quantity >= 0 && quantity <= 32767;
}

bool Component::isValidPrice(float price)
{
    return price >= 0.0f && price < 1000000.0f;
}

std::string Component::toUpperCase(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}