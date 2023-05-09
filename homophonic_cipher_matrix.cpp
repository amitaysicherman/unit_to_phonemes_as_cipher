//
// homophonic_cipher_matrix.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#include "homophonic_cipher_matrix.h"

#include <cstdlib>
#include <iostream>
#include <random>
#include <cstring>
#include "utils.h"
#include <algorithm>

homophonic_cipher_matrix::homophonic_cipher_matrix(text_matrix &text_matrix, std::string file_bigram, std::string file_freqs, std::string freq_count_file, int count, int n_random_,std::string superv_file_) : e_matrix(text_matrix)
{
    e_matrix = text_matrix;
    code_count = count;
    matrix = read_2d(file_bigram, count);
    // letter_frequency = read_freqs(file_freqs, count);
    code_freqs = read_1d(file_freqs, count);
    std::cout << "letter_count : " << code_count << "\n";
    putative_key = new int[code_count];
    if (putative_key == NULL)
    {
        std::cout << "new failed for putative_key in homophonic_cipher_matrix";
        return;
    }
    int i;
    for (i = 0; i < code_count; i++)
    {
        putative_key[i] = -1;
    }
    freq_distribution = read_1d(freq_count_file, e_matrix.n_count);
    n_random = n_random_;
    superv_mapping = read_2d(superv_file_, e_matrix.n_count,code_count);
    superv_tot=0;
    for (i=0; i<e_matrix.n_count;i++){
        for (int j=0;j<code_count;j++){
            superv_tot+=superv_mapping[i][j];
        }
    }
    std::cout<<superv_tot<<std::endl;
}

homophonic_cipher_matrix::~homophonic_cipher_matrix()
{
    delete[] putative_key;

    for (int i = 0; i < code_count; i++)
    {
        delete[] matrix[i];
    }

    delete[] matrix;
}

void homophonic_cipher_matrix::solve_cipher()
{
    int score;
    int i;
    int score_least;
    int *curr_best_key = NULL;

    curr_best_key = new int[code_count];

    if (curr_best_key == NULL)
    {
        std::cout << "new failed for curr_best_key in homophonic_cipher_matrix";
        return;
    }

    std::cout << "Initial frequency distribution: ";
    for (i = 0; i < e_matrix.n_count; i++)
    {
        std::cout << freq_distribution[i] << " ";
    }
    std::cout << "\n";

    score_least = 1000000;

    int a, b, j, k;
    int score_flag = 0;
    int less_score_flag;
    for (b = 1; b < e_matrix.n_count; b++)
    {
        for (a = 0; (a + b) < e_matrix.n_count; a++)
        {
            less_score_flag = 0;
            score_flag = 0;
            i = a;
            j = a + b;

            // Try i-- and j++
            if (freq_distribution[i] >= 0)
            {
                freq_distribution[i]--;
                freq_distribution[j]++;

                score = random_initial_key();

                std::cout << "Current outer hill climb score: " << score;

                if (score < score_least)
                {
                    std::cout << " (OUTER new best)";

                    score_least = score;

                    for (k = 0; k < code_count; k++)
                    {
                        curr_best_key[k] = putative_key[k];
                    }

                    less_score_flag = 1;
                    score_flag = 1;
                    save_results(score_least, curr_best_key, code_count);
                }
                else
                {
                    for (k = 0; k < code_count; k++)
                    {
                        putative_key[k] = curr_best_key[k];
                    }
                    freq_distribution[i]++;
                    freq_distribution[j]--;
                }

                std::cout << "\n";
            }

            if ((score_flag == 0) && (freq_distribution[j] >= 0))
            {
                // Try i++ and j--
                freq_distribution[i]++;
                freq_distribution[j]--;

                score = random_initial_key();

                std::cout << "Current outer hill climb score: " << score;

                if (score < score_least)
                {
                    std::cout << " (OUTER new best)";

                    score_least = score;

                    for (k = 0; k < code_count; k++)
                    {
                        curr_best_key[k] = putative_key[k];
                    }

                    save_results(score_least, curr_best_key, code_count);
                }
                else
                {
                    for (k = 0; k < code_count; k++)
                    {
                        putative_key[k] = curr_best_key[k];
                    }
                    freq_distribution[i]--;
                    freq_distribution[j]++;
                }

                std::cout << "\n";
            }
            if (less_score_flag == 1)
            {
                a = 0;
                b = 1;
            }
        }
    }

    std::cout << "Finished outer hill climb, results:\n";

    std::cout << "Final score: " << score_least << "\n";

    std::cout << "Final frequency distribution:";
    for (i = 0; i < e_matrix.n_count; i++)
    {
        std::cout << " " << freq_distribution[i];
    }

    std::cout << "\nFinal key:\n     Cipher symbol: ";

    std::cout << "\n  Plaintext letter: ";
    for (i = 0; i < code_count; i++)
    {
        std::cout << putative_key[i];
    }
    std::cout << "\n";
    delete[] curr_best_key;
}

