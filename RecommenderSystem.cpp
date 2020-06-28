/**
 * @file RecommenderSystem.cpp
 * @author  Nimrod Kremer
 * @version 1.0
 * @date 26.5.2020
 *
 * @brief In charge of the recommendation system
 *
 * @section LICENSE
 * This program is not a free software; bla bla bla...
 *
 * @section DESCRIPTION
 * In charge of recommendation algorithm
 * Input  : user data for recommendation
 * Process: independant functions
 * Output : Notification.
 */

#include "RecommenderSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <istream>
#include <algorithm>
#include <cmath>

/**
 * when a file is bad and not able to open correctly
 */
#define BAD_FILE "Unable to open file "
/**
 * How NA looks in the file
 */
#define NA "NA"
/**
 * The message to give the user if the user wasn't found
 */
#define NO_USER "USER NOT FOUND"
/**
 * The value to give the number if it's NA
 */
#define NA_VALUE INT32_MAX
/**
 * what to return when the funtion failed
 */
#define FAIL -1

/**
 * in charge of loading user data
 * @param moviesAttributesFilePath
 * @param userRanksFilePath
 * @return
 */
int RecommenderSystem::loadData(const std::string &moviesAttributesFilePath, const std::string &userRanksFilePath)
{
    if (_readMovies(moviesAttributesFilePath.c_str()) == FAIL)
    {
        std::cerr << BAD_FILE << moviesAttributesFilePath << std::endl;
        return FAIL;
    }

    if (_readUserRanks(userRanksFilePath.c_str()) == FAIL)
    {
        std::cerr << BAD_FILE << userRanksFilePath << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

/**
 * Reads the given movie paths to our data structure
 * @param moviesAttributesFilePath path to the file
 * @return fail to success
 */
int RecommenderSystem::_readMovies(char const *moviesAttributesFilePath)
{
    std::ifstream fs(moviesAttributesFilePath);
    fs = std::ifstream(moviesAttributesFilePath);

    if (!fs || !fs.is_open() || !fs.good())
    {
        return FAIL;
    }

    std::string line;
    // run each line
    while (std::getline(fs, line))
    {
        std::istringstream iss(line);
        int iteration = 0;
        std::string movieName;
        std::vector<double> characteristics;
        double normal = 0;
        // run for each word in a line with space between them
        for (std::string s; iss >> s; )
        {
            if (iteration == 0)
            {
                movieName = s;
            }
            else
            {
                double num = std::stod(s);
                normal += std::pow(num, 2);
                characteristics.push_back(num);
            }
            iteration++;
        }
        this->_movieNormal.insert({movieName, std::sqrt(normal)});
        this->_moviesChar.insert({movieName, characteristics});
    }
    fs.close();
    return SUCCESS;
}

/**
 * builds a vector with all of the movies from the given stream
 * @param fs stream of data from file
 * @return vector with all of the movies
 */
static std::vector<std::string> getMovies(std::ifstream &fs)
{
    std::string line;
    std::vector<std::string> movieList;
    std::getline(fs, line);
    std::istringstream iss(line);

    for (std::string s; iss >> s; )
    {
        movieList.push_back(s);
    }

    return movieList;
}

/**
 * reads the users ranks from the given file path
 * @param userRanksFilePath the path to the file
 * @return success or fail
 */
int RecommenderSystem::_readUserRanks(char const *userRanksFilePath)
{
    std::ifstream fs(userRanksFilePath);
    fs = std::ifstream(userRanksFilePath);

    if (!fs || !fs.is_open() || !fs.good())
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
        for (std::string s; iss >> s; )
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
                    movieRank.rank = std::stod(s);
                }
                this->_MovieRanks[name][movieRank.movie] = movieRank.rank;
                movieRanks.push_back(movieRank);
            }
            iteration++;
        }
        this->_userRank.insert({name, movieRanks});

    }
    fs.close();
    return SUCCESS;
}

/**
 * dot product of the given vectors
 * @param vec1 vector 1
 * @param vec2 vector 2
 * @return the dot product of both
 */
static double dotProduct(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
    double sum = 0;
    for (size_t i = 0; i < vec1.size(); i++)
    {
        sum += vec1[i] * vec2[i];
    }

    return sum;
}
/**
 * calculates the normal of the given vector
 * @param vec the vector to normalize
 * @return the normal of the vector
 */
