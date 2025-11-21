#include "auth.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

void Authentication::loadCredentials(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "No existing credentials found.\n";
        initializeCredentials(filename);
        return;
    }

    // Read binary format: 30 bytes username + 30 bytes password
    char user[30], pass[30];
    file.read(user, 30);
    file.read(pass, 30);
    file.close();

    username = std::string(user, 30);
    password = std::string(pass, 30);

    // Remove null padding
    username = username.substr(0, username.find('\0'));
    password = password.substr(0, password.find('\0'));
    
    std::cout << "Credentials loaded successfully.\n";
}

void Authentication::initializeCredentials(const std::string& filename) {
    std::cout << "\n=== INITIALIZE CREDENTIALS ===\n";
    
    // Get username
    std::cout << "Create your username: ";
    std::getline(std::cin, username);
    
    // Validate username length
    while (username.empty() || username.length() > 29) {
        if (username.empty()) {
            std::cout << "Username cannot be empty! Enter username: ";
        } else {
            std::cout << "Username too long (max 29 characters)! Enter username: ";
        }
        std::getline(std::cin, username);
    }

    // Get password with confirmation
    std::string password1, password2;
    do {
        std::cout << "Create your password: ";
        password1 = getPasswordInput(false);
        
        std::cout << "Confirm your password: ";
        password2 = getPasswordInput(true);
        
        if (password1 != password2) {
            std::cout << "Passwords don't match! Please try again.\n";
        } else if (password1.empty()) {
            std::cout << "Password cannot be empty! Please try again.\n";
        }
    } while (password1 != password2 || password1.empty());
    
    password = password1;
    
    // Save to binary file
    saveCredentials(filename);
    std::cout << "\nCredentials initialized successfully!\n";
}

void Authentication::saveCredentials(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create credential file!\n";
        return;
    }

    // Prepare buffers with null padding
    char user[30] = {0};
    char pass[30] = {0};
    
    // Copy username and password to buffers
    strncpy(user, username.c_str(), 29);
    strncpy(pass, password.c_str(), 29);
    
    // Write exactly 30 + 30 bytes
    file.write(user, 30);
    file.write(pass, 30);
    file.close();
}

std::string Authentication::getPasswordInput(bool confirm) {
    std::string password;
#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != 13) { // Enter key
        if (ch == 8 && !password.empty()) { // Backspace
            std::cout << "\b \b";
            password.pop_back();
        } else if (ch != 8) {
            std::cout << '*';
            password.push_back(ch);
        }
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1 && ch != '\n') {
        if (ch == 127 && !password.empty()) { // Backspace
            std::cout << "\b \b";
            password.pop_back();
        } else if (ch != 127) {
            std::cout << '*';
            password.push_back(ch);
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    std::cout << std::endl;
    return password;
}

Authentication::Authentication(const std::string& credentialFile) {
    loadCredentials(credentialFile);
}

bool Authentication::authenticate() {
    int attempts = 3;
    while (attempts > 0) {
        std::cout << "\n=== LOGIN REQUIRED ===\n";
        if (attempts < 3) {
            std::cout << "You have " << attempts << " attempt(s) left!\n";
        }
        
        std::cout << "Username: ";
        std::string inputUser;
        std::getline(std::cin, inputUser);

        std::cout << "Password: ";
        std::string inputPass = getPasswordInput(false);

        if (inputUser == username && inputPass == password) {
            std::cout << "Login successful! Welcome, " << username << "!\n";
            return true;
        } else {
            attempts--;
            std::cout << "Invalid username or password!\n";
        }
    }
    std::cout << "Authentication failed! Access denied.\n";
    return false;
}