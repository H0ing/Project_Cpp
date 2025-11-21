#ifndef AUTH_HPP
#define AUTH_HPP

#include <string>

class Authentication {
private:
    std::string username;
    std::string password;
    
    void loadCredentials(const std::string& filename);
    void initializeCredentials(const std::string& filename);
    void saveCredentials(const std::string& filename);
    std::string getPasswordInput(bool confirm = false);
    
public:
    Authentication(const std::string& credentialFile = "mototrack.dat");
    bool authenticate();
};

#endif