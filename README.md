# Align Points
The program takes 2 sets of 2 points as input and finds a matrix that maps the first set onto the second. All done in native C (the main motivation for doing it)

#How to use
- Make sure your compiler can find Header.h and Header.cpp or just copy them into your code.

- `#include "Header.h"`

- Create 4 `vector3`s A, B, Ap, Bp. A and B are your initial set of points, Ap and Bp are where they end up later. 

- Call the function ` findW( A,  B,  Ap,  Bp)`.  You have to be sure this is a linear transformation. Simply put if your transformation applied to a point on the line AB, the resultant must be on the line ApBp. The function returns a double3x3 object. That's it.

##Stuff you might need to use:
###`double3x3::getij(int i,int j)`:
This lets you get the elements safely! I could have made things `private` but eh. Note that in this case the matrix is numbered like this:
```
[11 12 13]
[21 22 23]
[31 32 33]
```
just like in math so to get element 11 you do `mymatrix.getij(1,1)` or use `mymatrix.matrix[0][0]`. Simple.

###`vector3::vector3(double x, double y)`:
The constructor doesn't let you specify the 3rd value! Why! This is because we are not in 3D mode but are actually using homogeneous coordinates. w == 1 for points, w == 0 for vectors. This helps.

That's all!

#Stuff
Source.cpp is a demo. Just compile and follow the instructions on screen.
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
Really neat and nice. The guessed matrix is
```
11.126              -268.74             0
7.8739              379.74              0
94                  56                  1
```
You will find that A will produce (113, 167, 1) and  (112.9999,	167, 1) respectively when multiplied with either matrix.
The floating point errors can't be avoided. In Source.cpp you can change epsilon for the demo which governs the precision you need. If its too inaccurate you'll find yourself being warned. If you find the test being triggered many times for the value of epsilon you set then this may not be suitable for your needs.

Just note that this warning is in Source.cpp when you know the answer you're trying to get! Life is hardly that easy.

#ToDo
- Maybe extend it to 3D.
- Need to run more tests.
- Draw some stuff with it!
