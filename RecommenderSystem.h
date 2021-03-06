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

#include <unordered_map>
#include <vector>
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
     * unordered_map with movie and all of its charactersitics
     */
    std::unordered_map<std::string, std::vector<double>> _moviesChar;
    /**
     * unordered_map with name of the user and vector with all of his ranks for movie
     */
    std::unordered_map<std::string, std::vector<userMovieRank>> _userRank;
    /**
     * holds map with all of the movies and their normal
     */
    std::unordered_map<std::string, double> _movieNormal;
    /**
     * holds map with all of the movies and the angles between them
     */
    std::unordered_map<std::string, std::unordered_map<std::string, double>> _anglesBetweenMovies;
    /**
     * unordered_map with name of the user and movie map to the movie
     */
    std::unordered_map<std::string, std::unordered_map<std::string, double>> _MovieRanks;
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
     * @return a unordered_map with the movie and similarity to the given movie name
     */
    std::unordered_map<std::string, double> _getMoviesSimilarity(const std::string &movieName, const std::string &name);
    /**
     * finds the score of the movie according to the algorithm of the targil
     * @param kLargest k movies to check with
     * @param name name of the suer
     * @return double with the score of the movie
     */
    double _movieScore(const std::unordered_map<std::string, double> &similarity, const std::string &name, int k);
    /**
    * calculates the similarity of the 2 vectors according to the equation given
    * @param vec1 the first vector
    * @param vec2 the second vector
    * @return the similarity
    */
    double _getSimilarity(const std::vector<double> &vec1, const std::vector<double> &vec2, const std::string &movie);
    /**
     * finds the recommended movies for the user from the given data
     * @param userPref the users preferences
     * @param movies the movies
     * @param userRank the ranks of the user
     * @return the movie recommended
     */
    std::string _getMovieRecommended(const std::vector<double> &userPref,
                                     std::unordered_map<std::string, std::vector<double>> &movies,
                                     const std::vector<userMovieRank> &userRank);
    /**
     * builds a vector with all of the movies from the given stream
     * @param fs stream of data from file
     * @return vector with all of the movies
     */
    static std::vector<std::string> getMovies(std::ifstream &fs);
    /**
     * dot product of the given vectors
     * @param vec1 vector 1
     * @param vec2 vector 2
     * @return the dot product of both
     */
    static double dotProduct(const std::vector<double> &vec1, const std::vector<double> &vec2);
    /**
     * calculates the normal of the given vector
     * @param vec the vector to normalize
     * @return the normal of the vector
     */
    static double normal(const std::vector<double> &vec);
    /**
     * normalizes all of the users ranks, according the first step of the given algorithm in 3.2
     * @param userRank the users ranks
     * @return new vector with the given ranks
     */
    static std::vector<userMovieRank> &normalizeUser(std::vector<userMovieRank> &userRank);
    /**
     * calculates users preference
     * @param userRank the users ranks
     * @param movies the movies
     * @return all of the users preferences
     */
    static std::vector<double> getUserPreference(std::vector<userMovieRank> &userRank,
                                                 std::unordered_map<std::string,
                                                 std::vector<double>> movies);
    /**
     * function to help our sort
     * @param a first pair
     * @param b second pair
     * @return how to sort them
     */
    static bool sortBySimilarity(const std::pair<std::string, double> &a, const std::pair<std::string, double> &b);
    /**
     * sorts the given unordered_map by value
     * @param m the unordered_map to sort
     * @return a vector with pairs after we sorted
     */
    static std::vector<std::pair<std::string, double>> sortByValue(const std::unordered_map<std::string, double> &m);
public:
    /**
     * in charge of loading user data
     * @param moviesAttributesFilePath
     * @param userRanksFilePath
     * @return
     */
    int loadData(const std::string &moviesAttributesFilePath, const std::string &userRanksFilePath);
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
    double predictMovieScoreForUser(const std::string &movieName, const std::string &userName, int k);
    /**
     * finds the recommended movie according to the CH algorithm
     * @param userName the user name of the wanted person who wants recommendation
     * @param k k movie to check withthe movie recommended
     * @return the movie recommended
     */
    std::string recommendByCF(const std::string &userName, int k);
};


#endif //CPP4_RECOMMENDERSYSTEM_H
