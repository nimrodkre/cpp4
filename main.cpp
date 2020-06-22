#include <iostream>
#include "RecommenderSystem.h"

int main()
{
    RecommenderSystem r;
    std::string movies = "C:\\Users\\User\\Downloads\\Ex5 files-20200622\\movies_big.txt";
    std::string movies2 = "C:\\Users\\User\\Downloads\\Ex5 files-20200622\\ranks_big.txt";
    r.loadData(movies,movies2);
//    std::string a = r.recommendByContent("Reina");
//    std::cout << a << std::endl;
    //double b = r.predictMovieScoreForUser("Twilight", "Melody", 2);
    //std::cout << b << std::endl;
    //double b = r.predictMovieScoreForUser("DeepRed", "Christopher", 30);
    //std::cout << b << std::endl;
    std::cout << r.recommendByCF("Christopher", 30) << std::endl;
    return 1;
}
