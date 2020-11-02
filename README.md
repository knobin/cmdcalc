# cmdCalc
Command-line calculator in C++ using Top-Down Parsing implemented in C++17.

## Build
Start by cloning the repository:

```shell
$ git clone --recursive https://github.com/knobin/cmdcalc.git
```

Then use cmake to build the project. For example on unix systems:

```shell
$ mkdir build && cd build
$ cmake -G "Unix Makefiles" ..
$ make
```

It builds the small library `CalcEval` that contains the parser and `cmdCalc` which is the application.

## Usage
The `cmdCalc` can be used in two ways, either with REPL:

```shell
$ ./cmdCalc
> 1+2+3+4
10
> 1.05*20
21
```

or by providing command-line arguments:

```shell
$ ./cmdCalc "1+2+3+4"
10
$ ./cmdCalc "1+2+3+4" "1.05*20"
10
21
```

### Errors
When the parser encounter an error it will provide detailed information:

```shell
$ ./cmdCalc "10++10"
Error parsing!
Unexpected token of "plus" at line 1 : 4
10++
---^
Expected '(', identifier or number!
```
Same with the scanner:

```shell
$ ./cmdCalc "10?10"
Error scanning!
Unexpected symbol at line 1 : 3
10?
--^
```

## Grammar
The calculator can understand numbers, symbolic constants, single-argument functions, one unary and five binary operators using the following grammar below. 

```
<expr> ::= <term><expr_tail>

<expr_tail> ::= +<term><expr_tail>
        |   -<term><expr_tail>
        |   <empty>

<term> ::= <factor><term_tail>

<term_tail> ::= *<factor><term_tail>
        |   /<factor><term_tail>
        |   <empty>

<factor> ::= <unary> ^ <unary>
        |   <unary>

<unary> ::= -<value>
        |	<value>

<value> ::= ( <expr> )
        | 	<id>
        | 	num

<id> ::= function <value>
        | 	constant
```

### Symbolic constants
Currently supported constants are: 

Constant | implemented value
--- | ---
pi | 3.14159265
e | 2.71828183

### Single-argument functions
The implemented functions are:

Function | using
--- | ---
log | std::log
log10 | std::log10
exp | std::exp
sin | std::sin
cos | std::cos
tan | std::tan
arcsin | std::asin
arccos | std::acos
arctan | std::atan

## License
See [MIT License](LICENSE).

This project uses third-party dependencies:
* [Catch2](https://github.com/catchorg/Catch2): A unit testing framework for C++ ([licence](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
