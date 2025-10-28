# Infinitas Language (Under Development)
Infinitas is a bytecode-interpreted programming language focused on ethical cybersecurity.
The project here is not the minimum viable product of the project. This is a public active development repository. 

> This project is Licensed under Apache 2. Unit-tests are generated through Aritifical Intelligence (AI) and copyright to unit-tests are disclaimed by the author.
## Build Instructions
- Clone the github repository using:
```
git clone https://github.com/0x221E/Infinitas --recursive
```
- Enter this directory and then create a new folder called "build".
```
mkdir build && cd build
```
- Build the application using cmake.
```
cmake .. && cmake --build
```
## Shortcomings & Development Mistakes
Currently there are numerous architectural shortcomings. Known architectural issues include: 
- Inefficient and architecturally sub-optimal use of std::unordered_map in Compiler for native function search.
- Inefficient and architecturally sub-optimal executor case dispatching system in virtual-machine.
## Information and Requirements
- **Language Standard**: C++23 or later
- **Compiler**: Tested on g++14
- **Dependencies**: spdlog
## Authoritative Specification
A language feature is first drafted on paper inside [Infinitas Specification](https://infinitas-spec.atlassian.net/wiki) and later enforced inside this github repository.
This is to enforce:
- Strict, predictable behavior
- Clear memory management
- Language security
> Minor organizations, security patches, and bug fixes that are aimed to enforce the current behavior inside specification do not require changes inside the specification.
> You can access specification from this link: [https://infinitas-spec.atlassian.net/wiki](https://infinitas-spec.atlassian.net/wiki).
## Specification Features Under Implementation
- **4.3.5.1** - Primitive exit function errorCode parameter
- **2.2.11** - No-Heap Mode // planned for December.
- Standard Library
- Enforce primitive naming convention on lexer:
    - "\_\_" for functions (e.g., `__print`),
    - "\_\_\_" for types (e.g., `__int`)
    - User-defined functions and types cannot use these prefixes.
## Currently available specified features
- All primitive type conversions.
- Primitive type arithmetic through primitive arithmetic functions (Operators +-*/ is not allowed as of yet since overloading does not exist).
- \_\_print() function that takes a \_\_\_string as an argument.
- \_\_exit() function without an exit code (argument specified; yet to be implemented).
These functions will be used in the Standard Library (STL) that is being actively developed, operations similar to arithmetic will go through the overloading process.
## Planned Cybersecurity Features
- Sandboxing user code and native files through:
```
sandbox options=block-syscall,process-isolation{
    io.read_file("~/Desktop/binary") // File won't be read and instead the program will output the attempted syscall with extra information.
};

sandbox options=process-isolation file="~/Desktop/binary";
```
- Single-line networking and raw socket access:
```
network.send_raw(TCP, [ACK], 127.0.0.1, 5321).async();

var requestHeaders = /** ...specify headers... */

network.send_http(GET, 127.0.0.1, 5321, requestHeaders).async();
```
