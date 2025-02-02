#pragma once

/* C# Style Properties
 *
 * Usage:
 *
 * class MyClass
{
private:
  int _attribute;

public:
  DJINN_PROPERTY_RO(MyClass, int, readOnlyProperty, {
    std::clog << "Getting attribute equals to " << _attribute << std::endl;
    return _attribute;
  })

  DJINN_PROPERTY_WO(MyClass, int, writeOnlyProperty, {
    std::clog << "Setting attribute to " << value << std::endl;
    _attribute = value;
  })

  DJINN_PROPERTY_RW(MyClass, int, readWriteProperty, {
    std::clog << "Getting attribute equals to " << _attribute << std::endl;
    return _attribute;
  },{
    std::clog << "Setting attribute to " << value << std::endl;
    _attribute = value;
  })

  MyClass()
    : _attribute(0),
      readOnlyProperty(this),
      writeOnlyProperty(this),
      readWriteProperty(this) {
  }
};

int main()
{
  MyClass myVar;

  // Read only
  std::cout << myVar.readOnlyProperty << std::endl; // OK
  //myVar.readOnlyProperty = 42; // Will fail

  // Write only
  //std::cout << myVar.writeOnlyProperty << std::endl; // Will fail
  myVar.writeOnlyProperty = 42; // OK

  // Read write
  std::cout << myVar.readWriteProperty << std::endl; // OK
  myVar.readWriteProperty = 42; // OK
}
*/

#include <iostream>

namespace Djinn::Core {
    template <class T, class TType, TType (T::*Getter)() const>
    class Get {
    public:
        explicit Get(T const* self): _this(self) {
        }

        inline operator TType() const{
            return (_this->*Getter)();
        }

    private:
        TType operator=(TType value) const;

    private:
        T const* _this;
    };

    template <typename T, typename TType, void (T::*Setter)(TType)>
    class Set {
    public:
        explicit Set(T* self): _this(self) {
        }

        inline TType operator=(TType value) const {
            (_this->*Setter)(value);
            return value;
        }

    private:
        operator TType() const;

    private:
        T* _this;
    };

    template <typename T, typename TType, TType (T::*Getter)() const, void (T::*Setter)(TType)>
    class GetSet {
    public:
        explicit GetSet(T* self): _this(self) {
        }

        inline explicit operator TType() const {
            return (_this->*Getter)();
        }

        inline TType operator=(TType value) const {
            (_this->*Setter)(value);
            return value;
        }

    private:
        T* _this;
    };

// Read only.
#define DJINN_PROPERTYR(CLASS, TYPE, NAME, CODE)         \
  inline TYPE Get##NAME() const CODE         \
  Djinn::Core::Get<CLASS, TYPE, &CLASS::Get##NAME> NAME;

// Write only.
#define DJINN_PROPERTYW(CLASS, TYPE, NAME, CODE)         \
  inline void Set##NAME(TYPE value) CODE     \
  Djinn::Core::Set<CLASS, TYPE, &CLASS::Set##NAME> NAME;

// Read/Write.
#define DJINN_PROPERTY(CLASS, TYPE, NAME, GET, SET)  \
  inline TYPE Get##NAME() const GET          \
  inline void Set##NAME(TYPE value) SET      \
  Djinn::Core::GetSet<CLASS, TYPE, &CLASS::Get##NAME, &CLASS::Set##NAME> NAME;
}