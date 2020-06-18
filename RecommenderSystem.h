//
// Created by User on 18/06/2020.
//

#ifndef CPP4_RECOMMENDERSYSTEM_H
#define CPP4_RECOMMENDERSYSTEM_H

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <string>

#define FAIL -1
#define SUCCESS 0

typedef struct movieType
{
    std::string name;
    std::vector<int> characteristics;
}movieType;

typedef struct userMovieRank
{
    std::string movie;
    int rank;
}userMovieRank;

class RecommenderSystem
{
private:
    std::vector<movieType> movies;
    std::unordered_map<std::string, std::vector<userMovieRank>> userRank;
    int readMovies(char const* moviesAttributesFilePath);
    int readUserRanks(char const* userRanksFilePath);
public:
    int loadData(char const* moviesAttributesFilePath, char const* userRanksFilePath);
};


#endif //CPP4_RECOMMENDERSYSTEM_H
