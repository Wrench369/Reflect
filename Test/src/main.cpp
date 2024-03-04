#include "Player.h"
#include "Reflect.h"

void FuncNoReturn() {
  // Get a function with no return value.
  // Player player;
  // auto playerGetId = player.GetFunction("PrintHelloWorld");
  // playerGetId.Invoke();

  Person person;
  auto personGetId = person.GetFunction("TestFuncVoid");
  personGetId.Invoke();
}

// void FuncReturnValue() {
//   // Get a function with a return value std::string.
//   // The return value with be set to playerId.
//   Player player;
//   ReflectFunction playerGetId = player.GetFunction("GetId");
//   std::string playerId;
//   playerGetId.Invoke(&playerId);
// }

// void FuncWithParameters() {
//   // Get a function with no return value but which has a single
//   // parameter.
//   Player player;
//   ReflectFunction parameterFunc =
//   player.GetFunction("GetOnlineFriendsCountt");

//   // Setup the parameter to send to the function. This is order
//   // sensitive.
//   FunctionPtrArgs args;
//   int intParameter = 8;
//   args.AddArg(&intParameter);

//   int returnCount;
//   std::cout << ReflectReturnCodeToString(
//       parameterFunc.Invoke(&returnCount, args));
// }

int main(void) {
  FuncNoReturn();
  // FuncReturnValue();
  // FuncWithParameters();

  return 0;
}