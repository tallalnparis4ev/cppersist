#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;
using std::string;

template <typename Key, typename Value>
Cache<Key,Value>::Cache(function<string(Value)> pickle, function<Value(string)> unpickle){
  this->pickle = pickle;
  this->unpickle = unpickle;
}

template <typename Key, typename Value>
optional<Value> Cache<Key,Value>::get(Key key){
    std::ifstream inFile("test.txt");
    if (!inFile) {
        std::cout << "Unable to open file";
        return nullopt;
    }    
    std::ostringstream ss;
    ss << inFile.rdbuf(); 
    string str = ss.str();
    inFile.close();
    return optional<Value>{unpickle(str)};
};

template <typename Key, typename Value>
void Cache<Key,Value>::put(Key key, Value value){
  // return "put key for value";
};





