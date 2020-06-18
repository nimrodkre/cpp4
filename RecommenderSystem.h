//
// Created by User on 18/06/2020.
//

#ifndef CPP4_RECOMMENDERSYSTEM_H
#define CPP4_RECOMMENDERSYSTEM_H

#include <unordered_map>
#include <map>
#include <vector>
#include <unordered_set>
#include <string>

#define FAIL -1
#define SUCCESS 0

typedef struct userMovieRank
{
    std::string movie;
    double rank;
}userMovieRank;

class RecommenderSystem
{
private:
    std::unordered_map<std::string, std::vector<double>> _moviesChar;
    std::unordered_map<std::string, std::vector<userMovieRank>> _userRank;

    int _readMovies(char const* moviesAttributesFilePath);
    int _readUserRanks(char const* userRanksFilePath);
    std::string _getContentRecommendation(const std::string &name);
    std::unordered_map<std::string, double> _getMoviesSimilarity(std::string &movieName, std::string name);
    double _predictMovieScoreForUser(std::string &movieName, std::string &userName, const int k);
    double _movieScore(std::unordered_map<std::string, double> kLargest, std::string &name);
public:
    int loadData(char const* moviesAttributesFilePath, char const* userRanksFilePath);
    std::string recommendByContent(const std::string &userName);
    double predictMovieScoreForUser(std::string movieName, std::string userName, const int k);
};


#endif //CPP4_RECOMMENDERSYSTEM_H
