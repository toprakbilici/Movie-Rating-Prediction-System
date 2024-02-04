#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

using namespace std;
using std::cout;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::setw;
using std::sqrt;
using std::stringstream;
using std::tuple;

/**
 * @brief Get the dimensions of the matrix from a CSV file.
 *
 * @param filename The name of the CSV file.
 * @return tuple<int, int> A tuple containing the maximum user ID and movie ID.
 */
 
tuple<int, int> getMatrixDimensions(const std::string &filename) {
  ifstream input_file;
  int max_user_id = -1;
  int max_movie_id = -1;
  int user_id, movie_id;
  std::string ptr1, ptr2, ptr3, line;
  float rating;

  input_file.open(filename);
  input_file.ignore(20, '\n');

  if (!input_file.is_open()) {
    throw runtime_error("Could not open file");
  }

  while (getline(input_file, line)) {
    stringstream ss(line);

    getline(ss, ptr1, ',');
    user_id = stoi(ptr1);
    getline(ss, ptr2, ',');
    movie_id = stoi(ptr2);
    getline(ss, ptr3, ',');
    rating = stof(ptr3);

    if (user_id > max_user_id) {
      max_user_id = user_id;
    }

    if (movie_id > max_movie_id) {
      max_movie_id = movie_id;
    }
  }

  return std::make_tuple(max_user_id + 1, max_movie_id + 1);
}

/**
 * @brief Fill the matrix with ratings from a CSV file.
 *
 * @param filename The name of the CSV file.
 * @param rating_matrix The matrix to be filled.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 */
 
void fillMatrix(const std::string &filename, double **rating_matrix, size_t row,
                size_t col) {
  ifstream input_file;
  int user_id, movie_id;
  std::string ptr1, ptr2, ptr3, line;
  double rating;

  for (size_t i = 0; i < row; i++) {
    for (size_t j = 0; j < col; j++) {
      rating_matrix[i][j] = -1;
    }
  }

  input_file.open(filename);
  input_file.ignore(20, '\n');

  if (!input_file.is_open()) {
    throw runtime_error("Could not open file");
  }

  while (getline(input_file, line)) {
    stringstream ss(line);

    getline(ss, ptr1, ',');
    user_id = stoi(ptr1);
    getline(ss, ptr2, ',');
    movie_id = stoi(ptr2);
    getline(ss, ptr3, ',');
    rating = stof(ptr3);

    rating_matrix[user_id][movie_id] = rating;
  }
}
/**
 * @brief Perform insertion sort on an array of ratings along with their
 * indices.
 *
 * @param rating_array The array of ratings.
 * @param index_array The array of corresponding indices.
 * @param n The size of the arrays.
 */
 
void insertion_sort(double *rating_array, int *index_array, size_t n) {
  int j;
  double temp;

  for (size_t i = 1; i < n; ++i) {
    j = i;
    while (j > 1 && rating_array[j] < rating_array[j - 1]) {
      temp = rating_array[j];
      rating_array[j] = rating_array[j - 1];
      rating_array[j - 1] = temp;

      temp = index_array[j];
      index_array[j] = index_array[j - 1];
      index_array[j - 1] = temp;

      j--;
    }
  }
}
/**
 * @brief Print the top-rated movies.
 *
 * @param rating_matrix The matrix containing ratings.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 */
void print_top_movie(double **rating_matrix, size_t row, size_t col) {
	
  double* rating_counts = new double[col];
	int* index_array = new int[col];
	int i, j, count;
	double sum, rating; 
	
	for(i=0; i<=col; i++){
		index_array[i] = i;
	}
	
	rating_counts[0] = 0;
	for(i=1; i<col; i++){
		count = 0;
		
		for(j=1; j<row; j++){
			rating = rating_matrix[j][i];
			if(rating != -1){
				count = count + 1;
			}	
		}
		if(count != 0){
			rating_counts[i] = count;
		}
		
		else{
			rating_counts[i] = -1;
		}
	}

	insertion_sort(rating_counts, index_array, col);
	cout << "#";
	cout << setw (21);
	cout << "Top 10 Movies:";
	cout << setw (15);
	cout << "Counts:"<<endl;
	for(i=0; i<10; i++){
		cout << "#" << i+1;
		cout << setw (20);
		cout << index_array[col-1-i];
		cout << setw (15);
		cout << rating_counts[col-1-i] << "\n";
		
	}
}

/**
 * @brief Print the top-rated users.
 *
 * @param rating_matrix The matrix containing ratings.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 */
 
