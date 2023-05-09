//
// main.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "homophonic_cipher_matrix.h"
#include "utils.h"

int main(int argc, char *argv[]) {

    int n_random = 10;

    // check if the command-line argument for n_random is provided
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--n_random" && i + 1 < argc) {
            n_random = std::stoi(argv[i+1]);
        }
    }

    // print the value of n_random
    std::cout << "n_random = " << n_random << std::endl;

    text_matrix e_matrix("texts/phonemes_bi_grams.csv","texts/phonemes_counts.txt",39);
    std::cout<<"e_matrix created\n";

    homophonic_cipher_matrix d_matrix(e_matrix,"texts/code_bi_grams.csv","texts/code_counts.txt","texts/super_count.txt",101,n_random); //texts/freq_count.txt
    d_matrix.solve_cipher();
    printf("\n");

    return 0;
}
