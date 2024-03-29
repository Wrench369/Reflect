#pragma once

#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "Core/Core.h"
#include "Core/Enums.h"

#include "Core/Util.h"  // modify by xzf

struct ReflectFunction;
struct ReflectMember;

namespace Reflect {
struct ReflectTypeNameData {
  std::string Type;
  std::string Name;
  int TypeSize;
  ReflectMemberType ReflectMemberType;
  bool IsConst;
  std::vector<std::string> ContainerProps;

  ReflectTypeNameData()
      : Type("Unkown"),
        Name("Unkown"),
        TypeSize(0),
        ReflectMemberType(ReflectMemberType::Value),
        IsConst(false) {}

  ReflectTypeNameData(const std::string& type, const std::string& name,
                      const int& typeSize,
                      const Reflect::ReflectMemberType& memberType,
                      const bool& isConst)
      : Type(type),
        Name(name),
        TypeSize(typeSize),
        ReflectMemberType(memberType),
        IsConst(isConst) {}

  bool operator!=(const ReflectTypeNameData& other) const {
    bool propsEqual = ContainerProps.size() != other.ContainerProps.size();
    if (propsEqual) {
      for (uint32_t i = 0; i < ContainerProps.size(); ++i) {
        if (ContainerProps[i] == other.ContainerProps[i]) {
          propsEqual = false;
          break;
        }
      }
    }
    return Type != other.Type || Name != other.Name ||
           TypeSize != other.TypeSize || propsEqual;
  }
};

struct ReflectMemberData : public ReflectTypeNameData {
  ReflectType ReflectType = ReflectType::Member;
};

struct ReflectFunctionData : public ReflectTypeNameData {
  ReflectType ReflectType = ReflectType::Function;
  std::vector<ReflectTypeNameData> Parameters;
};

struct ReflectContainerData : public ReflectTypeNameData {
  std::string Name;
  ReflectType ReflectType;
  int ReflectGenerateBodyLine;

  std::vector<ReflectMemberData> Members;
  std::vector<ReflectFunctionData> Functions;
};

struct FileParsedData {
  std::string Data;
  int Cursor;
  std::string FilePath;
  std::string FileName;
  int GeneratedBodyLineOffset;

  std::vector<ReflectContainerData> ReflectData;
};

struct ReflectMemberProp {
  ReflectMemberProp(const char* name, const char* type, int offset,
                    std::vector<std::string> const& strProperties)
      : Name(name), Type(type), Offset(offset), StrProperties(strProperties) {}

  bool ContainsProperty(std::vector<std::string> const& flags) {
    for (auto const& flag : flags) {
      for (auto const& p : StrProperties) {
        if (p == flag) {
          return true;
        }
      }
    }
    return false;
  }

  const char* Name;
  const char* Type;
  int Offset;
  std::vector<std::string> StrProperties;
};

/// <summary>
/// Store arguments for a function ptr
/// </summary>
struct FunctionPtrArgs {
 public:
  struct Arg {
    Arg(std::string type, void* ptr) : Type(type), Ptr(ptr) {}

    void* Get() const { return Ptr; }

   private:
    std::string Type;
    void* Ptr;
  };

  FunctionPtrArgs() {}
  FunctionPtrArgs(const std::vector<Arg>& args) : m_args(args) {}

  void* GetArg(int index) { return m_args.at(index).Get(); }

  template <typename T>
  REFLECT_DLL void AddArg(T* obj) {
    m_args.push_back(Arg(Reflect::Util::GetTypeName(*obj), obj));
  }

 private:
  std::vector<Arg> m_args;
};

using FunctionPtr = Reflect::ReflectReturnCode (*)(void* objectPtr,
                                                   void* returnValue,
                                                   FunctionPtrArgs& args);

struct ReflectFunction {
  ReflectFunction(void* objectPtr, FunctionPtr func)
      : m_objectPtr(objectPtr), m_func(func) {}

  // template<typename... Args>
  // void Invoke(void* returnValue, Args... args)
  //{
  //	FunctionPtrArgs funcArgs =
  // PackFunctionArgs(std::forward<Args>(args)...); 	int i =
  //*static_cast<int*>(funcArgs.GetArg(0)); 	int* ip =
  // static_cast<int*>(funcArgs.GetArg(1)); 	return (*Func)(ObjectPtr,
  // returnValue, funcArgs);
  //}

  REFLECT_DLL Reflect::ReflectReturnCode Invoke(
      void* returnValue = nullptr,
      FunctionPtrArgs functionArgs = FunctionPtrArgs()) {
    if (IsValid()) {
      (*m_func)(m_objectPtr, returnValue, functionArgs);
      return ReflectReturnCode::SUCCESS;
    }
    return ReflectReturnCode::INVALID_FUNCTION_POINTER;
  }

  REFLECT_DLL bool IsValid() const { return m_objectPtr != nullptr; }

 private:
  template <typename... Args>
  FunctionPtrArgs PackFunctionArgs(Args... args) {
    std::vector<FunctionPtrArgs::Arg> funcArgs = {PackFunctionArg(args)...};
    return FunctionPtrArgs(funcArgs);
  }

  template <typename T, typename... Args>
  FunctionPtrArgs::Arg PackFunctionArg(T& t, Args&... args) {
    return FunctionPtrArgs::Arg(Reflect::Util::GetTypeName(t), &t);
  }

  template <typename T, typename... Args>
  FunctionPtrArgs::Arg PackFunctionArg(T* t, Args... args) {
    return FunctionPtrArgs::Arg(Reflect::Util::GetTypeName(t),
                                static_cast<void*>(t));
  }

 private:
  void* m_objectPtr;
  FunctionPtr m_func;
};

struct ReflectMember {
  ReflectMember(const char* memberName, std::string memberType, void* memberPtr)
      : m_name(memberName), m_type(memberType), m_ptr(memberPtr) {}

  REFLECT_DLL bool IsValid() const { return m_ptr != nullptr; }

  void* GetRawPointer() { return m_ptr; }

  std::string GetName() { return m_name; }

  std::string GetTypeName() { return m_type; }

  template <typename T>
  REFLECT_DLL T* ConvertToType() {
    const char* convertType = Reflect::Util::GetTypeName<T>();
    if (convertType != m_type) {
      return nullptr;
    }
    return static_cast<T*>(m_ptr);
  }

 private:
  const char* m_name;
  std::string m_type;
  void* m_ptr;
  int m_offset;
};

struct IReflect {
  REFLECT_DLL virtual ReflectFunction GetFunction(const char* functionName) {
    (void)functionName;
    return ReflectFunction(nullptr, nullptr);
  };
  REFLECT_DLL virtual ReflectMember GetMember(const char* memberName) {
    (void)memberName;
    return ReflectMember("", "void", nullptr);
  };
  REFLECT_DLL virtual std::vector<ReflectMember> GetMembers(
      std::vector<std::string> const& flags) {
    (void)flags;
    return {};
  };
};
}  // namespace Reflect

#define REFLECT_BASE() \
 public                \
  Reflect::IReflect
#define REFLECT_STRUCT(...)
#define REFLECT_CLASS(...)
