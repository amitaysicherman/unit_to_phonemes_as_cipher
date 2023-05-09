//
// text_matrix.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#include "text_matrix.h"

#include <math.h>

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "utils.h"

text_matrix::text_matrix(std::string file_bigram,std::string file_freqs,int N) {
    n_count=N;
    matrix = read_2d(file_bigram, N);
    letter_frequency = read_1d(file_freqs, N);

}

text_matrix::text_matrix() {
    text_matrix("texts/phonemes_bi_grams.csv","texts/phonemes_counts.txt",39);
}

int text_matrix::compute_score(int** matrix_d) const {
    int score = 0;
    int squared_score = 0;
    for (int i = 0; i < n_count; i++) {
        for (int j = 0; j < n_count; j++) {
            if (matrix[i][j] > matrix_d[i][j]) {
                squared_score = matrix[i][j] - matrix_d[i][j];
            } else {
                squared_score = matrix_d[i][j] - matrix[i][j];
            }
            score = score + squared_score;
        }
    }
    return score;
}

int text_matrix::compute_score_ij(int** matrix_ref,int i , int j) const{
    int score=0;
    for (int k = 0; k < n_count; k++) {
        score += std::abs(matrix[i][k]-matrix_ref[i][k]); 
        score += std::abs(matrix[j][k]-matrix_ref[j][k]) ;
        score += std::abs(matrix[k][i] - matrix_ref[k][i]);
        score += std::abs(matrix[k][j] -matrix_ref[k][j]);    
    }
    score-=std::abs(matrix[i][j]-matrix_ref[i][j]);
    score-=std::abs(matrix[j][i]-matrix_ref[j][i]);
    score-=std::abs(matrix[i][i]-matrix_ref[i][i]);
    score-=std::abs(matrix[j][j]-matrix_ref[j][j]);
    return score;
}


int text_matrix::compute_score_delta(int** matrix_1,int** matrix_2,int i , int j) const{
    int score_1=compute_score_ij(matrix_1,i,j);
    int score_2=compute_score_ij(matrix_2,i,j);
    return score_1-score_2;
}

