#ifndef ATTACK_HOMOPHONIC_CIPHER_MATRIX_H__
#define ATTACK_HOMOPHONIC_CIPHER_MATRIX_H__

#include "text_matrix.h"
class homophonic_cipher_matrix {
    int code_count;
    int **matrix;
    int *putative_key;
    text_matrix &e_matrix;
    int* code_freqs;
    int* freq_distribution;
    int n_random;
    int** superv_mapping;
    int superv_tot;
    
    
 public:
    homophonic_cipher_matrix(text_matrix &text_matrix,std::string file_bigram,std::string file_freqs,std::string freq_count_file,int count,int n_random_,std::string superv_file);
    ~homophonic_cipher_matrix();
    void get_random_key();
    void modify_putative_key(int** matrix_d, int temp_putative_key[], int swap_i, int swap_j);
    void apply_putative_key(int** matrix_d, int *curr_putative_key);
    int inner_hill_climb(int *curr_putative_key);
    int random_initial_key();
    void solve_cipher();
};

#endif  // ATTACK_HOMOPHONIC_CIPHER_MATRIX_H__
