#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip> 

using namespace std;

const int GSIZE = 10; // the single edge size of the grid
const int ROBOTS = 5; // the initial number of the each robot type
const int O = 0; // optimusprime robot type
const int RP = 1; // robocop robot type
const int RA = 2; // roomba robot type
const int B = 3; // bulldozer robot type
const int K = 4; // kamikaze robot type

class Robot;
class Humanic;
class Optimusprime;
class Robocop;
class Roomba;
class Bulldozer;
class Kamikaze;

class Grid
{
    friend class Robot;
    friend class Humanic;
    friend class Optimusprime;
    friend class Robocop;
    friend class Roomba;
    friend class Bulldozer;
    friend class Kamikaze;
    public:
        Grid (); // the default constructor of the grid matrix
        ~Grid (); // the destructor of the grid matrix
        static int getCount (); // returns the total number of the robots
        void Display ();
        Robot * getAt (int x, int y); // returns the pointer pointing to the specific coordinate
        void moveLoop (Robot* rbt); // the robot moves until fights with another robot
        void fight (int fromX, int fromY, int toX, int toY);
        void moveRobot (int fromX, int fromY, int toX, int toY);
        void setAt (int x, int y, Robot *rbt); // sets the specific coordinates pointed by the pointer
        void simulation (); // each element of the grid matrix is circulated
    private:
        static int count; // the total number of the robots
        Robot* grid [GSIZE][GSIZE]; // the grid matrix
};
class Robot 
{
    friend class Grid;
    public:
        Robot (); // the default constructor of the robot
        Robot (Grid *grd, int x, int y); // the parameterized constructor of the robot
        ~Robot (); // the destructor of the robot
        int getStrength () const; // returns the strength of the robot
        int getHitpoints () const; // returns the hitpoints of the robot
        string getName () const; // returns the name of the robot
        void setType (int new_type); // sets the type of teh robot
        void setStrength (int new_strength); // sets the new strength of the robot
        void setHitpoints (int new_Hitpoints); // sets the new hitpoints of the robot
        void setName (string new_name); // sets the name of the robot
        void move (); // function that moves robots
        void hitMessage (Robot* rbt, bool& isDead); // function that prints hit messages
        virtual int getType () = 0; // returns the type of the robot
        virtual int getDamage () = 0; // returns the damage dealt by the robot
    protected:
        string name; // the name of the robot
        int x, y; // the coordinates
        bool moved; // move flag
        int type; // the robot type
        int strength; // the strength
        int hitpoints; // the hitpoints
        Grid *grid; // grid pointers
};
int Grid :: count = ROBOTS * 5;

