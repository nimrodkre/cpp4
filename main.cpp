#include <iostream>
#include "RecommenderSystem.h"

int main()
{
    RecommenderSystem r;
    r.loadData("C:\\HUJI computer projects\\huji_C\\cpp4\\movies_features.txt","C:\\HUJI computer projects\\huji_C\\cpp4\\ranks_matrix.txt");
    return 1;
}