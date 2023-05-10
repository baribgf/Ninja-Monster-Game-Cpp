#include <iostream>
#include <random>
#include <string>
#include <unistd.h>

using namespace std;

int randint(int min, int max)
{
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(eng);
}

string input(string message)
{
    string text;
    cout << message;
    getline(cin, text);
    return text;
}

class Actor
{
public:
    string name;
    int health = 100;
    int power;
    int speed;
    int maxHits = 4;
    Actor* enemy;
    Actor(string name, int power, int speed) :
        name(name),
        power(power),
        speed(speed)
    {}
    
    void setEnemy(Actor* enemy)
    {
        this->enemy = enemy;
    }
    
    void attack()
    {
        cout << "'" + this->name + "'" << " is attacking " << "'" + this->enemy->name + "'";
        this->enemy->decreaseHealth((100.0 / this->maxHits) * (this->power / 100.0));
    }
    
    void increaseHealth(int value)
    {
        int newHealth = this->health + value;
        this->health = min(100, newHealth);
    }
    void decreaseHealth(int value)
    {
        int newHealth = this->health - value;
        this->health = max(newHealth, 0);
    }
    int test()
    {
        return 2004;
    }
};

class Game
{
private:
    Actor* ninja;
    Actor* monster;
    Actor** player;
    int thresh;

public:
    void init()
    {
        this->ninja = new Actor("Ninja", 65, 90);
        this->monster = new Actor("Monster", 85, 70);
        
        this->ninja->setEnemy(this->monster);
        this->monster->setEnemy(this->ninja);
        
        this->thresh = 50 - abs(this->ninja->speed - this->monster->speed);
    }

    int playerEndState()
    {
        int ninjaHealth = this->ninja->health;
        int monsterHealth = this->monster->health;
        if (ninjaHealth > monsterHealth)
        {
            if (*(this->player) == this->ninja)
                return 1;
            return -1;
        }
        else if (ninjaHealth < monsterHealth)
        {
            if (*(this->player) == this->monster)
                return 1;
            return -1;
        }
        else return 0;
    }
    
    void start()
    {
        string choice = input("Choose your player [N: Ninja, M: Monster]: ");
        if (choice == "N" || choice == "n")
            this->player = &(this->ninja);
        else if (choice == "M" || choice == "m")
            this->player = &(this->monster);
        else
        {
            cout << "Invalid choice !" << endl;
            start();
            return;
        }
        
        init();
        cout << "\nGame starting . ." << endl;
        while (!gameOver())
        {
            Actor* attacker;
            int randThresh = randint(0, 100);
            if (this->ninja->speed > this->monster->speed)
            {
                if (randThresh >= this->thresh)
                    attacker = this->ninja;
                else
                    attacker = this->monster;
            }
            else
            {
                if (randThresh >= this->thresh)
                    attacker = this->monster;
                else
                    attacker = this->ninja;
            }

            attacker->attack();
            cout << " | Your health: " << (*(this->player))->health
                << ", Enemy health: " << (*(this->player))->enemy->health
                << endl;

            usleep(500000);
        }

        string repChoice = input("Do you want to replay? [Y: for yes]: ");
        if (repChoice == "Y" || repChoice == "y")
            start();
        else
            cout << "Good bye !" << endl;
    }
    
    bool gameOver()
    {
        int ninjaHealth = this->ninja->health;
        int monsterHealth = this->monster->health;
        if (ninjaHealth <= 0 || monsterHealth <= 0)
        {
            cout << "> Game Over! ==> ";
            switch (this->playerEndState())
            {
                case 1:
                    cout << "You win !" << endl;
                    break;
                case -1:
                    cout << "You lose !" << endl;
                    break;
                case 0:
                    cout << "The score is even" << endl;
                    break;
            }
            return true;
        }
        return false;
    }
};

int main()
{
    Game newGame;
    newGame.start();
}
