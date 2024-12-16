# SingleLog
## Description

SingleLog is a header only **C++14** singleton logging library. It's fast, convenient, compiles on Windows and Linux, and does what it says on the tin.


## Example

Using SingleLog is as easy as this:

```c++
https://github.com/James-Chapman/SingleLog/blob/master/example.cpp
```

Which, when run will produce the following in the console:

```
user@machine> ./SingleLogExample 
2024-12-16 20:27:40 +0000  <CRITICAL>  Local logger ref thread:  Critial message
2024-12-16 20:27:40 +0000  <ERROR>  Local logger ref thread:  Error message
2024-12-16 20:27:40 +0000  <WARNING>  Local logger ref thread:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  Local logger ref thread:  Notice message
2024-12-16 20:27:40 +0000  <INFO>  Local logger ref thread:  Info message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging:  Critial message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging_v2:  Critical message 1
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging:  Error message
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging:  Notice message
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging_v2:  Error message 2
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging:  Info message
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging_v2:  Warning message 3
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging_v2:  Notice message 4
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging_v2:  Info message 5
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar
```

And the following in the log file **"example.log"**:

```
user@machine> cat example.log
2024-12-16 20:27:40 +0000  <CRITICAL>  Local logger ref thread:  Critial message
2024-12-16 20:27:40 +0000  <ERROR>  Local logger ref thread:  Error message
2024-12-16 20:27:40 +0000  <WARNING>  Local logger ref thread:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  Local logger ref thread:  Notice message
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  >>> Entering: MacroLogging
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  >>> Entering: MacroLogging_v2
2024-12-16 20:27:40 +0000  <INFO>  Local logger ref thread:  Info message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging:  Critial message
2024-12-16 20:27:40 +0000  <DEBUG>  Local logger ref thread:  Debug message
2024-12-16 20:27:40 +0000  <TRACE>  Local logger ref thread:  Trace message
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging:  Error message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging_v2:  Critical message 1
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging:  Notice message
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging_v2:  Error message 2
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging:  Info message
2024-12-16 20:27:40 +0000  <DEBUG>  MacroLogging:  Debug message
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging_v2:  Warning message 3
2024-12-16 20:27:40 +0000  <TRACE>  MacroLogging:  Trace message
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging_v2:  Notice message 4
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging_v2:  Info message 5
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  <<< Exiting: MacroLogging
2024-12-16 20:27:40 +0000  <DEBUG>  MacroLogging_v2:  Debug message 6
2024-12-16 20:27:40 +0000  <TRACE>  MacroLogging_v2:  Trace message 7
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  <<< Exiting: MacroLogging_v2
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar

```

