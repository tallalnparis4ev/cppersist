#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;

template <typename Key, typename Value>
Cache<Key,Value>::Cache(function<Value(String)> pickle, function<String(Value)> unpickle){
  this->pickle = pickle;
  this->unpickle = unpickle;
}

template <typename Key, typename Value>
optional<Value> Cache<Key,Value>::get(Key key){
    ifstream inFile("test.txt");
    if (!inFile) {
        cout << "Unable to open file";
        return nullopt;
    }    
    ostringstream ss;
    ss << inFile.rdbuf(); 
    String str = ss.str();
    inFile.close();
    return str;
};

template <typename Key, typename Value>
void Cache<Key,Value>::put(Key key, Value value){
  // return "put key for value";
};





