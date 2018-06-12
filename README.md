# Align Points
The program takes 2 sets of 2 points as input and finds a matrix that maps the first set onto the second.

It starts by randomly generating a transform matrix and transforming the points you entered. Then it queries the function for a transform matrix that will do the exact same transform and spits it out. Rarely will the two matrices match, but we provide a 100% cashback gurantee on errorneous answers! i.e. it works.

It is simple to modify it to query you for the target coordinates instead of using a random matrix to transform them. This makes it more useful. Originally it was made for this but it's easier to demonstrate with the random matrix thing inside.

