# Compile Time C++ Snake Game

Implementation of Tetris as a C++ template metaprogramming. All the logic is implemented at compile time. The runtime program is only needed to print out the game world and serialize it for next play.


## Playing
Plays one step of the game every time the game is recompiled. Uses compiler flags to control input:

```
$ clang++ -std=c++1z main.cpp -ftemplate-depth-5000 -Wgnu-string-literal-operator-template -D COMMAND -o tetris ; ./tetris
```

Valid commands are LEFT, RIGHT, LROT, and RROT, DROP, DOWN. Yoy can also enter no command.

### Compiling
* `-std=c++1z` since the program uses a few C++17 features.
* `-ftemplate-depth-5000` is just a guess. If you compiler supports fold expressions you can try using a lower value
* `-Wgnu-string-literal-operator-template` is used for generating compile time strings.
