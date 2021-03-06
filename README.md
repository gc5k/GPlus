# gplus
GEAR improved

## Developer Note

### Prerequisite Libraries

On Linux and Windows, you need to download and build the Program Options library and Log library of [Boost 1.67.0](https://dl.bintray.com/boostorg/release/1.67.0/source/) in advanced. 

On Windows, either copy the Boost libraries to the `lib/win` subdirectory of the gplus project or define the `BOOST_1_67_0_LIB_DIR` environment variable pointing to the directory path of the Boost libraries built on your machine (such as D:\boost_1_67_0\stage\lib).

On Mac OS X, you don't have to worry about this as we have already shipped with the Mac OS X version of the prebuilt boost libraries. Just open Xcode and build the gplus product.

### Build on Linux

You need to install [CMake](https://cmake.org/) to build gplus on Linux. Recall that Boost Progrm Options and Logs (version 1.67.0) are also required.

After CMake is installed,

```bash
cd /path/to/gplus
mkdir build
cd build
cmake ..
make
```

The gplus executable will be built in the `build` subdirectory of gplus.

### Coding Style

We basically (but not strictly) follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), especially there're (not only) such following exceptions:

- The default formatting, especially those formatting which cannot be adjusted by IDE options/preferences, applied by Xcode and Visual Studio is allowed. Therefore, the following formatting are allowed because they are the default and even the only available behaviors of IDEs, especially Xcode. And we don't want to add too much burden on team members:
  - Indentations of namespace are allowed.
  - Therefore, each line of text is allowed to reach the limit of 90 characters long, taking account of nested namespace.
  - The `public:`, `protected:`, and `private:` keywords are fine to not indent.
  - Trailing whitespace at the end of a line is allowed.
  - Constructor initializer lists can be formatted as:
  ```c++
    MyClass::MyClass(int var)
    : some_var_(var)
    , some_other_var_(var + 1) {
      DoSomething();
    }
  ```
- Use of Boost libraries and C++17 are encouraged to try their new and ambitious methodology, as long as they don't complicate the business code.

[`cpplint.sh`](https://github.com/gc5k/gplus/blob/master/cpplint.sh) is used to check whether all or specific source files follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). But it is modified to adapt the gplus project. Notice that it uses the `-printf` option of `find` to ignore the `third_party` directory. This option is only available in GNU `find` but not in BSD `find` which is default on Mac OS X. Type `brew install findutils` in terminal to install GNU `find` on Mac OS X. See [this post](https://superuser.com/a/293200/95715) for more details.
