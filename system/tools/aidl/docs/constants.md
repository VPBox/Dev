# Defining constants as part of an interface

AIDL has been enhanced to support defining integer and string constants
as part of an interface.

## Integer constants

```
interface IMyInterface {
    const int CONST_A = 1;
    const int CONST_B = 2;
    const int CONST_C = 3;
    ...
}
```

These map to appropriate 32 bit integer class constants in Java and C++ (e.g.
`IMyInterface.CONST_A` and `IMyInterface::CONST_A` respectively).

## String constants

```
interface IMyInterface {
    const String CONST_A = "foo";
    const String CONST_B = "bar";
    ...
}
```

These map to class level String constants in Java, and static getter
functions that return a const android::String16& in C++.

The constants are limited to contain printable ASCII characters < 0x10
and without backspaces (i.e. no '\' character).

