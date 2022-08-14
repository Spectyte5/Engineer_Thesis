#include <iostream>
#include "vector3D.h"

int main()
{

    Vector3D vector1(5, 4, 3), vector2(2, 3, 4);

    std::cout << "V1: " << vector1 << std::endl;
    std::cout << "V2: " << vector2 << std::endl;

    std::cout << "V1+V2: " << vector1 + vector2 << std::endl;
    std::cout << "V1-V2: " << vector1 - vector2 << std::endl;

    std::cout << "V1*4: " << vector1 * 4 << std::endl;
}