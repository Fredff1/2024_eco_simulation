#include <iostream>
#include <vector>
#include <memory>
#include <iostream>

using std::vector;
using std::unique_ptr;
using std::cout;
using std::cin;
using std::endl;


typedef enum plantTypeInGrid{
            empty=0,
            peaShooter,
};

class Plant{
public:
    Plant(int health,int cost);
    virtual void act()=0; 
protected:
    int health;
    int cost;
    int damage;
};


class PeaShooter:public Plant{
private:
    
public:
    PeaShooter(int health=300,int cost=100):Plant(health,cost){
        damage=20;
    }
    void act() override{
        //more features to add
    }

};

class Zombie{
public:
    Zombie(int health,int speed);
    virtual void move();
    virtual void attack(std::unique_ptr<Plant>& target);
protected:
    int health;
    int speed;
};

class BasicZombie:Zombie{
public:
    BasicZombie(int health=270,int speed=10):Zombie(health,speed){}
    void move() override{
        //more features to add
    }

    void attack(std::unique_ptr<Plant>& target) override{
        //more features to add
    }
};  





class GameGrid{
private:    
public:
    
    unique_ptr<Plant> plant;
    vector<unique_ptr<Zombie>> zombies;
    plantTypeInGrid plantType;
    int plantPos;
    int zombiePos;
    GameGrid():plant(nullptr),zombies(),plantPos(0),zombiePos(0),plantType(empty){
        cout<<"game ready"<<endl;
    }
     // 移动构造函数
    GameGrid(GameGrid&& other) noexcept = default;

    // 移动赋值运算符
    GameGrid& operator=(GameGrid&& other) noexcept = default;

    // 删除复制构造函数和复制赋值运算符
    GameGrid(const GameGrid&) = delete;
    GameGrid& operator=(const GameGrid&) = delete;
    ~GameGrid(){}
};
class Game{
private:
    bool isRunning;
    int score;
public:
int flag;
    bool init_grid(int flag);
    void printGrid(){
        for(const auto&row:grid){
            for(const auto&target:row){
                switch(target.plantType){
                case empty:
                    cout<<'*'<<' ';
                    break;
                case peaShooter:
                    cout<<'A'<<' ';    
                    break;
                default:
                    cout<<'&'<<' ';
                }
            }
            cout<<endl;
        }
    }
    vector<vector<GameGrid>> grid;
};


bool Game::init_grid(int flag){
    int rows,cols;
    switch(flag){
    case 0:
        rows=5;
        cols=12;//3 for zombies to appear
        grid.resize(rows); // 首先调整外层向量的大小为rows
        for (auto& row : grid) {
            row.resize(cols); // 然后为每行调整大小为cols
        }
        for(auto&row:grid){
            for(auto&target:row){
                //target.plantType=empty;
            }          
        }
        return true;
        break;
    }
    return false;
}






























int main(){
    cout<<"hello world"<<endl;
    cout.flush();
    int a=0;
    scanf("%d",&a);
    cout<<"yes"<<endl;
    printf("hello world");
    std::cout<<"hello world \n";
    Game game;
    game.init_grid(0);
    game.printGrid();
    return 0;
}