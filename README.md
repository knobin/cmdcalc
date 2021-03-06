# cmdCalc
[![Build Status](https://travis-ci.org/knobin/cmdcalc.svg?branch=main)](https://travis-ci.org/knobin/cmdcalc)

Command-line calculator in C++17 using Top-Down Parsing.

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
> 1+2+3+4*10/8
11
> 1+3-(-5)*4^2+pi+log10(10)
88.1416
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

<factor> ::= -<value><factor_tail>
        |   <value><factor_tail>

<factor_tail> ::= ^-<value><factor_tail>
        |   ^<value><factor_tail>
        |   <empty>

<value> ::= ( <expr> )
        |   <id>
        |   num

<id> ::= function <value>
        |   constant
```

### Symbolic constants
Currently supported constants are: 

Constant | Implementation
--- | ---
pi | 3.14159265
e | 2.71828183

Example usage:

```shell
$ ./cmdCalc
> 2*pi*5
31.4159
> e^e
15.1543
```

### Single-argument functions
The implemented functions are:

Function | Implementation | Computes
--- | --- | ---
log | std::log | Natural (base-e) logarithm of arg
log10 | std::log10 | Common (base-10) logarithm of arg
exp | std::exp | e raised to the power of arg
sin | std::sin | Sine of arg (in radians)
cos | std::cos | Cosine  of arg (in radians)
tan | std::tan | Tangent of arg (in radians)
arcsin | std::asin | Principal value of the arc sine of arg
arccos | std::acos | Principal value of the arc cosine of arg
arctan | std::atan | Principal value of the arc tangent of arg

Example usage:

```shell
$ ./cmdCalc
> log(10)
2.30259
> cos(pi)
-1
```

## License
See [MIT License](LICENSE).

This project uses third-party dependencies:
* [Catch2](https://github.com/catchorg/Catch2): A unit testing framework for C++ ([licence](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
