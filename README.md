# TankGame
Tank game written in C language.

> This code is developed in VSCode (Windows - WSL[Ubuntu]). Please open it with 
> VSCode if the indentation is not working on other editors. Windows style line
> endings may affect the indentation of the code.

Highlights:
- Code comments (short & concise)
- Function header comments (short & concise)
- Modularization of the code (Short functions), 40-60 max lines of code
- Line width (100 char max)
- Use of #define constants in macros.h (nothing is hard-coded elsewhere)
- Use of function pointers
- Proper indentation of the code
- Low cyclomatic complexity of the code
- No multiple returns, global variables
- No break, continue statements inside loops (switch statements are allowed)
- No code duplication
- Header file usage (logical grouping of functions into headers)
- Static functions (all methods not listed in headers must be static)
- Meaningful variable names and function names (the name itself indicates what it does without comments)
- Input validation according to the assignment specification
- Valgrind (0 memory leaks, 0 errors) during gameplay & initial stage of validating input
- Zero crashes (should meet all assignment spec requirements)
- Makefile dependencies are correctly listed (non-dependant headers not included)
- Performance improvement (Refer PERF comment) - Optional

## Code Stats
![](resources/codeStats.PNG)


## Demonstration
![](resources/TankGame.gif)