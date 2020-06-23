#include <iostream>
#include "RecommenderSystem.h"

int main()
{
    RecommenderSystem r;
    std::string movies = "C:\\Users\\User\\Downloads\\Ex5 files-20200622\\movies_features.txt";
    std::string movies2 = "C:\\Users\\User\\Downloads\\Ex5 files-20200622\\ranks_matrix.txt";
    r.loadData(movies,movies2);
    std::string a = r.recommendByContent("Arik");
    std::cout << a << std::endl;
    //double b = r.predictMovieScoreForUser("Twilight", "Melody", 2);
    //std::cout << b << std::endl;
    //double b = r.predictMovieScoreForUser("DeepRed", "Christopher", 30);
    //std::cout << b << std::endl;
//    std::cout << r.recommendByCF("Christopher", 30) << std::endl;
    return 1;
}
