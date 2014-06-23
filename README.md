pointer_union
=============

A simple structure that acts as a tagged union for pointers.

The key concept is to allow only a restricted set of types to be
pointed to, and to be able to cast the pointer union to any one of
those types; if the pointer union is cast to the last inserted type,
the actual pointer is safely retrieved, if not, you receive a null
pointer.

Example:

    int i = 2;
    double d = 3.14;
    ptrunion<int, double> p(&i); // constructs a pointer union pointing to i

    int* ip = (int*) p;          // ip points to i
    double* dp = (double*) p;    // dp points to NULL

    p = &d;                      // sets the pointer union to point to d

    ip = (int*) p;               // ip points to NULL
    dp = (double*) p;            // dp points to d

This allows for a limited polymorphic behavior that is different from
virtual classes in that it a) works on built-in types as well as
user-defined classes, and b) requires no unifying class hierarchy to
work.