static double normal(const std::vector<double> &vec)
{
    double sum = 0;
    for (auto num: vec)
    {
        sum += std::pow(num, 2);
    }

    return std::sqrt(sum);
}
/**
 * calculates the similarity of the 2 vectors according to the equation given
 * @param vec1 the first vector
 * @param vec2 the second vector
 * @return the similarity
 */
double RecommenderSystem::_getSimilarity(const std::vector<double> &vec1, const std::vector<double> &vec2,
                                         const std::string &movie)
{
    double curVal = dotProduct(vec1, vec2);
    curVal /= (normal(vec1) * _movieNormal[movie]);
    return curVal;
}

/**
 * normalizes all of the users ranks, according the first step of the given algorithm in 3.2
 * @param userRank the users ranks
 * @return new vector with the given ranks
 */
static std::vector<userMovieRank> &normalizeUser(std::vector<userMovieRank> &userRank)
{
    double sum = 0;
    int num = 0;
    for (auto &it: userRank)
    {
        if (it.rank != NA_VALUE)
        {
            sum += it.rank;
            num++;
        }
    }
    double avg =  sum / num;
    for (auto &it: userRank)
    {
        if (it.rank != NA_VALUE)
        {
            it.rank -= avg;
        }
    }
    return userRank;
}

/**
 * calculates users preference
 * @param userRank the users ranks
 * @param movies the movies
 * @return all of the users preferences
 */
static std::vector<double>
getUserPreference(std::vector<userMovieRank> &userRank, std::unordered_map<std::string, std::vector<double>> movies)
{
    // holds the vectors after multiplied by the scalar of the rank
    std::vector<std::vector<double>> userPref;
    for (auto &it: userRank)
    {
        if (it.rank != NA_VALUE)
        {
            std::vector<double>pref = movies[it.movie];
            for (size_t i = 0; i < pref.size(); i++)
            {
                pref[i] *= it.rank;
            }
            userPref.push_back(pref);
        }
    }

    // now add all vectors together in userPref
    std::vector<double> retPref;
    // at least 1 movie that the user likes
    for (size_t i = 0; i < userPref[0].size(); i++)
    {
        double num = 0;
        for (auto pref: userPref)
        {
            num += pref[i];
        }
        retPref.push_back(num);
    }

    return retPref;
}

/**
 * finds the recommended movies for the user from the given data
 * @param userPref the users preferences
 * @param movies the movies
 * @param userRank the ranks of the user
 * @return the movie recommended
 */
std::string RecommenderSystem::_getMovieRecommended(const std::vector<double> &userPref,
                                                    std::unordered_map<std::string, std::vector<double>> &movies,
                                                    const std::vector<userMovieRank> &userRank)
{
    double maxVal = INT8_MIN;
    std::string bestMovie;
    for (auto &it: userRank)
    {
        if (it.rank == NA_VALUE)
        {
            double curVal = _getSimilarity(userPref, movies[it.movie], it.movie);
            if (curVal > maxVal)
            {
                maxVal = curVal;
                bestMovie = it.movie;
            }
        }
    }

    return bestMovie;
}

/**
 * finds the content best suited for the user
 * @param name the name of the user
 * @return the movie best fit for the given user
 */
std::string RecommenderSystem::_getContentRecommendation(const std::string &name)
{
    std::unordered_map<std::string, std::vector<double>> movies = _moviesChar;
    std::vector<userMovieRank> userRank = _userRank[name];
    userRank = normalizeUser(userRank);
    std::vector<double> userPref = getUserPreference(userRank, movies);
    return _getMovieRecommended(userPref, movies, userRank);
}

/**
 * finds the recommended movie for the user
 * @param userName the user name to check
 * @return the movie recommneded
 */
std::string RecommenderSystem::recommendByContent(const std::string &userName)
{
    if (_userRank.find(userName) == _userRank.end())
    {
        return NO_USER;
    }

    return _getContentRecommendation(userName);
}

/**
 * finds the movies most similiar for the user and the movie.
 * We want to save the angle between movies as they are constant for the full run of the
 * program.
 * So if we haven't calculated the angle, than we will have to calculate and enter it to our map
 * with saved angles.
 * If it was calculated already, than just take it out of the map
 * @param movieName the movie to check
 * @param name the user
 * @return a unordered_map with the movie and similarity to the given movie name
 */
