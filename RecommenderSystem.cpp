//
// Created by User on 18/06/2020.
//

#include "RecommenderSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <istream>

#define BAD_FILE "Unable to open file "
#define NA "NA"
#define NA_VALUE -1

int RecommenderSystem::loadData(char const* moviesAttributesFilePath, char const* userRanksFilePath)
{
    if (readMovies(moviesAttributesFilePath) == FAIL)
    {
        std::cerr << BAD_FILE << moviesAttributesFilePath << std::endl;
        return FAIL;
    }

    if (readUserRanks(userRanksFilePath) == FAIL)
    {
        std::cerr << BAD_FILE << userRanksFilePath << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

int RecommenderSystem::readMovies(char const* moviesAttributesFilePath)
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
        movieType movie = {"", {}};
        int iteration = 0;
        for (std::string s; iss >> s;)
        {
            if (iteration == 0)
            {
                movie.name = s;
            }
            else
            {
                movie.characteristics.push_back(stoi(s));
            }
            iteration++;
        }
        this->movies.push_back(movie);
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

int RecommenderSystem::readUserRanks(char const* userRanksFilePath)
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
            }
            else
            {
                userMovieRank movieRank;
                movieRank.movie = movieList[iteration - 1];
                if (s == NA)
                {
                    movieRank.rank = NA_VALUE;
                }
                else
                {
                    movieRank.rank = stoi(s);
                }
                movieRanks.push_back(movieRank);
            }
            iteration++;
        }
        this->userRank.insert({name, movieRanks});
    }

    return SUCCESS;
}

