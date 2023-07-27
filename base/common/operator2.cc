#include <iostream>

class SalesData {
public:
    SalesData(int isbn = 0, int price = 0) : isbn_(isbn), price_(price) {}
    friend std::ostream &operator<<(std::ostream &os, const SalesData &data);
private:
    int isbn_;
    uint32_t price_;
};

std::ostream &operator<<(std::ostream &os, const SalesData &data)
{
    os << "isbn " << data.isbn_ << ", price " << data.price_;
    return os;
}

int main(int argc, char **argv)
{
    SalesData sd(111111, 100);
    std::cout << sd << std::endl;
}