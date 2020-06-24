#include <iostream>
#include "RecommenderSystem.h"

int main()
{
    RecommenderSystem r;
    std::string movies = "C:\\HUJI computer projects\\huji_C\\cpp4\\movies_features.txt";
    std::string movies2 = "C:\\HUJI computer projects\\huji_C\\cpp4\\ranks_matrix.txt";
    r.loadData(movies,movies2);
//    std::string a = r.recommendByContent("Arik");
//    std::cout << a << std::endl;
    //double b = r.predictMovieScoreForUser("Twilight", "Melody", 2);
    //std::cout << b << std::endl;
    //double b = r.predictMovieScoreForUser("DeepRed", "Christopher", 30);
    //std::cout << b << std::endl;
    std::cout << r.recommendByCF("Arik", 30) << std::endl;
    return 1;
}
