#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>
#include <cstdlib>

//ENV VAR PART
std::string getFolderPathFromEnvVar(const char* envVarName) {
    char* envVarValue = std::getenv(envVarName);
    if (envVarValue == nullptr) {
        std::cerr << "Environment variable " << envVarName << " not set." << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string(envVarValue);
}

std::string getFullFilePath(const std::string& folderPath, const std::string& filename) {
    // Assuming the folder path doesn't end with a separator, add one before appending the filename
    char pathSeparator = '/'; // Adjust this based on your target platform (e.g., '\\' on Windows)
    if (folderPath.back() != pathSeparator) {
        return folderPath + pathSeparator + filename;
    }
    return folderPath + filename;
}

class CSVHandler
{
private:
    std::string filename;
public:
    CSVHandler(const std::string &file) : filename(file) {}

    // Function to read data from the CSV file and store it in a vector
    std::vector<std::pair<std::string, double>> readData()
    {
        std::vector<std::pair<std::string, double>> data;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cout << "Error opening file: " << filename << std::endl;
            return data;
        }

        std::string line, category;
        double price;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::getline(ss, category, ',');
            ss >> price;
            data.push_back(std::make_pair(category, price));
        }

        file.close();
        return data;
    }

    // Function to write data to the CSV file
    void writeData(const std::vector<std::pair<std::string, double>> &data)
    {
        std::ofstream file(filename);

        if (!file.is_open())
        {
            std::cout << "Error opening file: " << filename << std::endl;
            return;
        }

        for (const auto &item : data)
        {
            file << item.first << "," << item.second << std::endl;
        }

        file.close();
        std::cout << "Data written to the file successfully.\n";
    }
};

class Admin;

class Authentication
{
public:
    // static bool isAdmin()
    // {
    //     // std::string choice;
    //     std::auto choice;
    //     std::cout << "Do you want to login as"<<endl;
    //     std::cout << "1. admin"<<endl;
    //     std::cout << "2. user"<<endl;
    //     std::cin >> choice;
    //     if (choice=="admin" or choice == 1)
    //     {
    //         return True;
    //     }
    //     else if (choice=="user" or choice == 2)
    //     {
    //        return False;
    //     }
    //     else{
    //           std::cout<<"pick valid choice"<<endl;
    //     }
    // }

static bool isAdmin()
{
    std::string choice;
    while (true)
    {
        std::cout << "Do you want to login as" << std::endl;
        std::cout << "1. admin" << std::endl;
        std::cout << "2. user" << std::endl;
        std::cin >> choice;
         std::cin.ignore(1000, '\n');

for (char &c : choice)
        {
            c = std::tolower(c);
        }

        if (choice == "admin" || choice == "1")
        {
            return true;
        }
        else if (choice == "user" || choice == "2")
        {
            return false;
        }
        else
        {
            std::cout << "Please type out a valid choice (admin or user).\n";
        }
    }
}

    static bool authenticateAdmin(Admin &admin);
};

class UserBase
{
protected:
    void viewAllCategoriesWithPrices()
    {
         CSVHandler csvHandler("categories.csv");
            std::vector<std::pair<std::string, double>> categories = csvHandler.readData();

            if (categories.empty()) {
                std::cout << "No categories found.\n";
            } else {
                std::cout << "----- All Categories with Prices -----\n";
                for (const auto& item : categories) {
                    std::cout << item.first << ": " << item.second << " Rs/kg\n";
                }
                std::cout << "-------------------------------------\n";
            }
    }

public:
    virtual void greet() const = 0;
    virtual void userMenu() = 0;
};

class Admin : public UserBase
{
private:
    std::string username;
    std::string password;
    std::string categoriesCsvPath;
    std::string transactionsCsvPath;
    std::string adminFilePath;
    void showMenu()
    {
        std::cout << "----------- Admin Menu -----------\n"
                  << "1. Change Password\n"
                  << "2. Add Waste Category\n"
                  << "3. Remove Waste Category\n"
                  << "4. Change Price of Category\n"
                  << "5. View All Categories with Prices\n"
                  << "6. View all Transactions\n"
                  << "7. Quit\n"
                  << "----------------------------------\n";
    }

