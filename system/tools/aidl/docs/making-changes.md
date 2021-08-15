# Making changes

## Coding style

This project was originally written in C, in the Android platform style.
It has been substantially re-written in C++, in the Google C++ style.

This style
[is summarized here](https://google.github.io/styleguide/cppguide.html).

When in doubt, clang-format -style=google is a good reference.

## Testing

This codebase has both integration and unittests, all of which are expected to
consistently pass against a device/emulator:

```
$ ./runtests.sh && echo "All tests pass"

```
