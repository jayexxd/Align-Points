# Align Points
The program takes 2 sets of 2 points as input and finds a matrix that maps the first set onto the second. All done in native C (the main motivation for doing it)

#Stuff
Header.h and Header.cpp contain the actual useful code. Source.cpp is a demo. Just compile and follow the instructions on screen.
Basically it lets you enter a pair of points, generates a random matrix and transforms those 2 points, then uses the stuff in Header.cpp to guess the matrix.

Don't be surprised if you get hugely differing matrices!
You can check it for example:
I input 
```
1
1
```
For A, and the generated matrix is 
```
12                  14                  51
7                   97                  68
94                  56                  13
```
Really neat and nice. The guesses matrix is
```
11.126              -268.74             0
7.8739              379.74              0
94                  56                  1
```
You will find that A will produce (113, 167, 1) and  (112.9999,	167, 1) respectively when multiplied with either matrix.
The floating point errors can't be avoided. In Source.cpp you can change epsilon for the demo which governs the precision you need. 
If you find the test being triggered many times for the value of epsilon you set then this may not be suitable for your needs.

Also you don't have to worry about the homogeneous coordinates. They're safe and auto.

# ToDo
Draw the points on screen
Display more decimal points lol
Idk if anything will break it
