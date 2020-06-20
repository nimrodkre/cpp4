/**
 * @file RecommenderSystem.h
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

#ifndef CPP4_RECOMMENDERSYSTEM_H
#define CPP4_RECOMMENDERSYSTEM_H

#include <map>
#include <map>
#include <vector>
#include <unordered_set>
#include <string>

/**
 * program failed
 */
#define FAIL -1
/**
 * program succeeded
 */
#define SUCCESS 0

/**
 * holds the movie and its rank
 */
typedef struct userMovieRank
{
    std::string movie;
    double rank;
}userMovieRank;

/**
 * class in charge of the recommendation system
 */
class RecommenderSystem
{
private:
    /**
     * map with movie and all of its charactersitics
     */
    std::map<std::string, std::vector<double>> _moviesChar;
    /**
     * map with name of the user and vector with all of his ranks for movie
     */
    std::map<std::string, std::vector<userMovieRank>> _userRank;

    /**
    * Reads the given movie paths to our data structure
    * @param moviesAttributesFilePath path to the file
    * @return fail to success
    */
    int _readMovies(char const* moviesAttributesFilePath);
    /**
     * reads the users ranks from the given file path
     * @param userRanksFilePath the path to the file
     * @return success or fail
     */
    int _readUserRanks(char const* userRanksFilePath);
    /**
     * finds the content best suited for the user
     * @param name the name of the user
     * @return the movie best fit for the given user
     */
    std::string _getContentRecommendation(const std::string &name);
    /**
     * finds the movies most similiar for the user and the movie
     * @param movieName the movie to check
     * @param name the user
     * @return a map with the movie and similarity to the given movie name
     */
    std::map<std::string, double> _getMoviesSimilarity(std::string &movieName, std::string name);
    /**
     * predicts the movie score for the user
     * @param movieName the movie name
     * @param userName the name of the user
     * @param k number of movie to check with
     * @return the score given
     */
    double _predictMovieScoreForUser(std::string &movieName, std::string &userName, const int k);
    /**
     * finds the score of the movie according to the algorithm of the targil
     * @param kLargest k movies to check with
     * @param name name of the suer
     * @return double with the score of the movie
     */
    double _movieScore(std::map<std::string, double> kLargest, std::string &name);
public:
    /**
     * in charge of loading user data
     * @param moviesAttributesFilePath
     * @param userRanksFilePath
     * @return
     */
    int loadData(std::string moviesAttributesFilePath, std::string userRanksFilePath);
    /**
     * finds the recommended movie for the user
     * @param userName the user name to check
     * @return the movie recommneded
     */
    std::string recommendByContent(const std::string &userName);
    /**
     * predicts the movie score for the user
     * @param movieName the movie name
     * @param userName the name of the user
     * @param k number of movie to check with
     * @return the score given
     */
    double predictMovieScoreForUser(std::string movieName, std::string userName, const int k);
    /**
     * finds the recommended movie according to the CH algorithm
     * @param userName the user name of the wanted person who wants recommendation
     * @param k k movie to check withthe movie recommended
     * @return the movie recommended
     */
    std::string recommendByCF(std::string userName, int k);
};


#endif //CPP4_RECOMMENDERSYSTEM_H
