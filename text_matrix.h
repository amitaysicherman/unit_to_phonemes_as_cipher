//
// text_matrix.h
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#ifndef ATTACK_TEXT_MATRIX_H__
#define ATTACK_TEXT_MATRIX_H__

#include "utils.h"

class text_matrix {
    int** matrix;
    int text_len;

 public:
    explicit text_matrix(std::string file_bigram,std::string file_freqs,int N);
    explicit text_matrix();
    int compute_score(int** matrix_d) const;
    int compute_score_ij(int** matrix,int i , int j) const;
    int compute_score_delta(int** matrix_1,int** matrix_2,int i , int j) const;

    int n_count;
    int* letter_frequency;


};

#endif  // ATTACK_TEXT_MATRIX_H__
