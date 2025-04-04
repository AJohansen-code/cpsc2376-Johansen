#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>

class Shape {
public:
    virtual double getArea() const = 0;
    virtual ~Shape() = default;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double getArea() const override {
        return width_ * height_;
    }
private:
    double width_;
    double height_;
};

class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
private:
    double radius_;
};

void printAllAreas(const std::vector<std::unique_ptr<Shape>>& shapes) {
    for (const auto& shape : shapes) {
        std::cout << "Area: " << std::setprecision(6) << shape->getArea() << std::endl;
    }
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    std::ifstream file("shapes.txt");

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string shapeType;
        ss >> shapeType;

        if (shapeType == "rectangle") {
            double width, height;
            ss >> width >> height;
            shapes.push_back(std::make_unique<Rectangle>(width, height));
        } else if (shapeType == "circle") {
            double radius;
            ss >> radius;
            shapes.push_back(std::make_unique<Circle>(radius));
    } else {
        std::cerr << "Error: Invalid circle format in file: " << shapeType << std::endl;
    }
}
file.close();
    printAllAreas(shapes);

    return 0;
}