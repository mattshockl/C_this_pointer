#include "this.h"
#include <stdio.h>

// declare our required classes and methods here
class_decl(dog);
class_decl(tail);

method_decl(dog, void, bark, (int));
method_decl(dog, void, print, (void));

method_decl(tail, void, wag, (void));
method_decl(tail, int, get_wags, (void));

// define our classes here
class_def(tail,
    {   
        // class members
        int total_wags;

        // class methods
        method(tail, wag);
        method(tail, get_wags);

        // required destructor
        class_destroy(tail);
    },
    (void), {   
        // class initializer
        this->total_wags = 0;

        method_init(tail, wag);
        method_init(tail, get_wags);
    },
    {   
        // class destructor
        method_destroy(tail, wag);
        method_destroy(tail, get_wags);
    }
)

class_def(dog,
    {   
        // class members
        int total_barks;
        char *name;
        tail *my_tail;

        // class methods
        method(dog, bark);
        method(dog, print);

        // required destructor
        class_destroy(dog);
    },
    (char *name), {
        // class initializer
        this->name = name;
        this->total_barks = 0;
        this->my_tail = new(tail);
        
        method_init(dog, bark);
        method_init(dog, print);
    },
    {
        // class destructor
        printf("You killed %s!\n", this->name);
        destroy(this->my_tail);

        method_destroy(dog, bark);
        method_destroy(dog, print);
    }
)

/* define our method bodies here */
method_def(dog,
    void, bark, (int num_times),
    {
        this->total_barks += num_times;
    }
)

method_def(dog,
    void, print, (void),
    {
        printf("%s barked %d times and wagged its tail %d times\n", this->name, this->total_barks, this->my_tail->get_wags());
    }
)

method_def(tail,
    void, wag, (void),
    {
        this->total_wags++;
    }
)

method_def(tail,
    int, get_wags, (void),
    {
        return this->total_wags;
    }
)