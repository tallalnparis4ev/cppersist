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

TEST(FSMemoizedObj, ChangeLocMiss) {
  string newDir = "../dummy";
  auto memo = getLocalMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>);
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");
  memo.setCacheLoc(newDir);
  ASSERT_EQ(memo.getCache()->get("a"), nullopt);

  removeDir(newDir);
}

TEST(FSMemoizedObj, ChangeLocHit) {
  string newDir = "../dummy";
  auto memo = getLocalMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>);
  memo.setCacheLoc(newDir);
  ASSERT_EQ(memo.getCache()->get("a"), nullopt);
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");

  removeDir(newDir);
}


TEST(MongoMemoizedObj, ChangeLocMiss) {
  string newLoc = "localhost:5001";
  auto memo = getMongoMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>,"localhost:5000");
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");
  memo.setCacheLoc(newLoc);
  ASSERT_EQ(memo.getCache()->get("a"), nullopt);
}

TEST(MongoMemoizedObj, ChangeLocHit) {
  string newLoc = "localhost:5001";
  auto memo = getMongoMemoizedObj<TestClass>(identity<string>,identity<string>,identity<string>,"localhost:5000");
  memo.setCacheLoc(newLoc);
  ASSERT_EQ(memo.getCache()->get("a"), nullopt);
  memo("a");
  ASSERT_EQ(memo.getCache()->get("a"), "a");
}