std::unordered_map<std::string, double> RecommenderSystem::_getMoviesSimilarity(const std::string &movieName,
                                                                                const std::string &name)
{
    std::vector<userMovieRank> userRank = _userRank[name];
    std::unordered_map<std::string, double> similarity = {};
    auto res = _anglesBetweenMovies.find(movieName);
    for (auto &it: userRank)
    {
        double angle = 0;

        if (res == _anglesBetweenMovies.end())
        {
            angle = _getSimilarity(_moviesChar[it.movie], _moviesChar[movieName], movieName);
            _anglesBetweenMovies[it.movie][movieName] =  angle;
            _anglesBetweenMovies[movieName][it.movie] =  angle;
        }
        else
        {
            auto res2 = _anglesBetweenMovies[movieName].find(it.movie);
            if (res2 == _anglesBetweenMovies[movieName].end())
            {
                angle = _getSimilarity(_moviesChar[it.movie], _moviesChar[movieName], movieName);
                _anglesBetweenMovies[it.movie][movieName] =  angle;
                _anglesBetweenMovies[movieName][it.movie] =  angle;
            }
            else
            {
                angle = _anglesBetweenMovies[it.movie][movieName];
            }
        }


        if (it.rank != NA_VALUE && it.movie != movieName)
        {

            similarity.insert({it.movie, angle});
        }
    }
    return similarity;
}

/**
 * function to help our sort
 * @param a first pair
 * @param b second pair
 * @return how to sort them
 */
static bool sortBySimilarity(const std::pair<std::string, double> &a,
                             const std::pair<std::string, double> &b)
{
    return (a.second > b.second);
}

/**
 * sorts the given unordered_map by value
 * @param m the unordered_map to sort
 * @return a vector with pairs after we sorted
 */
static std::vector<std::pair<std::string, double>> sortByValue(const std::unordered_map<std::string, double> &m)
{
    std::vector<std::pair<std::string, double>> unordered_mapVector;
    // Insert entries
    for (auto &iterator : m)
    {
            unordered_mapVector.emplace_back(iterator);
    }
    sort(unordered_mapVector.begin(), unordered_mapVector.end(), sortBySimilarity);

    return unordered_mapVector;
}

/**
 * finds the score of the movie according to the algorithm of the targil
 * @param kLargest k movies to check with
 * @param name name of the suer
 * @return double with the score of the movie
 */
double RecommenderSystem::_movieScore(const std::unordered_map<std::string, double> &similarity,
                                      const std::string &name, int k)
{
    double numerator = 0;
    double denominator = 0;
    std::vector<std::pair<std::string, double>> sim;
    sim = sortByValue(similarity);

    int iterations = 0;
    // final calculation for the movie score
    for (auto &it : sim)
    {
        if (iterations < k)
        {
            numerator += it.second * _MovieRanks[name][it.first];
            denominator += it.second;
        }

        iterations++;
    }
    return numerator / denominator;
}

/**
 * predicts the movie score for the user
 * @param movieName the movie name
 * @param userName the name of the user
 * @param k number of movie to check with
 * @return the score given
 */
double RecommenderSystem::predictMovieScoreForUser(const std::string &movieName, const std::string &userName, int k)
{
    if (_userRank.find(userName) == _userRank.end() || _moviesChar.find(movieName) == _moviesChar.end())
    {
        return FAIL;
    }
    std::unordered_map<std::string, double> movieSimilarity = _getMoviesSimilarity(movieName, userName);
    return _movieScore(movieSimilarity, userName, k);
}

/**
 * finds the recommended movie according to the CH algorithm
 * @param userName the user name of the wanted person who wants recommendation
 * @param k k movie to check withthe movie recommended
 * @return the movie recommended
 */
std::string RecommenderSystem::recommendByCF(const std::string &userName, int k)
{
    if (_userRank.find(userName) == _userRank.end())
    {
        return NO_USER;
    }

    std::string bestMovie;
    double bestMovieScore = INT8_MIN;
    // predict movie for all of the NA and check the maximum
    for (size_t i = 0; i < _userRank[userName].size(); i++)
    {
        if (_userRank[userName][i].rank == NA_VALUE)
        {
            double score = predictMovieScoreForUser(_userRank[userName][i].movie, userName, k);
            if (score != FAIL && score > bestMovieScore)
            {
                bestMovie = _userRank[userName][i].movie;
                bestMovieScore = score;
            }
        }

    }
    return bestMovie;
}