    // Function to handle the admin menu
    void handleMenu()
    {
        int choice;
        while (true)
        {
            showMenu();
            std::string input;
            std::cout << "Enter your choice: ";
            std::cin >> input;

            // Check if input is an integer
            std::istringstream iss(input);
            if (!(iss >> choice))
            {
                std::cout << "Invalid input. Please enter a valid integer choice.\n";
                std::cin.clear();                                                   // Clear error flags from cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining characters in the input buffer
                continue;                                                           // Continue to next iteration of the loop
            }

            switch (choice)
            {
            case 1:
                changePassword();
                break;
            case 2:
                addCategory();

                break;
            case 3:
                removeCategory();

                break;
            case 4:
                changePrice();
                break;
            case 5:
                viewAllCategoriesWithPrices();
                break;
            case 6:
                viewAllTransactions();
                break;
            case 7:
                std::cout << "Exiting Admin Menu.\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

     void viewAllTransactions() {
            std::string filename = "transactions.csv";
            std::ifstream inputFile(filename);
            if (!inputFile) {
                std::cout << "Error opening file for reading transactions.\n";
                return;
            }

            std::string line;
            std::cout << "----- All Transactions -----\n";
            while (std::getline(inputFile, line)) {
                std::cout << line << '\n';
            }
            std::cout << "-----------------------------\n";

            inputFile.close();
        }

   std::pair<std::string, std::string> readAdminInfo()
    {
        std::ifstream file("adminfile.csv");
        if (!file.is_open())
        {
            std::cout << "Error opening admin file.\n";
            return std::make_pair("", "");
        }

        // Assuming the format of the file is: username,password
        std::string line;
        if (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::getline(iss, username, ',');
            std::getline(iss, password, ',');
        }

        file.close();
        return std::make_pair(username, password);
    }

//   Function to write admin information to the file
    void writeAdminInfo()
    {
        std::ofstream file("adminfile.csv");
        if (!file.is_open())
        {
            std::cout << "Error opening admin file.\n";
            return;
        }

        file << username << "," << password << std::endl;

        file.close();
    }

void changePassword()
{
    readAdminInfo(); // Read admin information from the file

    std::string currentPassword, newPassword, confirmPassword;
    std::cout << "Enter current password: ";
    std::cin >> currentPassword;

    if (currentPassword != password)
    {
        std::cout << "Incorrect current password. Password not changed.\n";
        return;
    }

    std::cout << "Enter new password: ";
    std::cin >> newPassword;

    std::cout << "Confirm new password: ";
    std::cin >> confirmPassword;

    if (newPassword != confirmPassword)
    {
        std::cout << "New password and confirmation do not match. Password not changed.\n";
        return;
    }

    password = newPassword;
    writeAdminInfo(); // Update the new password in the file
    std::cout << "Password changed successfully.\n";
}

    void addCategory()
    {
        viewAllCategoriesWithPrices();
        std::string category;
        double price;

        std::cout << "Enter the new category: ";
        std::cin.ignore(); // Ignore any previous newline character in the input buffer
        std::getline(std::cin, category);

        std::cout << "Enter the price (in Rs/kg): ";
        std::cin >> price;

        std::pair<std::string, double> newCategory(category, price);

        CSVHandler csvHandler("categories.csv");
        std::vector<std::pair<std::string, double>> categories = csvHandler.readData();

        bool categoryExists = false;
        for (const auto &item : categories)
        {
            if (item.first == category)
            {
                categoryExists = true;
                break;
            }
        }

        if (categoryExists)
        {
            std::cout << "Category already exists. Please try adding a different category.\n";
        }
        else
        {
            categories.push_back(newCategory);
            csvHandler.writeData(categories);
            std::cout << "Category added successfully.\n";
        }
    }

    void removeCategory()
    {
        viewAllCategoriesWithPrices();
        std::string categoryToRemove;
        std::cout << "Enter the category to remove: ";
        std::cin.ignore(); // Ignore any previous newline character in the input buffer
        std::getline(std::cin, categoryToRemove);

        CSVHandler csvHandler("categories.csv");
        std::vector<std::pair<std::string, double>> categories = csvHandler.readData();

        auto it = std::remove_if(categories.begin(), categories.end(), [&](const auto &item)
                                 { return item.first == categoryToRemove; });

        if (it != categories.end())
        {
            categories.erase(it, categories.end());
            csvHandler.writeData(categories);
            std::cout << "Category removed successfully.\n";
        }
        else
        {
            std::cout << "Category not found.\n";
        }
    }

    void changePrice()
    {

        viewAllCategoriesWithPrices();
        std::string categoryToChange;
        double newPrice;

        std::cout << "Enter the category to change the price: ";
        std::cin.ignore(); // Ignore any previous newline character in the input buffer
        std::getline(std::cin, categoryToChange);

        std::cout << "Enter the new price (in Rs/kg): ";
        std::cin >> newPrice;

        CSVHandler csvHandler("categories.csv");
        std::vector<std::pair<std::string, double>> categories = csvHandler.readData();

        bool categoryExists = false;
        for (auto &item : categories)
        {
            if (item.first == categoryToChange)
            {
                item.second = newPrice;
                categoryExists = true;
                break;
            }
        }

        if (categoryExists)
        {
            csvHandler.writeData(categories);
            std::cout << "Price for category updated successfully.\n";
        }
        else
        {
            std::cout << "Category not found. Unable to update the price.\n";
        }
    }

    //   void viewAllCategoriesWithPrices() {
    //         CSVHandler csvHandler("categories.csv");
    //         std::vector<std::pair<std::string, double>> categories = csvHandler.readData();

    //         if (categories.empty()) {
    //             std::cout << "No categories found.\n";
    //         } else {
    //             std::cout << "----- All Categories with Prices -----\n";
    //             for (const auto& item : categories) {
    //                 std::cout << item.first << ": " << item.second << " Rs/kg\n";
    //             }
    //             std::cout << "-------------------------------------\n";
    //         }
    //     }

public:
    // Admin(const std::string& u, const std::string& p) : UserBase(), username(u), password(p) {}
    Admin(const std::string& categoriesPath, const std::string& transactionsPath, const std::string& adminPath) : UserBase(), categoriesCsvPath(categoriesPath), transactionsCsvPath(transactionsPath), adminFilePath(adminPath)
    {
       std::pair<std::string, std::string> adminCredentials = readAdminInfo();
        username = adminCredentials.first;
        password = adminCredentials.second;
    }
    void greet() const override
    {
        std::cout << "Hi, Admin!" << std::endl;
    }
    void userMenu() override
    {
        if (Authentication::authenticateAdmin(*this))
        {
            std::cout << "Hi, Admin! You are now logged in.\n";
            handleMenu();
        }
        else
        {
            std::cout << "Invalid username or password for admin.\n";
        }
    }

      Admin() : UserBase()
    {
       std::pair<std::string, std::string> adminCredentials = readAdminInfo();
        username = adminCredentials.first;
        password = adminCredentials.second;
    }
    void adminMenu()
    {
        if (Authentication::authenticateAdmin(*this))
        {
            std::cout << "Hi, Admin! You are now logged in.\n";
            handleMenu();
        }
        else
        {
            std::cout << "Invalid username or password for admin.\n";
        }
    }
    friend class Authentication;

    bool authenticate(const std::string &u, const std::string &p) const
    {
        return (u == username && p == password);
    }
};

bool Authentication::authenticateAdmin(Admin &admin)
{
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    return admin.authenticate(username, password);
}

class User : public UserBase
{
private:
    struct CartItem
    {
        std::string category;
        double weight;
        double price;
    };

    std::vector<CartItem> cart;

    void addToCart()
    {
        viewAllCategoriesWithPrices();
        std::string categoryToAdd;
        double weight;

        std::cout << "Enter the category to add to the cart: ";
        std::cin.ignore(); // Ignore any previous newline character in the input buffer
        std::getline(std::cin, categoryToAdd);

        std::cout << "Enter the weight (in kg): ";
        std::cin >> weight;

        CSVHandler csvHandler("categories.csv");
        std::vector<std::pair<std::string, double>> categories = csvHandler.readData();

        auto it = std::find_if(categories.begin(), categories.end(), [&](const auto &item)
                               { return item.first == categoryToAdd; });

        if (it != categories.end())
        {
            double price = it->second * weight;
            cart.push_back({categoryToAdd, weight, price});
            std::cout << "Category added to the cart successfully.\n";
        }
        else
        {
            std::cout << "Category not found. Unable to add to the cart.\n";
        }
    }

    void removeFromCart()
    {
        viewCart();
        if (cart.empty())
        {
            std::cout << "Cart is empty.\n";
            return;
        }

        std::string categoryToRemove;
        std::cout << "Enter the category to remove from the cart: ";
        std::cin.ignore(); // Ignore any previous newline character in the input buffer
        std::getline(std::cin, categoryToRemove);

        auto it = std::remove_if(cart.begin(), cart.end(), [&](const auto &item)
                                 { return item.category == categoryToRemove; });

        if (it != cart.end())
        {
            cart.erase(it, cart.end());
            std::cout << "Category removed from the cart successfully.\n";
        }
        else
        {
            std::cout << "Category not found in the cart.\n";
        }
    }

    void checkout()
    {
        if (cart.empty())
        {
            std::cout << "Cart is empty. Nothing to checkout.\n";
            return;
        }

        std::string filename = "transactions.csv";
        std::ofstream outputFile(filename, std::ios::app);
        if (!outputFile)
        {
            std::cout << "Error opening file for writing transactions.\n";
            return;
        }

        std::string userName;
        std::cout << "Enter your name: ";
        std::cin.ignore(); // Ignore any previous newline character in the input buffer
        std::getline(std::cin, userName);

        double totalAmount = 0.0;
        std::time_t currentTime = std::time(nullptr);
        std::string timestamp = std::asctime(std::localtime(&currentTime));

        outputFile << "Transaction Time, User Name, Category, Weight (kg), Price (Rs)\n";
        for (const auto &item : cart)
        {
            outputFile << timestamp << ", " << userName << ", " << item.category << ", " << item.weight << ", " << item.price << "\n";
            totalAmount += item.price;
        }

        outputFile << "Total Amount, , , ," << totalAmount << "\n";

        outputFile.close();

        // Empty the cart after successful checkout
        cart.clear();

        std::cout << "Checkout successful .\n";
    }

    void viewCart()
    {
        if (cart.empty())
        {
            std::cout << "Cart is empty.\n";
        }
        else
        {
            double totalPrice = 0.0;
            std::cout << "----- Cart -----\n";
            for (const auto &item : cart)
            {
                std::cout << item.category << ": " << item.weight << " kg - " << item.price << " Rs\n";
                totalPrice += item.price;
            }
            std::cout << "----------------\n";
            std::cout << "Total Price: " << totalPrice << " Rs\n";
        }
    }
    void totalPrice()
    {
        if (cart.empty())
        {
            std::cout << "Price = 0 as cart is empty\n";
        }
        else
        {
            double totalPrice = 0.0;
            for (const auto &item : cart)
            {
                totalPrice += item.price;
            }
            std::cout << "----------------\n";
            std::cout << "Total Price: " << totalPrice << " Rs\n";
        }
    }

public:
    User() {}
    void greet() const override
    {
        std::cout << "Hi, User!" << std::endl;
    }
    void userMenu()
    {
        int choice;
        while (true)
        {
            std::cout << "----------- User Menu -----------\n"
                      << "1. View All Categories with Prices\n"
                      << "2. Add Items to Cart\n"
                      << "3. Remove Items from Cart\n"
                      << "4. View Cart\n"
                      << "5. Total Price\n"
                      << "6. Checkout\n"
                      << "7. Quit\n"
                      << "----------------------------------\n";

            std::string input;
            std::cout << "Enter your choice: ";
            std::cin >> input;

            // Check if input is an integer
            std::istringstream iss(input);
            if (!(iss >> choice))
            {
                std::cout << "Invalid input. Please enter a valid integer choice.\n";
                std::cin.clear();                                                   // Clear error flags from cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining characters in the input buffer
                continue;                                                           // Continue to next iteration of the loop
            }

            switch (choice)
            {
            case 1:
                viewAllCategoriesWithPrices();
                break;
            case 2:
                addToCart();
                break;
            case 3:
                removeFromCart();
                break;
            case 4:
                viewCart();
                break;
            case 5:
                totalPrice();
                break;
            case 6:
                checkout();
                break;
            case 7:
                std::cout << "Exiting User Menu.\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main()
{
   // Retrieve environment variables for file paths
   const char* envVarName = "CSV";
    std::string folderPath = getFolderPathFromEnvVar(envVarName);
    std::string filename1 = "transactions.csv";

    std::string fullFilePath = getFullFilePath(folderPath, filename1);

    std::ifstream inputFile(fullFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input CSV file.\n";
        return 1;
    }

    const char* categoriesCsvPath = std::getenv("CSV");
    const char* transactionsCsvPath = std::getenv("CSV");
    const char* adminFilePath = std::getenv("CSV");

    if (!categoriesCsvPath || !transactionsCsvPath || !adminFilePath)
    {
        std::cout << "Error: One or more environment variables not set for file paths.\n";
        return 1;
    }

    // Admin admin(adminCredentials.first, adminCredentials.second);
    Admin admin(categoriesCsvPath, transactionsCsvPath, adminFilePath);
    bool isAdmin = Authentication::isAdmin();
    if (isAdmin)
    {
        admin.greet();
        admin.userMenu();
    }
    else
    {
        User user;
        user.greet();
        user.userMenu();
    }

    
    return 0;
}