int homophonic_cipher_matrix::random_initial_key()
{
    int i;
    int j;
    int k;
    int score_least;
    int score;
    int *best_putative_key = NULL;

    std::cout << "Creating " << n_random << " random keys using frequency distribution: ";
    for (i = 0; i < e_matrix.n_count; i++)
    {
        std::cout << freq_distribution[i] << " ";
    }
    std::cout << "\n";

    get_random_key();

    score_least = inner_hill_climb(putative_key);
    
    std::cout << "Initial putative key score: " << score_least << "\n";

    best_putative_key = new int[code_count];

    if (best_putative_key == NULL)
    {
        std::cout << "new failed for best_putative_key in homophonic_cipher_matrix";
        return 100000;
    }
    for (k = 0; k < code_count; k++)
    {
        best_putative_key[k] = putative_key[k];
    }

    for (j = 0; j < n_random; j++)
    {
        get_random_key();

        score = inner_hill_climb(putative_key);

        std::cout << "Current inner hill climb score: " << score;

        if (score < score_least)
        {
            std::cout << " (RANDOM IMPROVED)";

            // copy best_putative_key to putative_key
            for (i = 0; i < code_count; i++)
            {
                best_putative_key[i] = putative_key[i];
            }
            score_least = score;
        }

        std::cout << "\n";
    }

    std::cout << "Best score from inner hill climb: " << score_least << "\n";
    for (i = 0; i < code_count; i++)
    {
        putative_key[i] = best_putative_key[i];
    }
    delete[] best_putative_key;

    return score_least;
}

void homophonic_cipher_matrix::get_random_key()
{

    int *mapping = new int[code_count];
    std::fill_n(mapping, code_count, -1);
    for (int i = 0; i < e_matrix.n_count; i++)
    {
        int t = e_matrix.letter_frequency[i];
        int c = freq_distribution[i];
        for (int j = 0; j < c; j++)
        {
            int opt_index[code_count];
            int options[code_count];
            int num_options = 0;
            for (int k = 0; k < code_count; k++)
            {
                if (mapping[k] == -1)
                {
                    opt_index[num_options] = k;
                    options[num_options] = code_freqs[k];
                    num_options++;
                }
            }
            double w[num_options];
            std::transform(options, options + num_options, w, [t](int o)
                           { return std::abs(o - t); });
            int m = *std::max_element(w, w + num_options);
            std::transform(w, w + num_options, w, [m](int w_)
                           { return m - w_; });
            double tot = std::accumulate(w, w + num_options, 0.0);
            std::transform(w, w + num_options, w, [tot](double w_)
                           { return w_ / tot; });
            std::discrete_distribution<int> dist(w, w + num_options);
            std::mt19937 gen(std::random_device{}());
            int v = opt_index[dist(gen)];
            mapping[v] = i;
            t -= code_freqs[v];
        }
    }
    for (int i = 0; i < code_count; i++)
    {
        putative_key[i] = mapping[i];
    }
}

void homophonic_cipher_matrix::apply_putative_key(int **matrix_d, int *curr_putative_key)
{
    int i, j;
    for (i = 0; i < code_count; i++)
    {
        for (j = 0; j < code_count; j++)
        {
            matrix_d[curr_putative_key[i]][curr_putative_key[j]] += matrix[i][j];
        }
    }
}

void homophonic_cipher_matrix::modify_putative_key(int **matrix_d, int temp_putative_key[], int swap_i, int swap_j)
{
    // Swap the elements in temp_putative_key
    int temp = temp_putative_key[swap_i];
    temp_putative_key[swap_i] = temp_putative_key[swap_j];
    temp_putative_key[swap_j] = temp;

    int key_i = temp_putative_key[swap_i];
    int key_j = temp_putative_key[swap_j];

    /* Reset the matrix_d rows and columns corresponding to temp_putative_key[i]
     * and temp_putative_key[j]
     */
    for (int k = 0; k < e_matrix.n_count; k++)
    {
        matrix_d[key_i][k] = 0;
        matrix_d[key_j][k] = 0;

        matrix_d[k][key_i] = 0;
        matrix_d[k][key_j] = 0;
    }

    /*
     */
    for (int i = 0; i < code_count; i++)
    {
        int temp_i = temp_putative_key[i];
        int should_calc = temp_i == key_i || temp_i == key_j;
        for (int j = 0; j < code_count; j++)
        {
            if (should_calc || temp_putative_key[j] == key_i || temp_putative_key[j] == key_j)
            {
                matrix_d[temp_i][temp_putative_key[j]] += matrix[i][j];
            }
        }
    }
}

