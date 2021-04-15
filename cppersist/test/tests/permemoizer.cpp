#include "cppersist/local.hpp"
#include "cppersist/mongo.hpp"
#include "gtest/gtest.h"
#include <string>
#include <filesystem>
using namespace cpst;
using std::string;

class TestClass : public Memoizable<string, string> {
 public:
  string solve(string x) override {
    return x;
  }
};

//ensure that changing filesystem cache path is actually changing the cache location
TEST(FSMemoizedObj, ChangeLocMiss) {
  string newDir = "../dummy";
  auto memo = getLocalMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>);
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");
  memo.setCacheLoc(newDir);
  ASSERT_EQ(memo.getCache()->get("a"), nullopt);

  removeDir(newDir);
}

//ensure that changing filesystem cache path correctly adjusts to the new cache location
TEST(FSMemoizedObj, ChangeLocHit) {
  string newDir = "../dummy";
  auto memo = getLocalMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>);
  memo.setCacheLoc(newDir);
  ASSERT_EQ(memo.getCache()->get("a"), nullopt);
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");

  removeDir(newDir);
}

//ensure that the DB URL of the cache is actually changing to the new cache location
TEST(MongoMemoizedObj, ChangeLocMiss) {
  string newLoc = "localhost:5001";
  auto memo = getMongoMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>,"localhost:5000","test");
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");
  memo.setCacheLoc(newLoc);
  ASSERT_EQ(memo.getCache()->get("a"), "a");
}

//ensure that changing MongoDB URL still results in cache hit after changing location 
TEST(MongoMemoizedObj, ChangeLocHit) {
  string newLoc = "localhost:5001";
  auto memo = getMongoMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>,"localhost:5000","test");
  memo.setCacheLoc(newLoc);
  ASSERT_EQ(memo.getCache()->get("a"), "a");
}

//invalid url - MongoDB cache
TEST(MongoMemoizedObj, NonExistentURL) {
  auto memo = getMongoMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>,"dummy","dummy");
  memo("a"); //shouldn't quit the user's program, should fail silently
}

//invalid path - filesystem cache
TEST(FSMemoizedObj, NonExistentPath) {
  auto memo = getLocalMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>,"dummy");
  memo.setCacheLoc("z/cq=;d12d11d;';'./c.xcz v, .z,c .z"); //invalid path name
  memo("a"); //shouldn't quit the user's program, should fail silently
}





