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

class PrimeGenerator : public ArgGenerator{
  public:
    using ArgGenerator::ArgGenerator;
    int max;
    string getNext(){
      return "hello";
    };
};
