#include <cpr/cpr.h>

#include <iostream>
#include <optional>
#include <string>
using std::function;
using std::nullopt;
using std::optional;
using std::string;

namespace cpst {
template <typename Ret, typename... Args>
MongoDBCache<Ret, Args...>* MongoDBCache<Ret, Args...>::clone() {
  return new MongoDBCache<Ret, Args...>(this->key, this->pickle, this->unpickle,
                                        this->hash, this->base);
}

// Constructor
template <typename Ret, typename... Args>
MongoDBCache<Ret, Args...>::MongoDBCache(string (*key)(Args...),
                                         string (*pickle)(Ret),
                                         Ret (*unpickle)(string),
                                         string (*hash)(string), string base)
    : MongoDBCache(key, pickle, unpickle, hash) {
  this->base = base;
}

// Constructor
template <typename Ret, typename... Args>
MongoDBCache<Ret, Args...>::MongoDBCache(string (*key)(Args...),
                                         string (*pickle)(Ret),
                                         Ret (*unpickle)(string),
                                         string (*hash)(string), string dbURL,
                                         string funcName)
    : MongoDBCache(key, pickle, unpickle, hash,
                   dbURL + "/persist/" + funcName + "/") {}

template <typename Ret, typename... Args>
string MongoDBCache<Ret, Args...>::makeUrlForKey(const string& key) {
  return this->base + this->hash(key);
}

std::string parseJson(std::string s) {
  for (std::string::size_type i = 0; i < s.size(); i++) {
    if (s[i] == '\"') {
      bool isResult = false;
      string resultStr = "result";
      int index = 0;
      for (std::string::size_type j = i + 1; j < s.size(); j++) {
        if (isResult && (s[j] == '\"') && (s[j + 1] == ':')) {
          int startIndex = j + 1;
          while (startIndex < s.size()) {
            if (s[startIndex] == '\"') break;
            startIndex++;
          }
          startIndex++;
          int endIndex = startIndex;
          while (endIndex < s.size()) {
            if (s[endIndex] == '\"' && s[endIndex] != '\\') break;
            endIndex++;
          }
          return s.substr(startIndex, endIndex - startIndex);
        }
        if (isResult) break;
        if (s[j] != resultStr[index++]) break;
        if (s[j] == 't') isResult = true;
      }
    }
  }
  return "";
}

template <typename Ret, typename... Args>
std::optional<Ret> MongoDBCache<Ret, Args...>::get(const Args&... args) {
  cpr::Response response =
      cpr::Get(cpr::Url{makeUrlForKey(this->key(args...))});  // catch error
  if (response.status_code == 200) {
    return optional<Ret>{this->unpickle(parseJson(response.text))};
  }
  return nullopt;
}

template <typename Ret, typename... Args>
void MongoDBCache<Ret, Args...>::put(const Args&... args, const Ret& value) {
  string key = this->key(args...);
  // string url = makeUrlForKey(key);
  string url = this->base;
  string valueStr = this->pickle(value);
  string funcName = "{\"funcname\": \"test\",";
  string hashLine = "\"hash\": \"" + key + "\",";
  string nameSpace = "\"namespace\": \"cppersist\",";
  string result = "\"result\": \"" + valueStr +
                  "\""
                  "}";
  string jsonPost = funcName + hashLine + nameSpace + result;
  // string jsonPost = std::format(jsonPostTemplate, key, valueStr);
  // cpr::Response response =
  // cpr::Post(cpr::Url{url}, cpr::Body{"{\"return\": \"" + valueStr + "\"}"},
  // cpr::Header{{"Content-Type", "application/json"}});
  cpr::Response response =
      cpr::Post(cpr::Url{url}, cpr::Body{jsonPost},
                cpr::Header{{"Content-Type", "application/json"}});
}
}  // namespace cpst