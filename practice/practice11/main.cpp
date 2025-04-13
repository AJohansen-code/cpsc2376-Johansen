#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Game {
public:
    std::string title;
    int totalCopies;
    int availableCopies;

    Game(std::string title, int totalCopies) : title(title), totalCopies(totalCopies), availableCopies(totalCopies) {}

    void decreasedAvailableCopies() {
        if (availableCopies > 0) {
            availableCopies--;
        } else {
            std::cout << "No available copies left for " << title << std::endl;
        }
    }

    void increasedAvailableCopies() {
        if (availableCopies < totalCopies) {
            availableCopies++;
        } else {
            std::cout << "All copies of " << title << " are already available." << std::endl;
        }
    }

    int getAvailableCopies() const {
        return availableCopies;
    }
    std::string getTitle() const {
        return title;

    }
};

class Customer {
public:
    std::string name;
    int customerId;

    Customer(std::string name, int id) : name(name), customerId(id) {}

    std::string getName() const{
        return name;
    }
    int getId() const{
    return customerId;
    }
};


class Loan {
public:
    int loanId;
    std::string gameTitle;
    int customerId; 
    std::string dueDate;

    Loan(int id, const std::string& gameTitle, int customerId, std::string dueDate) : loanId(id), gameTitle(gameTitle), customerId(customerId), dueDate(dueDate) {}
    
    int getLoanId() const {
        return loanId;
    }  

    int getCustomerId() const{
        return customerId;
    }

    std::string getDueDate() const {
        return dueDate;
    }

    std::string getGameTitle() const {
        return gameTitle;
    }
};

class Inventory {
public:
    std::vector<Game> games;

    void addGame(const Game& game) {
        games.push_back(game);
    }

    Game* findGame(const std::string& title) {
        for (auto& game : games) {
            if (game.getTitle() == title) {
                return &game;
            }
        }
        return nullptr;
    }

    void listGames() const {
        for (const auto& game : games) {
            std::cout << "Title: " << game.getTitle() << ", Available Copies: " << game.getAvailableCopies() << std::endl;
        }
    }
};

class LendingSystem {
public:
    Inventory inventory;
    std::vector<Customer> customers;
    std::vector<Loan> activeLoans;
    int nextLoanId = 1;
    int nextCustomerId = 1;


    void addGameToInventory(const std::string& title, int totalCopies) {
        inventory.addGame(Game(title, totalCopies));
        std::cout << "\"" << title << "\" added to inventory." << std::endl;
    }

    void addCustomer(const std::string& name){
        customers.push_back(Customer(name, nextCustomerId++));
        std::cout << "Customer \"" << name << "\" added with ID: " << (nextCustomerId - 1) << std::endl;
    }

    void checkoutGame(const int customerId, const std::string& gameTitle, const std::string& dueDate) {
        const Customer* customer = findCustomerById(customerId);
        if (!customer) {
            std::cout << "Customer with ID " << customerId << " not found." << std::endl;
            return;
        }

        Game* game = inventory.findGame(gameTitle);
        if (!game) {
            std::cout << "Game \"" << gameTitle << "\" not found in inventory." << std::endl;
            return;
        }

        if (game->getAvailableCopies() > 0) {
            game->decreasedAvailableCopies();
            activeLoans.push_back(Loan(nextLoanId++, gameTitle, customerId, dueDate));
                std::cout << "Game \"" << gameTitle << "\" checked out to " << customer->getName() << "\" with due date: "<< dueDate << std::endl;
            } else {
            std::cout << "No available copies of \"" << gameTitle << "\"." << std::endl;
        }
    }

    void returnGame(const int loanId) {
        auto it = std::find_if(activeLoans.begin(), activeLoans.end(), [&](const Loan& loan) {  
            return loan.getLoanId() == loanId; 
        });

        if (it != activeLoans.end()) {
            std::string returnedGameTitle = it->getGameTitle();
            Game* game = inventory.findGame(returnedGameTitle);
            if (game) {
                game->increasedAvailableCopies();
                std::cout << "Loan ID" << loanId << ": \"" << returnedGameTitle << "\" returned." << std::endl;
                activeLoans.erase(it);
            } else {
                std::cout << "Error: Could not find game \"" << returnedGameTitle << "\" associated with Loan ID" << loanId << " in inventory." << std::endl;
              }
        } else {
            std::cout << "Error: Loan ID " << loanId << " not found." << std::endl;
        }
    }

    void listAvailableGames() const {
        std::cout << "\n--- Available Games ---" << std::endl;
        for (const auto& game : inventory.games) {
            if (game.getAvailableCopies() > 0) {
                std::cout << "Title: " << game.getTitle() << ", Available Copies: " << game.getAvailableCopies() << std::endl;
            }
        }
    }

    void listBorrowedGames() const {
        std::cout << "\n--- Borrowed Games ---" << std::endl;
        if (activeLoans.empty()) {
            std::cout << "No games are currently borrowed." << std::endl;
            return;
        }
            for (const auto& loan : activeLoans) {
                 const Customer* customer = findCustomerById(loan.customerId);
            if (customer) {
                std::cout << "Loan ID:" << loan.loanId << ", Game: " << loan.getGameTitle() << ", Borrowed by: " << customer->getName() << ", Due Date: " << loan.dueDate << std::endl;
            } else {
                std::cout << "Error: Customer ID " << loan.customerId << " not found for Loan ID " << loan.loanId << std::endl;
            }
        }
    }
    void listAllGames() const {
            inventory.listGames();
    }

private:
    const Customer* findCustomerById(int id) const {
        for (auto& customer : customers) {
            if (customer.getId() == id) {
                return &customer;
            }
        }
        return nullptr;
    }
};

int main() {
    LendingSystem system;
    system.addGameToInventory("The Legend of Zelda: Breath of the Wild", 5);
    system.addGameToInventory("Super Mario Odyssey", 3);
    system.addCustomer("Rebeca");
    system.addCustomer("Zachary");

    system.checkoutGame(1, "The Legend of Zelda: Breath of the Wild", "2023-10-15");
    system.checkoutGame(2, "Super Mario Odyssey", "2023-10-20");

    system.listAvailableGames();
    system.listBorrowedGames();

    system.returnGame(1);
    system.listAvailableGames();
    system.listBorrowedGames();
    
    system.listAllGames();

    return 0;
}