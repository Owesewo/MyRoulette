#include "stdafx.h"

int main()
{
    setlocale(LC_ALL, "Rus");
    system("title European roulette");
    system("color 27");

    DrawEngine de;
    de.Main();
}
