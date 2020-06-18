#include <iostream>
#include "RecommenderSystem.h"

int main()
{
    RecommenderSystem r;
    r.loadData("C:\\HUJI computer projects\\huji_C\\cpp4\\movies_features.txt","C:\\HUJI computer projects\\huji_C\\cpp4\\ranks_matrix.txt");
    std::string a = r.recommendByContent("Sofia");
    std::cout << a << std::endl;
    double b = r.predictMovieScoreForUser("Twilight", "Nicole", 2);
    std::cout << b << std::endl;
    return 1;
}