void print_top_user(double **rating_matrix, size_t row, size_t col) {
  double* user_counts = new double[row];
	int* index_array = new int[row];
	int i, j;
	double rating;
	int count;
	
	for(i=1; i<row; i++){
		index_array[i] = i;
	}
	
	for(i=1; i<row; i++){
		count = 0;
		for(j=1; j<col; j++){
			rating = rating_matrix[i][j];
			if(rating != -1){
				count = count + 1;
			}
		}
		
		if(count != 0){
			user_counts[i] = count;
		}
		else{
			user_counts[i] = -1;
		}
	}
	
	insertion_sort(user_counts, index_array, row);
	cout << "#";
	cout << setw (21);
	cout << "Top 10 Users:";
	cout << setw (15);
	cout << "Counts:"<<endl;
	for(i=0; i<10; i++){
		cout << "#" << i+1;
		cout << setw (20);
		cout << index_array[row-1-i];
		cout << setw (15);
		cout << user_counts[row-1-i] << "\n";
	}
}

/**
 * @brief Calculate cosine similarity between a user vector and all other users.
 *
 * @param user_vector The vector representing a user.
 * @param rating_matrix The matrix containing ratings.
 * @param col The number of columns in the matrix.
 * @param row The number of rows in the matrix.
 * @return double* An array of cosine similarities.
 */
 
double *cossim(double *user_vector, double **rating_matrix, size_t col,
               size_t row) {
  	int i,j;
	double temp1,temp2,numerator,denominator;
	double* cossim_vector = new double[row];
	cossim_vector[0] = -2;
	for(i=1; i<row; i++){
		numerator = 0;
		denominator = 0;
		temp1 = 0;
		temp2 = 0;
		for(j=1; j<col; j++){
			numerator += user_vector[j]*rating_matrix[i][j];
			temp1 += user_vector[j]*user_vector[j];
			temp2 += rating_matrix[i][j]*rating_matrix[i][j];
		}
		denominator = sqrt(temp1)*sqrt(temp2);
		cossim_vector[i] = numerator/denominator;
		//cout<<denominator<<"   "<<numerator<<"   "<<numerator/denominator<<endl;
	}	
	return cossim_vector;
}

/**
 * @brief Generate movie recommendations based on cosine similarity.
 *
 * @param filename The name of the test CSV file.
 * @param rating_matrix The matrix containing ratings.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 */
 
void movie_recom(const std::string &filename, double **rating_matrix,
                 size_t row, size_t col) {
  int id, user_id, movie_id;
  double val, rec_rating;
  ifstream input_file;
  ofstream output_file;
  output_file.open("submission.csv", std::ios::out | std::ios::app);
  output_file << "Id"
              << ","
              << "Predicted" << std::endl;

  input_file.open(filename);
  input_file.ignore(20, '\n');
  std::string ptr1, ptr2, ptr3, line;
  double *cossim_vector = new double[row];
  double *rec_rating_vector = new double[5000];

  if (!input_file.is_open()) {
    throw runtime_error("Could not open file");
  }

  size_t k = 0;
  while (getline(input_file, line)) {
    stringstream ss(line);
    getline(ss, ptr1, ',');
    id = stoi(ptr1);
    getline(ss, ptr2, ',');
    user_id = stoi(ptr2);
    getline(ss, ptr3, ',');
    movie_id = stoi(ptr3);

    double *user_vector = new double[col];
    user_vector[0] = -1;

    for (size_t j = 1; j < col; j++) {
      user_vector[j] = rating_matrix[user_id][j];
    }

    cossim_vector = cossim(user_vector, rating_matrix, col, row);
    double max_sim = -2;
    int max_index = -1;

    for (size_t i = 1; i < row; i++) {
      val = cossim_vector[i];

      if (i != user_id && val > max_sim && rating_matrix[i][movie_id] != -1) {
        max_sim = val;
        max_index = i;
      }
    }

    rec_rating_vector[k] = rating_matrix[max_index][movie_id];
    output_file << id << "," << rec_rating_vector[k] << std::endl;
    k++;
  }
}

/**
 * @brief The main function that orchestrates the execution of the program.
 */
 
int main() {
  std::string train_filename = "train.csv";
  std::string test_filename = "test.csv";
  size_t max_user_id, max_movie_id;

  std::tie(max_user_id, max_movie_id) = getMatrixDimensions(train_filename);
  cout << "Total number of users is: " << max_user_id << "\n";
  cout << "Total number of movies is: " << max_movie_id << "\n";

  double **rating_matrix = new double *[max_user_id];
  for (size_t i = 0; i < max_user_id; i++) {
    rating_matrix[i] = new double[max_movie_id];
  }

  fillMatrix(train_filename, rating_matrix, max_user_id, max_movie_id);
  print_top_movie(rating_matrix, max_user_id, max_movie_id);
  cout << "\n\n";
  print_top_user(rating_matrix, max_user_id, max_movie_id);
  movie_recom(test_filename, rating_matrix, max_user_id, max_movie_id);
}
