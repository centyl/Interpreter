# Interpreter

This application works as an interpreter of a simple scripting language. It analyses provided source code, generates a sequence of operations, and manages their execution.

# How it works

## Correct order of operations ensured by utilizing Abstract Syntax Trees

Usually, parsing a line of code results in an Abstract Syntax Tree object, containing info on type and order of operations to be executed. 
Branch nodes of this tree represent one operation, and leaves represent objects such as integer literals or variables. 
Main advantage of AST’s hierarchical structure is triviality of executing operations contained therein in correct order. 
The higher a node is, the lower is its priority. Therefore, all its children should execute before it. 

| ![obraz](https://user-images.githubusercontent.com/50883639/173599520-9fe3175d-e1d2-4ffb-9925-71ce73d88bb3.png) |
|:--:|
| Sample Abstract Syntax Tree representation of a while loop printing and incrementing a variable |

Created trees are grouped into code blocks. Apart from the default block, those are encased in brackets, and may represent, for example, body of a function or of a loop. 

| ![obraz](https://user-images.githubusercontent.com/50883639/173601670-79612387-a3e6-40cf-91b3-f062c0e50953.png) |
|:--:|
| Sample code block containing three ASTs |

## Parsing a line of code

In general a Tree is built from subsequent operation symbols found in a line, and operand values extracted from between them. 
Usually after finding a symbol, two entities are created 
* Node representing the operation corresponding to that symbol
* Object created from characters between current and previous symbol found 

After execution priority is assigned to the Node, taking into account parentheses currently open, these two elements can be added to an AST.

| Priority | Symbols | Operations |
| - | - | - |
| 0 | -> | Calling anonymous function |
| 1 | , | Creating array of elements |
| 2 | = += -= /= *= | Assignment |
| 3 | \|\| | Logical disjunction |
| 4 | && | Logical conjunction |
| 5 | == != | Testing objects' equality |
| 6 | > >= < <= | Less than/Greater than comparisons |
| 7 | + - | Addition, subtraction |
| 8 | * / // | Multiplication, division, integer division |
| 9 | ** | Exponentiation |
| 10 | . | Object member access |
| 11 | () | Function call |

### Adding a node and an object to the Syntax Tree

Even though most operations require two operands, it’s usually impossible to provide both of them when creating the Node object.
It is so, because values of the operands may be linked to priority of the next operation (not yet encountered by that time). 
Because of that, nodes are inserted into the Tree with a missing operand, which is later assigned to them when the next operation is added.
Such unfinished node is later called Active Node, and its priority determines a manner in which New Node is added.

#### Case 1: tree is empty

In this trivial case, New Node becomes Active Node and the root of the tree, and Object is assigned as its operand.

| ![obraz](https://user-images.githubusercontent.com/50883639/173608717-e426d44e-f12a-4324-a3b6-e36a34f433ec.png) |
|:--:|
| State of a tree after adding Integer 2 and Add operation node, contained in instruction 2+3 |

#### Case 2: Active Node priority = New Node priority, operation is left-to-right associative

Object is assigned to Active Node. New Node becomes its parent and a new Active Node.

| ![obraz](https://user-images.githubusercontent.com/50883639/173609854-82abce96-65fa-4d8f-8f3b-ac556a55e7f4.png) |
|:--:|
| State of a tree after adding Integer 3 and Add operation node, contained in instruction 2+3+4 |

#### Case 3: Active Node priority = New Node priority, operation is right-to-left associative OR Active Node priority < New Node priority

Object becomes a child of New Node, which becomes a child of Active Node, and a new Active Node. 

| ![obraz](https://user-images.githubusercontent.com/50883639/173610375-eb35623b-1fc8-445f-b8b6-b72a64341662.png) | 
|:--:|
| State of a tree after adding Reference y and Assign operation node, contained in instruction x=y=z |

#### Case 4: Active Node priority > New Node priority

In this case, it is necessary to find the Anchor Node, that is an ancestor of Active Node, such that New Node can be placed above it without disrupting correct execution order.

##### Case 4a: Active Node has no ancestors

Active Node itself becomes the Anchor Node. New Node is placed above it, becoming its parent.

| ![obraz](https://user-images.githubusercontent.com/50883639/173640008-3ccc66d4-f1b6-4617-aeb9-c31c35024b4c.png) |
|:--:|
| State of a tree after adding Integer 3 and Add operation, contained in instruction 2*3+4 |

##### Case 4b: Anchor Node parent's priority < New Node priority

New Node is placed between Anchor Node and its parent.

| ![obraz](https://user-images.githubusercontent.com/50883639/173612345-e080a646-9932-415a-9135-7771810539bb.png) |
|:--:|
| State of a tree after adding Integer 4 and Multiply operation, contained in instruction 2+3**4*5 |

##### Case 4c: Anchor Point priority = New Node priority

Node is inserted as in case 2 or 3, depending on operation associativity.

| ![obraz](https://user-images.githubusercontent.com/50883639/173612752-4e0ef864-24e7-470a-9b7f-5ed57a2b4e53.png) |
|:--:|
| State of a tree after adding Integer 4 and Add operation, contained in instruction 2+3*4+5 |

Last operand, created from characters between last symbol found and End of Line, is added to the last Active Node. The Tree is then ready to be added to currently active code Block.

## Node types

For each recognized operation symbol, correct Node subtype must be created. Those may vary in execution method, or number and types of operands expected to correctly perform their task.
Some of those types are

### Basic Node

Basic Node contains two operands, and a binary operation to be executed. For operation * and operands A and B, the result would be A * B.
Most common operations are performed using this node type.

| ![obraz](https://user-images.githubusercontent.com/50883639/173615606-c1e0bd3a-d149-45c8-bedb-b9c473d4dc4e.png) |
|:--:|
| Basic Node representing instruction 2+3 |
 
### Logical operations nodes

Although operations && and || could have been executed by Basic Node, it would not allow for short circuiting them. 
Overriden result calculating methods in separate subtypes ensure, that Boolean value of the second operand will not be calculated, 
if value of the whole expression could be inferred from value of the first one.

| ![obraz](https://user-images.githubusercontent.com/50883639/173616858-eb0fa6b6-1a36-4d60-b0c6-8a7e44d08cb8.png) |
|:--:|
| And Node, if x <= 5 it will return false, without checking if x < 10 |

### Comma Node

It allows grouping variable number of objects together. Can be used to declare Array object, or for defining and calling a function.

![obraz](https://user-images.githubusercontent.com/50883639/173636138-17a1db09-712d-49ca-8920-2a600bbcccf4.png)

### If Node

Allows conditional execution of instructions. If Boolean value of the first operand is true, Block passed as the second operand is executed.
Otherwise, second (else) Block is executed, if it's provided.

| ![obraz](https://user-images.githubusercontent.com/50883639/173636224-2fe6ce0b-9a90-41f8-b185-be2dfa222380.png) |
|:--:|
| If x is positive, set its value to 10 |

### While Node

Works similarly to If Node, but rechecks value of the predicate after executing loop body. If it's still true, the body is executed again.

| ![obraz](https://user-images.githubusercontent.com/50883639/173636497-ddeb8186-9085-4b4a-8f38-5ce5a0c40d0d.png) |
|:--:|
| Increment x until it reaches 20 | 

### Function Node

Finds appropriate function definition, based on provided name and number of arguments, and calls it. By calling `return()` function inside the function's body,
an Object can be returned by this node.

| ![obraz](https://user-images.githubusercontent.com/50883639/173636626-ad51a935-2746-480b-a13d-00660e5ae867.png) | 
|:--:|
| Call function max with a and b as its arguments |

## Object types

There are different subtypes of objects defined, which differ by data stored and operations defined for them. Some of those are

* Integers
* Doubles
* Booleans
* String - simple implementation, with no escape characters; currently suitable for storing simple messages
* References - operations will usually not be performed on a Reference itself, but on an Object it's pointing at
* Array - may store multiple objects; Array object methods allow checking its size, filtering contents based on a predicate etc.

### Custom Objects 

Custom classes may be defined by the user, by providing their constructors and methods in the source file.
Any function definition starting with `constructor SampleClass`, will be treated as a constructor of SampleClass.
Function definition `function SampleClass.performX` will define a method performX of class SampleClass.
It's possible to overload constructors and methods just like global functions, by providing functions with the same name, differentiated by number of arguments expected.
When calling constructor or a method, variable `this` is added to the active reference frame, to allow access to object's other members.
Call `this.set(v, 4)` would create field v inside current object, and assign Integer 4 to it.
Method `str()` can be defined, to provide custom String representation of the object.

## Storing variables

Variables are stored in Reference Frames, which are maps, in which variable names are keys, and Objects they point at are values.
Each function call creates a frame and puts it on the stack, so that variables, both locally declared and passed as arguments, don't clash with variables from other scopes.
Assignment operation takes as operands a Reference object, and value to be assigned to its name. A link between them is then created in the active frame, from which the value can be retrieved
when other operation is being performed on a Reference object.

## Reference counting

As simple objects, such as Integers or Doubles, are only distinguishable from other objects of their type by their value,
two objects of the same type, storing the same value, are for all purposes identical. It is therefore possible for all References to given value to point at the same object,
as long as none of them tries to modify it. To achieve that, changing value of Reference does not modify underlying object, but rather makes it point at another Object with expected value.
This mechanism greatly reduces time and space spent on constructing redundant objects. 
It also, however, presents a challenge. To avoid dereferencing deallocated objects and memory leaks, an object must be destroyed exactly when the last Reference pointing thereat is destroyed.
Each object must therefore keep track of number of References pointing at it. This number goes up during assignment, and goes down when Reference starts pointing at another object, or when it goes out of scope.

## Function calls

Each function is stored as a vector of strings, representing names of its consecutive arguments, and a Block representing function body.
As arguments are untyped, functions can only be differenciated by their names, and number of arguments they expect.
It allows however for function overloading, by providing functions with the same name, taking different number of arguments.

Methods are grouped by class name, so that they don't clash with methods of other types. For this purpose global functions are treated like methods of a class with empty name.
When function, method, or a constructor is called, first step is finding fitting function definition in correct namespace.
If it's found, a new Reference Frame is put onto the Reference Stack, in which all local variables will be stored.
Each argument is paired with its value, and stored in the Frame.
For methods and constructors, Reference `this` is also stored, pointing at current instance.
Functions and methods may utilise `return()` function, to stop execution and return a value to the function caller.
After all instructions from the body are executed, or a `return()` call is encountered, Frame is popped out of the Stack, which results in decrementing reference counters of all local variables. 
Return value, or an Object(in case of constructors) is passed to the function caller, and execution of the program continues.

# Sample program

Sample code is provided in file source.txt

Checking execution order
```
print(2 + 3 * 5)
print(5 + 2 ** 3 * 4)
print((7+2) / 3)
```

Changing y should not change x
```
x = 5
y = x + 2
print(x, y)

x = y
y-= 4
print(x, y)
```

Function f should not modify value of x from global scope
```
function f(x) {
  print('function f called with x=' + x)
  x *= 5
  print('returning ' + x)
  return(x)
}

z = f(x)
print(x, z)
```

Logical operators should short circuit, and not call function g
```
function g() {
print('Function g called')
return(0)
}

print('Function g should not be called if logical operations short circuit')
p = 3 > 2 || g()
p = 4 > 7 && g()
```

Function overloading
```
function sum(x, y){
return(x +y)
}

function sum(x, y, z){
return(x+y+       z)
}

print(sum(4,6))
print(sum(63, 2*3, 8**2))
```

Find count of primes in given range
```
function isPrime(x) {
  if (x == 0 || x == 1) {
    return(0)
  }
  if (x == 2 || x == 3) {
    return(1)
  }
  if (x % 2 == 0 || x % 3 == 0) {
    return(0)
  }
  i = 5
  c = sqrt(x)
  while(i <= c) {
    if ((x % i == 0) || (x % (i + 2) == 0)) {
      return(0)
	}
    i += 6
  }
  return(1)
}

i = 3
limit = 100000
primesFound = 1
while (i < limit){
primesFound += isPrime(i)
i+=2
}

print('There are ' + primesFound + ' primes less than ' + limit)
```

Array operations
```
print((1,7,6,3,4).length)
print((1,2,1,1,5,7,1,2).count(1))
print((1,7,6,3,4).filter(x -> x % 2).map(u -> u * 2))
```

Creating custom objects
```
constructor Point(x, y) {
  this.set(x, x)
  this.set(y, y)
}

function Point.str() {
  return('Point: ' + this.x + char(44) + ' ' + this.y)
}

function Point.distance() {
  return(sqrt(this.x ** 2 + this.y ** 2))
}

function Point.move(dx, dy) {
  this.set(x, this.x + dx)
  this.set(y, this.y + dy)
}

function Point.move(p) {
  this.set(x, this.x + p.x)
  this.set(y, this.y + p.y)
}

p = Point(2,5)
print(p)
p.move(0,3)
print(p)
p.move(Point(7,0-4))
print(p)
print('Distance from center: ' + p.distance())
```

| ![obraz](https://user-images.githubusercontent.com/50883639/173638481-a6a9a224-f261-4bfb-a092-e9ac8d0773ad.png) |
|:--:|
| Output of sample program |


