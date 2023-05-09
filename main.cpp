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
#include <sstream>

int main(int argc, char *argv[])
{

    int n_random = 10;
    int n_code = 100;
    // check if the command-line argument for n_random is provided
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--n_random" && i + 1 < argc)
        {
            n_random = std::stoi(argv[i + 1]);
        }
        if (std::string(argv[i]) == "--n_code" && i + 1 < argc)
        {
            n_code = std::stoi(argv[i + 1]);
        }
    }

    std::cout << "n_random = " << n_random << std::endl;
    std::cout << "n_code = " << n_code << std::endl;

    std::string phonemes_bi_gram_file = "texts/phonemes_bi_grams.csv";
    std::string phonemes_freqs_file = "texts/phonemes_counts.txt";;
    int n_phonemes = 39;

    std::stringstream s1;
    s1 << "texts/code" << n_code << "_bi_grams.csv";
    std::string code_bi_gram_file = s1.str();
    std::stringstream s2;
    s2 << "texts/code" << n_code << "_counts.txt";
    std::string code_freqs_file = s2.str();

    std::stringstream s3;
    s3 << "texts/freq_count_" << n_code << ".txt";
    std::string phonemes_code_counts = s3.str();

    text_matrix e_matrix(phonemes_bi_gram_file, phonemes_freqs_file, n_phonemes);
    homophonic_cipher_matrix d_matrix(e_matrix, code_bi_gram_file, code_freqs_file, phonemes_code_counts, n_code + 1, n_random); // texts/super_count.txt
    d_matrix.solve_cipher();
    printf("END\n");

    return 0;
}
