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

The pointer union can hold pointers to any type, but will represent
them as `void*`s when they are not explicitly enumerated as holdable.

Example:

    int i = 2;
    double d = 3.14;
    const int ci = 5;

    ptrunion<int, double> p(&ci);

    int* ip = (int*) p;        // ip points to NULL
    double* dp = (double*) p;  // dp points to NULL
    void* vp = (void*) p;      // vp points to ci

Note that `int` and `const int` are treated as completely different
types, which is consistent with c++ in general.

Like the enhanced `enum`s in Swift, a pointer union allows you to
build `switch` statements that choose a case based on the type
currently pointed to by the union.

Example:

    int i = 2;
    double d = 3.14;
    ptrunion<int, double> p(&i);

    switch (p) {
    case ptrunion<int, double>::id_of<int>::value : {
      cout << "integer" << endl;
      break; }
    case ptrunion<int, double>::id_of<double>::value : {
      cout << "double" << endl;
      break; }
    default : {
      cout << "null" << endl;
      break; }
    }

All pointer unions can store `void*`s, so there is always a valid
branch called `ptrunion<...>::id_of<void>::value` that you got for
free with the types you specified. Be aware of that when attempting to
write exhaustive case lists.

