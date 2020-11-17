/*
条款 35：考虑 virtual 函数以外的其它选择
*/

#include <iostream>
#include <functional>

class GameCharacter;
int defaultHealthCalc(const GameCharacter& gc)
{
    std::cout << "defaultHealthCalc()" << std::endl;
}

/* 使用 std::function 代替古典的 Strategy 模式 */

class GameCharacter {
public:
    typedef std::function<int (const GameCharacter&)> HealthCalcFunc;

    explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc)
            : healthFunc(hcf) { 
        std::cout << "GameCharacter::constructor" << std::endl;
    }

    int healthValue() const { 
        return healthFunc(*this); 
    }

private:
    HealthCalcFunc healthFunc;
};

class EyeCandyCharacter : public GameCharacter {
public:
    explicit EyeCandyCharacter(HealthCalcFunc hcf = defaultHealthCalc)
            : GameCharacter(hcf) {
        std::cout << "EyeCandyCharacter::constructor" << std::endl;
    }
};

struct HealthCalculator {
    int operator()(const GameCharacter& g) {
        std::cout << "HealthCalculator::operator()" << std::endl;
        return 0;
    } 
};

class GameLevel {
public:
    int health(const GameCharacter& g) const {
        std::cout << "GameLevel::health()" << std::endl;
        return 0;
    }
};


int main(void)
{
    /* 是编译器的问题吗？写成 
       EyeCandyCharacter ecc(HealthCalculator()) 就有问题 */   

    std::function<int (const GameCharacter&)> f = HealthCalculator();    
    EyeCandyCharacter ecc(f);
    ecc.healthValue();

    GameLevel currentLevel;
    std::function<int (const GameCharacter&)> ff = 
            std::bind(&GameLevel::health, currentLevel, std::placeholders::_1); 
    EyeCandyCharacter edg(ff);
    edg.healthValue();

    return 0;
}


