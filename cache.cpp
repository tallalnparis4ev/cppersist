#include <fstream>
#include <sstream>
#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;
using std::string;

//Constructors
template <typename Ret, typename ...Args>
Cache<Ret,Args...>::Cache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)):
    key(key),
    pickle(pickle),
    unpickle(unpickle)
{}

// template <typename Key, typename Value>
// Cache<Key,Value>::Cache(function<string(Key)> key, function<string(Value)> pickle, function<Value(string)> unpickle):
//   key(key),
//   pickle(pickle),
//   unpickle(unpickle)
// {}
template <typename Ret, typename ...Args>
optional<Ret> Cache<Ret,Args...>::get(Args... args){
    std::ifstream inFile(this->key(args...)+".txt");
    if (!inFile) {
        std::cout << "Unable to open file" << std::endl;
        return nullopt;
    }    
    std::ostringstream ss;
    ss << inFile.rdbuf(); 
    string str = ss.str();
    inFile.close();
    return optional<Ret>{unpickle(str)};
};

template <typename Ret, typename ...Args>
void Cache<Ret,Args...>::put(Args... args, Ret value){
  std::cout << "keying for put: " << this->key(args...) << std::endl;
  std::ofstream myfile(this->key(args...)+".txt");
  myfile << pickle(value);
  myfile.close();
};





