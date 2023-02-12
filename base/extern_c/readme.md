### 将 lib.c 编译成动态库
```sh
gcc lib.c -shared -fPIC -o libcfunc.so
```

### 编译 main.cc
```sh
g++ --std=c++11 main.cc -L. -lcfunc -o main.out
```
如果 lib.h 中不追加 extern "C" 的判断，则 main.cc 无法使用 libcfunc.so 中的 say_hi 方法，会报找不到该方法。这是因为 C++ 支持函数重载，而 C 不支持。函数被 C++ 编译后在符号库中的名字与 C 语言的不同。例如，假设某个函数的原型为：void foo( int x, int y )，该函数被 C 编译器编译后在符号库中的名字为 _foo，而 C++ 编译器则会产生像 _foo_int_int 之类的名字（不同的编译器可能生成的名字不同，但是都采用了相同的机制，生成的新名字称为 “mangled name”）。

### 运行 main.out
```sh
export LD_LIBRARY_PATH=.
./main.out
```