int homophonic_cipher_matrix::inner_hill_climb(int *curr_putative_key)
{

    int i, j, score;
    int score_least = 0;
    int score_delta = 0;
    if (curr_putative_key == NULL)
    {
        std::cout << "curr_putative_key is NULL";
        return -1;
    }

    int **matrix_d = new int *[e_matrix.n_count];
    for (i = 0; i < e_matrix.n_count; i++)
    {
        matrix_d[i] = new int[e_matrix.n_count];
        for (j = 0; j < e_matrix.n_count; j++)
        {
            matrix_d[i][j] = 0;
        }
    }

    // Update the matrix_d with the putative key
    apply_putative_key(matrix_d, curr_putative_key);

    int **previous_matrix_d = new int *[e_matrix.n_count];
    for (i = 0; i < e_matrix.n_count; i++)
    {
        previous_matrix_d[i] = new int[e_matrix.n_count];
        for (j = 0; j < e_matrix.n_count; j++)
        {
            previous_matrix_d[i][j] = matrix_d[i][j];
        }
    }

    // Compute the score by comparing E-Matrix and D-Matrix
    score = e_matrix.compute_score(matrix_d);

    /* Have a temporary putative key which will be used for modification */
    int *temp_putative_key = new int[code_count];
    if (temp_putative_key == NULL)
    {
        std::cout << "new failed for temp_putative_key in homophonic_cipher_matrix";
        return -1;
    }

    for (i = 0; i < code_count; i++)
    {
        temp_putative_key[i] = curr_putative_key[i];
    }

    // save the least score till now into score_least
    score_least = score;

    // Modify the putative keys in nested for loops

    int a;
    int b;

    for (b = 1; b < code_count; b++)
    {
        for (a = 0; (a + b) < code_count; a++)
        {
            i = a;
            j = a + b;
            if (temp_putative_key[i] == temp_putative_key[j])
            {
                continue;
            }

            modify_putative_key(matrix_d, temp_putative_key, i, j);
            score_delta = e_matrix.compute_score_delta(matrix_d, previous_matrix_d, temp_putative_key[i], temp_putative_key[j]);

            if (score_delta < 0)
            {
                curr_putative_key[i] = temp_putative_key[i];
                curr_putative_key[j] = temp_putative_key[j];
                score_least = score_least + score_delta;

                for (int k = 0; k < e_matrix.n_count; k++)
                {
                    previous_matrix_d[k][temp_putative_key[i]] = matrix_d[k][temp_putative_key[i]];
                    previous_matrix_d[k][temp_putative_key[j]] = matrix_d[k][temp_putative_key[j]];
                    previous_matrix_d[temp_putative_key[i]][k] = matrix_d[temp_putative_key[i]][k];
                    previous_matrix_d[temp_putative_key[j]][k] = matrix_d[temp_putative_key[j]][k];
                }
                a = 0;
                b = 1;
                i = a;
                j = a + b;
            }
            else
            {
                int temp = temp_putative_key[j];
                temp_putative_key[j] = temp_putative_key[i];
                temp_putative_key[i] = temp;

                for (int k = 0; k < e_matrix.n_count; k++)
                {
                    matrix_d[k][temp_putative_key[i]] = previous_matrix_d[k][temp_putative_key[i]];
                    matrix_d[k][temp_putative_key[j]] = previous_matrix_d[k][temp_putative_key[j]];
                    matrix_d[temp_putative_key[i]][k] = previous_matrix_d[temp_putative_key[i]][k];
                    matrix_d[temp_putative_key[j]][k] = previous_matrix_d[temp_putative_key[j]][k];
                }
            }
        }
    }
    
    int superv_score=superv_err(superv_mapping,curr_putative_key,superv_tot,e_matrix.n_count,code_count);

    delete[] temp_putative_key; // delete temp putative_key
    for (int i = 0; i < e_matrix.n_count; i++)
    {
        delete[] previous_matrix_d[i];
    }
    delete[] previous_matrix_d;

    for (int i = 0; i < e_matrix.n_count; i++)
    {
        delete[] matrix_d[i];
    }
    delete[] matrix_d;
    return superv_score;
    
    // return score_least;
}
