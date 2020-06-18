//
// Created by User on 18/06/2020.
//

#include "RecommenderSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <istream>
#include <limits>
#include <cmath>

#define BAD_FILE "Unable to open file "
#define NA "NA"
#define NO_USER "USER NOT FOUND"
#define NA_VALUE INT32_MAX

int RecommenderSystem::loadData(char const *moviesAttributesFilePath, char const *userRanksFilePath)
{
    if (_readMovies(moviesAttributesFilePath) == FAIL)
    {
        std::cerr << BAD_FILE << moviesAttributesFilePath << std::endl;
        return FAIL;
    }

    if (_readUserRanks(userRanksFilePath) == FAIL)
    {
        std::cerr << BAD_FILE << userRanksFilePath << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

int RecommenderSystem::_readMovies(char const *moviesAttributesFilePath)
{
    std::ifstream fs(moviesAttributesFilePath);
    fs = std::ifstream(moviesAttributesFilePath);

    if (!fs || !fs.is_open())
    {
        return FAIL;
    }

    std::string line;
    while (std::getline(fs, line))
    {
        std::istringstream iss(line);
        int iteration = 0;
        std::string movieName;
        std::vector<double> characteristics;
        for (std::string s; iss >> s;)
        {
            if (iteration == 0)
            {
                movieName = s;
            } else
            {
                characteristics.push_back(atof(s.c_str()));
            }
            iteration++;
        }
        this->_moviesChar.insert({movieName, characteristics});
    }

    return SUCCESS;
}

static std::vector<std::string> getMovies(std::ifstream &fs)
{
    std::string line;
    std::vector<std::string> movieList;
    std::getline(fs, line);
    std::istringstream iss(line);

    for (std::string s; iss >> s;)
    {
        movieList.push_back(s);
    }

    return movieList;
}

int RecommenderSystem::_readUserRanks(char const *userRanksFilePath)
{
    std::ifstream fs(userRanksFilePath);
    fs = std::ifstream(userRanksFilePath);

    if (!fs || !fs.is_open())
    {
        return FAIL;
    }

    std::string line;
    std::vector<std::string> movieList = getMovies(fs);

    while (std::getline(fs, line))
    {
        std::istringstream iss(line);
        std::vector<userMovieRank> movieRanks;
        int iteration = 0;
        std::string name;
        for (std::string s; iss >> s;)
        {
            if (iteration == 0)
            {
                name = s;
            } else
            {
                userMovieRank movieRank;
                movieRank.movie = movieList[iteration - 1];
                if (s == NA)
                {
                    movieRank.rank = NA_VALUE;
                } else
                {
                    movieRank.rank = atof(s.c_str());
                }
                movieRanks.push_back(movieRank);
            }
            iteration++;
        }
        this->_userRank.insert({name, movieRanks});
    }

    return SUCCESS;
}

static double dotProduct(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
    double sum = 0;
    for (int i = 0; i < vec1.size(); i++)
    {
        sum += vec1[i] * vec2[i];
    }

    return sum;
}

static double normal(const std::vector<double> &vec)
{
    double sum = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        sum += vec[i] * vec[i];
    }

    return std::sqrt(sum);
}

static double getSimilarity(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
    double curVal = dotProduct(vec1, vec2);
    curVal /= (normal(vec1) * normal(vec2));
    return curVal;
}

static std::vector<userMovieRank> &normalizeUser(std::vector<userMovieRank> &userRank)
{
    int sum = 0;
    int num = 0;
    for (auto it = userRank.begin(); it != userRank.end(); it++)
    {
        if (it->rank != NA_VALUE)
        {
            sum += it->rank;
            num++;
        }
    }
    double avg = (double) sum / num;
    for (auto it = userRank.begin(); it != userRank.end(); it++)
    {
        if (it->rank != NA_VALUE)
        {
            it->rank -= avg;
        }
    }
    return userRank;
}

static std::vector<double>
getUserPreference(std::vector<userMovieRank> &userRank, std::unordered_map<std::string, std::vector<double>> movies)
{
    // holds the vectors after multiplied by the scalar of the rank
    std::vector<std::vector<double>> userPref;
    for (auto it = userRank.begin(); it != userRank.end(); it++)
    {
        if (it->rank != NA_VALUE)
        {
            std::vector<double>pref = movies[it->movie];
            for (int i = 0; i < pref.size(); i++)
            {
                pref[i] *= it->rank;
            }
            userPref.push_back(pref);
        }
    }

    // now add all vectors together in userPref
    std::vector<double> retPref;
    // at least 1 movie that the user likes
    for (int i = 0; i < userPref[0].size(); i++)
    {
        int num = 0;
        for (int j = 0; j < userPref.size(); j++)
        {
            num += userPref[j][i];
        }
        retPref.push_back(num);
    }

    return retPref;
}

std::string getMovieRecommended(const std::vector<double> &userPref,
        std::unordered_map<std::string, std::vector<double>> &movies,
        const std::vector<userMovieRank> &userRank)
{
    double maxVal = INT8_MIN;
    std::string bestMovie;
    for (auto it = userRank.begin(); it != userRank.end(); it++)
    {
        if (it->rank == NA_VALUE)
        {
            double curVal = getSimilarity(userPref, movies[it->movie]);

            if (curVal > maxVal)
            {
                maxVal = curVal;
                bestMovie = it->movie;
            }
        }
    }

    return bestMovie;
}

std::string RecommenderSystem::_getContentRecommendation(const std::string &name)
{
    std::unordered_map<std::string, std::vector<double>> movies = _moviesChar;
    std::vector<userMovieRank> userRank = _userRank[name];

    userRank = normalizeUser(userRank);
    std::vector<double> userPref = getUserPreference(userRank, movies);
    return getMovieRecommended(userPref, movies, userRank);
}

std::string RecommenderSystem::recommendByContent(const std::string &userName)
{
    if (_userRank.find(userName) == _userRank.end())
    {
        return NO_USER;
    }
    return _getContentRecommendation(userName);
}

std::unordered_map<std::string, double> RecommenderSystem::_getMoviesSimilarity(std::string &movieName, std::string name)
{
    std::vector<userMovieRank> userRank = _userRank[name];
    std::unordered_map<std::string, double> similarity = {};
    for (auto it = userRank.begin(); it != userRank.end(); it++)
    {
        if (it->rank != NA_VALUE)
        {
            similarity.insert({it->movie, getSimilarity(_moviesChar[it->movie], _moviesChar[movieName])});
        }
    }
    return similarity;
}

static double _predictMovieScoreForUser(std::string &movieName, std::string &userName, const int k)
{
    std::unordered_map<std::string, double> movieSimilarity;
}