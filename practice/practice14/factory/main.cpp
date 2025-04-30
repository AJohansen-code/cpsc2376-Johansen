#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>

class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {
        if (r <= 0) {
            throw std::invalid_argument("Radius must be positive.");
        }
    }

    double area() const override {
        return M_PI * radius * radius;
    }
};

class Square : public Shape {
private:
    double side;

public:
    Square(double s) : side(s) {}
    double area() const override {
        return side * side;
    }
};

class Rectangle : public Shape {
private:
    double width, length;

public:
    Rectangle(double w, double l) : width(w), length(l) {}
    double area() const override {
        return width * length;
    }
};

class ShapeFactory {
public:
    static std::unique_ptr<Shape> createShape(const std::string& shapeType, const std::string& dimensions) {
        std::istringstream iss(dimensions);
        if (shapeType == "Circle") {
            double radius;
            iss >> radius;
            return std::make_unique<Circle>(radius);
        } else if (shapeType == "Square") {
            double side;
            iss >> side;
            return std::make_unique<Square>(side);
        } else if (shapeType == "Rectangle") {
            double width, length;
            iss >> width >> length;
            return std::make_unique<Rectangle>(width, length);
        } else {
            throw std::invalid_argument("Unknown shape type.");
        }
    }
};

int main() {
    std::ifstream inputFile("shapes.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    double totalArea = 0.0;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string shapeType;
        std::string dimensions;
        iss >> shapeType >> std::ws;
        std::getline(iss, dimensions);
    
        try {
            std::unique_ptr<Shape> shape = ShapeFactory::createShape(shapeType, dimensions);
            totalArea += shape->area();
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error processing line \"" << line << "\" :" << e.what() << std::endl;
        }
    }

    inputFile.close();
    std::cout << "Total area of all shapes: " << totalArea << std::endl;

    return 0;
}