Grid :: Grid () // the default constructor of the grid matrix
{
    int i, j;
    for (i = 0; i < GSIZE; i++)
    {
        for (j = 0; j < GSIZE; j++)
        {
            grid [i][j] = nullptr; // nullptr is assigned to each element of the grid matrix
        }
    }
}
Grid :: ~Grid () // the destructor of the grid matrix
{
    int i, j;
    for (i = 0; i < GSIZE; i++)
    {
        for (j = 0; j < GSIZE; j++)
        {
            if (grid [i][j] != nullptr)
            {
                delete (grid [i][j]); // deletes the each element of the grid
                grid [i][j] = nullptr;
            }
        }
    }
}
int Grid :: getCount () // returns the total number of the robots
{
    return count; 
}
Robot* Grid :: getAt (int x, int y) // returns the pointer pointing to the specific coordinate
{
    if ((x >= 0) && (x < GSIZE) && (y >= 0) && (y < GSIZE)) 
    {
        return grid [x][y];
    }
    return nullptr;
}
void Grid :: moveLoop (Robot *rbt)
{
    while (rbt -> moved == false) // unless the robot fights
    {
        if (count == 1) // if one robot left
        {   
            break;
        }
        rbt -> move (); // calls the move function
    }
}
void Grid :: fight (int fromX, int fromY, int toX, int toY)
{
    grid [fromX][fromY] -> moved = true; // sets the move flag to true
    bool isDeadAt = false; // is attacker dead
    bool isDeadVic = false; // is victim dead
    while (true) // until one of the two robots dies
    {    
        grid [fromX][fromY] -> hitMessage (grid [toX] [toY], isDeadVic); // calls the hitmessage function
        if (grid [fromX][fromY] -> getType () == K) // if the attacker is a kamikaze robot
        {
            cout << endl << "The " << grid [fromX][fromY] -> getName () << " is died" << endl; // die message
            grid [fromX][fromY] = nullptr; // sets the nullptr to the kamikaze's old position
            count--; // decrements the total number of the robots
            break; // breaks the infinite loop
        }
        if (isDeadVic == true) // if the victim is dead
        {
            if (grid [fromX][fromY] != nullptr)
            {
                grid [toX][toY] = grid [fromX][fromY]; // sets the attacker to the deleted position
                grid [fromX][fromY] = nullptr; // sets the nullptr to the attacker's old position
            }
            count--; // decrements the total number of the robots
            break; // breaks the infinite loop
        }
        grid [toX][toY] -> hitMessage (grid [fromX][fromY], isDeadAt); // calls the hitmessage function
        if (grid [toX][toY] -> getType () == K) // if the victim is a kamikaze robot
        {
            cout << endl << "The " << grid [toX] [toY] -> getName () << " is died" << endl; // die message
            grid [toX][toY] = grid [fromX][fromY]; // sets the attacker to kamikaze's position
            grid [fromX][fromY] = nullptr; // sets the nullptr to the kamikaze'S old position
            count--; // decrements the total number of the robots
            break; // breaks the infinite loop
        }
        if (isDeadAt == true) // if the attacker is dead
        {
            if (grid [fromX][fromY] != nullptr)
            {
                grid [fromX][fromY] = nullptr; // sets the nullptr to the attacker's position
            }
            count--; // decrements the total number of the robots
            break; // breaks the infinite loop
        }
    }
}
void Grid :: moveRobot (int fromX, int fromY, int toX, int toY) 
{
    if ((fromX >= 0 && fromX < GSIZE) && (fromY >= 0 && fromY < GSIZE) && (toX >= 0 && toX < GSIZE) && (toY >= 0 && toY < GSIZE)) 
    {
        if (grid [toX][toY] == nullptr) 
        {
            grid [toX][toY] = grid [fromX][fromY];
            grid [fromX][fromY] = nullptr;
        } 
        else 
        {
            fight (fromX, fromY, toX, toY);
        }
    }
}
void Grid :: setAt (int x, int y, Robot *rbt) // sets the specific coordinates pointed by the pointer
{
    grid [x][y] = rbt;
}
void Grid :: Display () 
{
    cout << endl << endl;
    for (int i = 0; i < GSIZE; i++) 
    {
        for (int j = 0; j < GSIZE; j++) 
        {
            if (grid [i][j] == nullptr) 
            {
                cout << "     ";
            } 
            else 
            {
                string name = grid [i][j] -> getName ();
                if (name.size () >= 4) 
                {
                    string short_name = name.substr (0, 1) + name.substr (name.size () - 3, 3);
                    cout << short_name << " ";
                } 
                else 
                {
                    cout << name << "     ";
                }
            }
        }
        cout << endl;
    }
}
void Grid :: simulation () // each element of the grid matrix is circulated
{
    int i, j;
    for (i = 0; i < GSIZE; i++) // navigates the grid matrix
    {
        for (j = 0; j < GSIZE; j++)
        {
            if (grid [i][j] != nullptr) 
            {
                grid [i][j] -> moved = false;  // sets the move flag of all objects on the grid matrix to false
            }
        }
    }
    bool breakLoops = false;
    for (i = 0; i < GSIZE && !breakLoops; i++) // navigates the grid matrix
    {
        for (j = 0; j < GSIZE && !breakLoops ; j++)
        {
            if (grid [i][j] != nullptr) 
            {   
                moveLoop (grid [i][j]);
                if (count == 1)
                {
                    breakLoops = true; // it breaks the loops
                }
            }
        }
    }
    if (count == 1)
    {
        for (i = 0; i < GSIZE; i++) // navigates the grid matrix
        {
            for (j = 0; j < GSIZE; j++)
            {
                if (grid [i][j] != nullptr) // if it finds the winner robot
                {
                    cout << endl << "The " << grid [i][j] -> name << " wins" << endl
                        << "Game over!" << endl;

                    grid [i][j] = nullptr; // sets the nullptr to that location
                    return; // exits the function
                }
            }
        } 
    }
}
Robot :: Robot () // the default constructor of the robot
{ // all variables are assigned to default values
    grid = nullptr; 
    moved = false; 
    strength = 0; 
    hitpoints = 0;
    name = " ";
    type = -1;
    x = 0;
    y = 0;
}
Robot :: Robot (Grid *grd, int x, int y) // the parameterized constructor of the robot
{ // all variables are assigned to specific values
    this -> grid = grd;
    moved = false;
    strength = 0;
    hitpoints = 0;
    name = " ";
    type = -1;
    this -> x = x;
    this -> y = y;
    grd -> setAt (x, y, this);
}
Robot :: ~Robot () // the destructor of the robot
{
    if (grid != nullptr)
    {
        delete grid; // deletes the pointer
        grid = nullptr;
    }
}
int Robot :: getStrength () const // returns the strength of the robot
{
    return strength;
}
int Robot :: getHitpoints () const // returns the hitpoints of the robot
{
    return hitpoints;
}
string Robot :: getName () const // returns the name of the robot
{
    return name;
}
void Robot :: setType (int new_type) // sets the type of the robot
{
    type = new_type;
}
void Robot :: setStrength (int new_strength) // sets the new strength of the robot
{
    strength = new_strength;
}
void Robot :: setHitpoints (int new_hitpoints) // sets the new hitpoints of the robot
{
    hitpoints = new_hitpoints;
}
void Robot :: setName (string new_name) // sets the name of the robot
{
    name = new_name;
}
void Robot :: move () // function that moves robots
{ 
    int dir = rand () % 4; // determine direction
    int newX = x, newY = y;

    switch (dir) 
    {
        case 0: 
            newY--; // down
            break;
        case 1: // right 
            newX++; 
            break;
        case 2: // up
            newY++; 
            break;
        case 3: // left
            newX--; 
            break;
    }
    if ((newX >= 0 && newX < GSIZE) && (newY >= 0 && newY < GSIZE)) 
    {
        grid -> moveRobot (x, y, newX, newY);
        x = newX;
        y = newY;
    }
}
void Robot :: hitMessage (Robot* rbt, bool& isDead) // function that prints hit messages
{
    int new_hitpoints = 0;
    int damage = getDamage ();
    cout << endl << getName () << "(" << getHitpoints () << ") hits "
         << rbt -> getName () << "(" << rbt -> getHitpoints () << ") with " << damage << endl;
    

    new_hitpoints = (rbt -> getHitpoints ()) - damage; // calculates the new hitpoints
    rbt -> setHitpoints (new_hitpoints);
    if (new_hitpoints > 0)
    {
        cout << endl << "The new hitpoints of " << rbt -> getName () << " is " << rbt -> getHitpoints () << endl;
    }
    else // if hitpoints is less then or equal to zero
    {
        cout << endl << "The " << rbt -> getName () << " is died" << endl;
        isDead = true; // then the robot is dead
    }
}
class Humanic : public Robot
{
    friend class Grid;
    public:
        Humanic (); // the default constructor of the humanic type robot
        Humanic (Grid *grd, int x, int y); // the parameterized constructor of the humanic type robot
        ~Humanic (); // the destructor of the humanic type robot
        int getDamage (); // returns the damage dealt by the humanic type robot
};
Humanic :: Humanic () : Robot () // the default constructor of the humanic type robot
{
}
Humanic :: Humanic (Grid *grd, int x, int y) : Robot (grd, x, y) // the parameterized constructor of the humanic type robot
{
}
Humanic :: ~Humanic () // the destructor of the humanic type robot
{
}
int Humanic :: getDamage () // returns the damage dealt by the humanic type robot
{
    int damage = 0;
    damage = (rand () % strength) + 1; // the damage is less then strength and greater then zero
    if (rand () % 10 == 0) // 10% chance of dealing another 50 damage
    {
        damage += 50;
    }
    return damage;
}
class Roomba : public Robot
{
    friend class Grid;
    public:
        Roomba (); // the default constructor of the roomba type robot
        Roomba (Grid *grd, int x, int y); // the parameterized constructor of the roomba type robot
        ~Roomba (); // the destructor of the roomba type robot
        int getDamage (); // returns the damage dealt by the roomba type robot
        int getType (); // returns the roomba type robot
};
Roomba :: Roomba () : Robot () // the default constructor of the roomba type robot
{
}
Roomba :: Roomba (Grid *grd, int x, int y) : Robot (grd, x, y) // the parameterized constructor of the roomba type robot
{
}
Roomba :: ~Roomba () // the destructor of the roomba type robot
{
}
int Roomba :: getDamage () // returns the damage dealt by the rommba type robot
{
    int damage = 0;
    damage = (rand () % strength) + 1; // the damage is less then strength and greater then zero
    damage += (rand () % strength) + 1; // it does attack twice because it's fast
    return damage;
}
int Roomba :: getType() // returns the roomba type robot
{
    return RA;
}
class Bulldozer : public Robot
{
    friend class Grid;
    public:
        Bulldozer (); // the default constructor of the bulldozer type robot
        Bulldozer (Grid *grd, int x, int y); // the parameterized constructor of the bulldozer type robot
        ~Bulldozer (); // the destructor of the bulldozer type robot
        int getDamage (); // returns the damage dealt by the bulldozer type robot
        int getType (); // returns the bulldozer type robot
};
Bulldozer :: Bulldozer () : Robot () // the default constructor of the bulldozer type robot
{
}
Bulldozer :: Bulldozer (Grid *grd, int x, int y) : Robot (grd, x, y) // the parameterized constructor of the bulldozer type robot
{
}
Bulldozer :: ~Bulldozer () // the destructor of the bulldozer type robot
{
}
int Bulldozer :: getDamage () // returns the damage dealt by the bulldozer type robot
{
    int damage = 0;
    damage = (rand () % strength) + 1; // the damage is less then strength and greater then zero
    return damage;;
}
int Bulldozer :: getType () // returns the bulldozer type robot
{
    return B;
}
class Kamikaze : public Robot
{
    friend class Grid;
    public:
        Kamikaze (); // the default constructor of the kamikaze type robot
        Kamikaze (Grid *grd, int x, int y); // the parameterized constructor of the kamikaze type robot
        ~Kamikaze (); // the destructor of the kamikaze type robot
        int getDamage (); // returns the damage dealt by the kamikaze type robot
        int getType (); // returns the kamikaze type robot
};
Kamikaze :: Kamikaze () : Robot () // the default constructor of the kamikaze type robot
{
}
Kamikaze :: Kamikaze (Grid *grd, int x, int y) : Robot (grd, x, y) // the parameterized constructor of the kamikaze type robot
{
}
Kamikaze :: ~Kamikaze () // the destructor of the kamikaze type robot
{
}
int Kamikaze :: getDamage () // returns the damage dealt by the kamikaze type robot
{
    int damage = 0;
    damage = hitpoints; // it deals damage equal to hitpoint
    return damage;
}
int Kamikaze :: getType () // returns the kamikaze type robot
{
    return K;
}
class Optimusprime : public Humanic
{
    friend class Grid;
    public:
        Optimusprime (); // the default constructor of the optimusprime type robot
        Optimusprime (Grid *grd, int x, int y); // the parameterized constructor of the optimusprime type robot
        ~Optimusprime (); // the destructor of the optimusprime type robot
        int getDamage (); // returns the damage dealt by the optimusprime type robot
        int getType (); // returns the optimusprime type robot
};
Optimusprime :: Optimusprime () : Humanic () // the default constructor of the optimusprime type robot
{
}
Optimusprime :: Optimusprime (Grid *grd, int x, int y) : Humanic (grd, x, y) // the parameterized constructor of the optimusprime type robot
{
}
Optimusprime :: ~Optimusprime () // the destructor of the optimusprime type robot
{
}
int Optimusprime :: getDamage () // returns the damage dealt by the optimusprime type robot
{
    int damage = 0;
    damage = (rand () % strength) + 1; // the damage is less then strength and greater then zero
    if (rand () % 10 == 0) // 10% chance of dealing another 50 damage
    {
        damage += 50;
    }
    if (rand () % 100 < 15) // 15% chance to deal double damage
    {
        damage *= 2;
    }
    return damage;
}
int Optimusprime :: getType () // returns the optimusprime type robot
{
    return O;
}
class Robocop : public Humanic
{
    friend class Grid;
    public:
        Robocop (); // the defult constructor of the robocop type robot
        Robocop (Grid *grd, int x, int y); // the parameterized constructor of the robocop type robot
        ~Robocop (); // the destructor of the robocop type robot
        int getType (); // returns the robocop type robot
};
Robocop :: Robocop () : Humanic () // the defult constructor of the robocop type robot
{
}
Robocop :: Robocop (Grid *grd, int x, int y) : Humanic (grd, x, y) // the parameterized constructor of the robocop type robot
{
}
Robocop :: ~Robocop () // the destructor of the robocop type robot
{

}
int Robocop :: getType () // returns the robocop type robot
{
    return RP;
}
int main()
{
    string s;
    srand (time (NULL));
    Grid g;
    int optimusprime_count = 0;
    while (optimusprime_count < ROBOTS) // creates any number of optimusprime type robots
    {
        int x = rand () % GSIZE; // random number for x coordinate
        int y = rand () % GSIZE; // random number for y coordinate
        if (g.getAt (x, y) == nullptr) // if the coordinates are null
        {
            Optimusprime *optimusprime = new Optimusprime (&g, x, y); // dynamically allocated
            // sets the initial values
            optimusprime -> setStrength (100);
            optimusprime -> setHitpoints (100);
            optimusprime -> setType (O);
            optimusprime -> setName ("optimusprime_" + to_string (optimusprime_count));
            optimusprime_count++;
        }
    }
    int robocop_count = 0;
    while (robocop_count < ROBOTS) // creates any number of robocop type robots
    {
        int x = rand () % GSIZE; // random number for x coordinate
        int y = rand () % GSIZE; // random number for y coordinate
        if (g.getAt (x, y) == nullptr) // if the coordinates are null
        {
            Robocop *robocop = new Robocop (&g, x, y); // dynamically allocated
            // sets the initial values
            robocop -> setStrength (30);
            robocop -> setHitpoints (40);
            robocop -> setType (RP);
            robocop -> setName ("robocop_" + to_string (robocop_count));
            robocop_count++;
        }
    }
    int roomba_count = 0;
    while (roomba_count < ROBOTS) // creates any number of roomba type robots
    {
        int x = rand () % GSIZE; // random number for x coordinate
        int y = rand () % GSIZE; // random number for y coordinate
        if (g.getAt (x, y) == nullptr) // if the coordinates are null
        {
            Roomba *roomba = new Roomba (&g, x, y); // dynamically allocated
            // sets the initial values
            roomba -> setStrength (3);
            roomba -> setHitpoints (10);
            roomba -> setType (RA);
            roomba -> setName ("roomba_" + to_string (roomba_count));
           
            roomba_count++;
        }
    }
    int bulldozer_count = 0;
    while (bulldozer_count < ROBOTS) // creates any number of bulldozer type robots
    {
        int x = rand () % GSIZE; // random number for x coordinate
        int y = rand () % GSIZE; // random number for y coordinate
        if (g.getAt (x, y) == nullptr) // if the coordinates are null
        {
            Bulldozer *bulldozer = new Bulldozer (&g, x, y); // dynamically allocated
            // sets the initial values
            bulldozer -> setStrength (50);
            bulldozer -> setHitpoints (200);
            bulldozer -> setType (B);
            bulldozer -> setName ("bulldozer_" + to_string (bulldozer_count));
            bulldozer_count++;
        }
    }
    int kamikaze_count = 0;
    while (kamikaze_count < ROBOTS) // creates any number of kamikaze type robots
    { 
        int x = rand () % GSIZE; // random number for x coordinate
        int y = rand () % GSIZE; // random number for y coordinate
        if (g.getAt (x, y) == nullptr) // if the coordinates are null
        {
            Kamikaze *kamikaze = new Kamikaze (&g, x, y); // dynamically allocated
            // sets the initial values
            kamikaze -> setStrength (10);
            kamikaze -> setHitpoints (10);
            kamikaze -> setType (K);
            kamikaze -> setName ("kamikaze_" + to_string (kamikaze_count));
            kamikaze_count++;
        }
    }
    while (true)
    {
        g.simulation ();       
        if (g.getCount () == 1) // if one robot survives
        {
            break; // breaks the infinite loop
        }
    }
    return 0;
}