# Static Program Analysis tool for SIMPLE language

## Prerequisites
- CMake (version 3.20.2 or higher)
- C++ compiler (supporting C++ 17 or newer)

### Build Instructions

1. Clone the repository
2. Navigate to the project repository
```shell
cd spa/Team20/Code20
```
3. Create a build directory and navigate to it
```shell
mkdir build
cd build
```
4. Generate build files and build the project
```shell
cmake ..
cmake --build .
```

### Run Instructions

To run the REPL interface, navigate to `src/spa` folder inside the build directory and 
locate the executable `main`. Run the code with the path to a text file containing the SIMPLE source code.

```shell
cd src/spa
./main <SOURCE_CODE_PATH>
```

Example Program Query Language (PQL) queries to run
```
1. Select all statement numbers
stmt s; Select s 

2. Select all procedures that call procedure "Third" directly or indirectly
Select p such that Calls* (p, "Third")

3. Select statement number of all assignment statements to variable "x" located in a loop, that can be reached (in terms of control flow) from statement 1.
assign a; while w; Select a pattern a ("x", _) such that Parent* (w, a) and Next* (1, a)
```

### Language Reference
- [SIMPLE Language](https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/simple-programming.html)
- [Basic Program Query Language (PQL)](https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/program-query-language/introduction.html)
- [Extensions to PQL](https://nus-cs3203.github.io/course-website/contents/advanced-spa-requirements/pql.html)

### Running Cpplint Locally

- Run and execute `./cpplint_local.h` in /Team20 directory in terminal.
- Cpplint errors will be displayed in the terminal.

# Team Members

Name | Email              | Development OS/Toolchain
-:|:-------------------|-|
Zhang Zhikai | e0543603@u.nus.edu | macOS
Glenn Lim | glennljw@u.nus.edu | macOS
Cai Zhibin | czhibin@u.nus.edu  | macOS, CMake 3.27.8, Make 3.81
Phua Jun Heng | e0559947@u.nus.edu | windows 11
Ng Choon Siong | choon.ng@u.nus.edu | macOS
