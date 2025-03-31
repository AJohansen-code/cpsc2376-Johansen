#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

class Employee {
public:
    std::string name;
    int id;

    Employee(std::string name, int id) : name(name), id(id) {}

    virtual double calculateSalary() const = 0;
    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Name: " << name << ", Type: ";
    }

    virtual ~Employee() = default; 
};

class SalariedEmployee : public Employee {
public:
    double monthlySalary;

    SalariedEmployee(std::string name, int id, double monthlySalary)
        : Employee(name, id), monthlySalary(monthlySalary) {}

    double calculateSalary() const override {
        return monthlySalary;
    }

    void displayInfo() const override {
        Employee::displayInfo();
        std::cout << "Salaried, Monthly Salary: $" << std::fixed << std::setprecision(2) << monthlySalary;
    }
};

class HourlyEmployee : public Employee {
public:
    double hourlyRate;
    int hoursWorked;

    HourlyEmployee(std::string name, int id, double hourlyRate, int hoursWorked)
        : Employee(name, id), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    double calculateSalary() const override {
        return hourlyRate * hoursWorked;
    }

    void displayInfo() const override {
        Employee::displayInfo();
        std::cout << "Hourly, Hourly worked: " << hoursWorked 
        << ", Hourly rate: $" << std::fixed << std::setprecision(2) << hourlyRate << ", Salary: $" << calculateSalary();
    }
};

class CommissionEmployee : public Employee {
public:
    double baseSalary;
    double salesAmount;
    double commissionRate;

    CommissionEmployee(std::string name, int id, double baseSalary, double salesAmount, double commissionRate)
        : Employee(name, id), baseSalary(baseSalary) ,salesAmount(salesAmount), commissionRate(commissionRate) {}

    double calculateSalary() const override {
        return baseSalary + (salesAmount * commissionRate);
    }

    void displayInfo() const override {
        Employee::displayInfo();
        std::cout << "Commission, Base Salary: $" << std::fixed << std::setprecision(2) <<baseSalary 
        << ", sales: $ " << std::fixed << std::setprecision(2) << salesAmount << ", Commission Rate: " << (commissionRate * 100) << "%, Salary: $" << calculateSalary();
    }
};

int main() {
    std::ifstream file("employees.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }
   
    std::vector<Employee*> employees;
    std::string line;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type, name;
            int id;
    
            ss >> type >> id >> name;
    
            if (type == "Salaried") {
                double monthlySalary;
                ss >> monthlySalary;
                employees.push_back(new SalariedEmployee(name, id, monthlySalary));
            } else if (type == "Hourly") {
                double hourlyRate;
                int hoursWorked;
                ss >> hourlyRate >> hoursWorked;
                employees.push_back(new HourlyEmployee(name, id, hourlyRate, hoursWorked));
            } else if (type == "Commission") {
                double baseSalary, salesAmount, commissionRate;
                ss >> baseSalary >> salesAmount >> commissionRate;
                employees.push_back(new CommissionEmployee(name, id, baseSalary, salesAmount, commissionRate));
            }
        }
        file.close();

        for (const auto& employee : employees) {
            employee->displayInfo();
            std::cout << std::endl;
        }
        
        for (auto& employee : employees) {
            delete employee; 
        }
        employees.clear(); 

        return 0;
}
    