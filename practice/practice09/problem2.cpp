#include <iostream>
#include <limits>

struct Point {
    float x;
    float y;
};

Point calcMidpoint(const Point* p1, const Point* ps){
    return Point{(p1->x + ps->x) / 2.0f, (p1->y + ps->y) / 2.0f};
}

int main() {
    Point p1{0.0f, 0.0f};
    Point p2{0.0f, 0.0f};
    Point* pMidpoint{nullptr};

    std::string input;
    while (true) {
        std::cout << "Enter the coordinates of the first point (x y) or 'done' to quit: ";
        std::cin >> input;

        if (input == "done") {
            return 0;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!(std::cin >> p1.x >> p1.y)) {
                std::cout << "Invalid input. Please enter numeric values for x and y." << std::endl;
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        }
        std::cout << "Enter the coordinates of the second point (x y) or 'done' to quit: ";
        std::cin >> input;

        if (input == "done") {
            return 0;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!(std::cin >> p2.x >> p2.y)) {
                std::cout << "Invalid input. Please enter numeric values for x and y." << std::endl;
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            std::cout << "Debug: p1(" << p1.x << ", " << p1.y << "), p2(" << p2.x << ", " << p2.y << ")" << std::endl;
        }

        pMidpoint = new Point(calcMidpoint(&p1, &p2));
        std::cout << "Midpoint: (" << pMidpoint->x << ", " << pMidpoint->y << ")" << std::endl;
        delete pMidpoint;
    }

    return 0;
}  
