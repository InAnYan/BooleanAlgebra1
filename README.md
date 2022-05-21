## Description
Simple truth table generator.
WRITTEN FOR EDUCATIONAL PURPOSES.
## Output
    Boolean function truth table generator
    Copyright(C) 2022 Ruslan Popov <ruslanpopov1512@gmail.com>
    
    Enter operation:
    1. Generate truth table for one function
    2. Compare two functions
    3. Exit
    Enter number [1-3]: 2

    Functions must have same variables
    Enter first function: a*b+b*c+a*c
    Enter second function: a^b^c
    
         a*b+b*c+a*c                a^b^c
    ---------------------   ---------------------
    |   a|   b|   c|func|   |   a|   b|   c|func|
    |----|----|----|----|   |----|----|----|----|
    |   0|   0|   0|   0|   |   0|   0|   0|   0|
    |----|----|----|----|   |----|----|----|----|
    |   0|   0|   1|   0|   |   0|   0|   1|   1|
    |----|----|----|----|   |----|----|----|----|
    |   0|   1|   0|   0|   |   0|   1|   0|   1|
    |----|----|----|----|   |----|----|----|----|
    |   0|   1|   1|   1|   |   0|   1|   1|   0|
    |----|----|----|----|   |----|----|----|----|
    |   1|   0|   0|   0|   |   1|   0|   0|   1|
    |----|----|----|----|   |----|----|----|----|
    |   1|   0|   1|   1|   |   1|   0|   1|   0|
    |----|----|----|----|   |----|----|----|----|
    |   1|   1|   0|   1|   |   1|   1|   0|   0|
    |----|----|----|----|   |----|----|----|----|
    |   1|   1|   1|   1|   |   1|   1|   1|   1|
    ---------------------   ---------------------
## Syntax
- Variables must be one character long.
- All boolean operators must be written explicitly.
- OR: +
- XOR: ^
- AND: * , &
- NOT: ! , ~ , -
