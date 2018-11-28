#include "dog.h"

int main(int argc, char **argv)
{
    dog *bob = new(dog, "Bob");
    dog *bill = new(dog, "Bill");

    bob->print();
    bob->bark(20);
    bob->print();

    bill->print();

    unsigned int i;
    for (i = 0; i < 300; i++)
    {
        bill->my_tail->wag();
    }

    bill->print();

    destroy(bill);
    destroy(bob);

    return 0;
}
