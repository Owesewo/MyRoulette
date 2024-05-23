#include <iostream>
#include <conio.h>
#include "DrawEngine.h"

int main()
{
    setlocale(LC_ALL, "Rus");
    //system("mode con cols=100 lines=20");
    system("title European roulette");
    system("color 27");

    DrawEngine de;
    de.Main();
}
