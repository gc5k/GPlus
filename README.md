# gplus
GEAR improved

## Developer Note

To build on Windows, you need to download and build the Program Options library and Log library of [Boost 1.67.0](https://dl.bintray.com/boostorg/release/1.67.0/source/) in advanced. Either copy the Boost libraries to the `lib/win` subdirectory of the gplus project or define the `BOOST_1_67_0_LIB_DIR` environment variable pointing to the directory path of the Boost libraries built on your machine (such as D:\boost_1_67_0\stage\lib).

### Coding Style

Please follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). However, C++ exceptions are allowed. [Google C++ Style Guide bans C++ exceptions](https://google.github.io/styleguide/cppguide.html#Exceptions) mainly because Google's existing code is not exception-tolerant the time the style was wrote. Exceptions actually have more pros than cons. See [Exceptions and Error Handling](https://isocpp.org/wiki/faq/exceptions) written by Standard C++ Foundation. Also see the [debate on Zhihu](https://www.zhihu.com/question/22889420).

[`cpplint.sh`](https://github.com/gc5k/gplus/blob/master/cpplint.sh) is used to check whether all or specific source files follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). Notice that it uses the `-printf` option of `find` to ignore the `third_party` directory. This option is only available in GNU `find` but not in BSD `find` which is default on Mac OS X. Type `brew install findutils` in terminal to install GNU `find` on Mac OS X. See [this post](https://superuser.com/a/293200/95715) for more details.
