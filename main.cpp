#include <iostream>
#include "RecommenderSystem.h"

int main()
{
    RecommenderSystem r;
    r.loadData("C:\\HUJI computer projects\\huji_C\\cpp4\\movies_big.txt","C:\\HUJI computer projects\\huji_C\\cpp4\\ranks_big.txt");
    std::string a = r.recommendByContent("Melody");
    std::cout << a << std::endl;
    double b = r.predictMovieScoreForUser("Twilight", "Melody", 2);
    std::cout << b << std::endl;
    b = r.predictMovieScoreForUser("Titanic", "Melody", 2);
    std::cout << b << std::endl;
    std::cout << r.recommendByCF("Melody", 30) << std::endl;
    return 1;
}
