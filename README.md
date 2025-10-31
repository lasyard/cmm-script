# C-- Script

C-- Script is a simple script language.

## Comments

Comments are C-style, that is, in `/* */` pairs.

## Consts

There are three types of const number in C-- Script. They are all integers. No other data types are supported. They are

* Decimal int, like `1234`, `5678`, `90`
* Hexadecimal int, like `0x1A`, `0X2B`
* Binary int, like `0b11`, `0B1010`

There is also const string in C-- Script. To make a string, put text between a pair of double quotation marks, like `"Hello, World!"`. Escape characters as in a C-style string are not supported.

## Varibles

Rules for varible names are like in C language, except that @ and \ can be used in varible names.

Varibles are defined when they appear for the first time. The type of a varible is determined by the value assigned to it. If a varible is never assigned a value, its type is UNDEF. According to the values assigned are integers or strings, varibles can be INT or STR type, which are simple types.

A varible can also be VEC type, that means there are a series of values associated with the varible, which are indexed by integers. Each of the value can also be of any type. For example

```cpp
a = @
/* @ is a built-in special varible. 
 * To make a varible VEC type, you should assign @ to it.
 */
a[0] = 1; a[1] = "Hello"; a[2] = @
a[2][0] = 2; a[2, 1] = 3 /* both are supported */
```

A varible can also be MAP type, that means there are a series of values associated with the varible, which are indexed by strings. Each of the value can also be any type. For example

```cpp
a = \
/* \ is a built-in special varible.
 * To make a varible MAP type, you should assign \ to it.
 */
a["first"] = 1; a.second = 2 /* both are supported */
```

## Expressions and statements

Expressions are all like in C language if the operators are supported. The unsupported operators are:

* Post self increment ++ and post self decrement --
* Address & and derefrence *
* Ternary operator ? :

Any expressions can be statements. Statements are ended if a line is ended. You can use `;` to seperate several statements in a single line, but `;` is not required for a statement.

## Branch structure

Using `? : $` to make a branch structure, like

```cpp
a < 10 ? ++num1 : ++num2 $
```

That means, if `a` is less than 10, the value of `num1` is increased by one, otherwise the value of `num2` is increased by one.

## Loop structure

Using { # } to make a loop structure, like

```cpp
i = 0 { i < 10 ?:#$ ++i }
```

That means, `i` is increased by one, starting from `0`; if `i` is not less than `10`, the loop ends. The codes between `{` and `}` are executed repeatedly, and `#` always makes it jump out the loop to the code after `}`.

## Functions

There are only built-in functions, no user-defined functions are supported. Functions take unlimited number of parameters and return a value, like

```cpp
_pw(3, a, a+b, (a+b)*c)
```

Currently the built-in functions are:

* `p()`: to output the parameters
* `pln()`: to output the parameters with a line break
* `pw()`: to output the parameters except the first one. the first parameter is used to set the output width

## Command Line

In the command line interface, you can type in any codes, which are executed when you press Enter. There are also a few commands you can use, but they are not part of C-- Script. They are

* `clear`: to clear all user-defined varibles
* `ids`: to show all the varibles
* `funs`: to show all the functions
* `run filename`: to look for a file named `filename.cmm`, read it in and execute it
* `exit`: exit the program

## MIT License

<http://lasy.fwh.is/mit_license>.
