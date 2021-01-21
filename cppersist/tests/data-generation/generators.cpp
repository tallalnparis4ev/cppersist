#include <vector>
using std::string;

class ArgGenerator{
  public: 
    ArgGenerator(int seed){
      srand (seed);
    };
    ArgGenerator(){
      srand (time(NULL));
    };
    string getNext();
    bool hasNext();
};

class StringGenerator : public ArgGenerator{
  public:
    using ArgGenerator::ArgGenerator;
    const char lowercase[27] = "abcdefghijklmnopqrstuvwxyz";
    int max = 2;
    string getNext(){
      return string() + lowercase[rand()%26] + lowercase[rand()%26] + lowercase[rand()%26];
    };
};

class NumberGenerator : public ArgGenerator{
  public:
    using ArgGenerator::ArgGenerator;
    int max;
    string getNext(){
      return to_string((rand()%max)+1);
    };
};
