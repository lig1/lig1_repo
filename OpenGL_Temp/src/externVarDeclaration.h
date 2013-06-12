#ifndef EXTERN_VAR_DECL_H
#define EXTERN_VAR_DECL_H

extern int i; // This is like a "function prototype", only
// for variables.
extern int arr[4];

// The extern declaration says, "this variable doesn't exist yet,
// but it WILL exist, and its value is going to be defined
// in another file that will be part of this project when it compiles."
// (see externVarDefinition.cpp)

// So, using an EXTERN is how you share a single global variable
// ACROSS MULTIPLE FILES.
// Call it a SUPERGLOBAL.  TO understand this, think about
// functions and function prototypes.

// Defining a global function, you kind of expect it to have
// "superglobal" status..
// like, if you define a function somewhere, you should be
// able to break it apart into some prototypes and #include
// those prototypes in ANY FILE that you want
// to use those functions.  Right?

// So, the reason you have prototypes is so that multiple
// files can #include the same prototypes (function
// DECLARATIONS) then the actual code for the
// DEFINITION of the functions is in one place,
// a .cpp file (like "function_set.cpp").  This way you don't
// have multiple re-definitions of the functions declared in
// function_set.h, but you may have multiple re-declarations
// (the compiler might see the same prototype several times
// in a row, which is fine, as long as it sees the BODY
// definition only once.)

// So, an extern'd variable is a lot like that.
// You basically treat it like a function prototype, where the

///// extern int i ;

// part is the DECLARATION (like a function prototype),
// and the part in externVarDefinition.cpp:

// int i = 500 ;

// is like the DEFINITION, or function body.

// C++ can only come across the DEFINITION (value giving part) __ONCE__ for any
// given variable.

// So with externs, you can #include the "extern variable declaration" into as
// many files as you like (just like you can #include the function prototypes
// into as many different files as you wish), so long as
// the "DEFINITION" (the int i = 500 part) only occurs once, in one file.

// If you uncomment this line below, you will see
// its ILLEGAL.

//////////int j = 50 ; ////ILLEGAL

// Surprised?  Well, very illegal!

// Reason:  EVEN WITH the #ifndef #include guards
// on this file.. because this "externVarDeclaration.h" file is
// #included in more than one other file, for some reason the C++
// LINKER (__NOT__ the compiler) will flag it as an error:

// "int j" already defined in main.obj.

// One day I'd like to be able to fully understand why this is
// myself, but for now, "EXTERN solves this problem".

#endif //EXTERN_VAR_DECL_H