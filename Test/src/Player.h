#pragma once

#include "Generated/Player_reflect_generated.h"
#include "Reflect.h"

REFLECT_CLASS()
class Person : REFLECT_BASE() {
  REFLECT_GENERATED_BODY()

 public:
  Person() {}

  REFLECT_PROPERTY()
  int y;
  REFLECT_PROPERTY()
  int x;

  REFLECT_PROPERTY()
  int TestFuncInt(int const* testInt, const int& testIntPtr) {
    std::cout << "TestFunc is being called";
    return *testInt + testIntPtr;
  }
  REFLECT_PROPERTY()
  const char* TestFuncChar() {
    std::cout << "TestFuncChar";
    return "";
  }
  REFLECT_PROPERTY()
  void TestFuncVoid() { std::cout << "TestFunc is being called"; }

  REFLECT_PROPERTY()
  std::string TestString;
};