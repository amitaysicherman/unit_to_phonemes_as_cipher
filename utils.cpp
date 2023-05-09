//
// utils.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>


int** read_2d(std::string filename, int N) {
    std::ifstream file(filename);
    int** bi_grams = new int*[N];
    for (int i = 0; i < N; i++) {
        bi_grams[i] = new int[N];
    }
    int row = 0;
    std::string line;
    while (std::getline(file, line) && row < N) {
        int col = 0;
        size_t pos = 0;
        std::string token;
        while ((pos = line.find(',')) != std::string::npos && col < N) {
            token = line.substr(0, pos);
            bi_grams[row][col] = std::stoi(token);
            line.erase(0, pos + 1);
            col++;
        }
        if (col < N) {
            bi_grams[row][col] = std::stoi(line);
        }
        row++;
    }
    // print_2d(bi_grams, N);
    return bi_grams;
}

void print_2d(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        std::cout << "\n";
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
        }
    }
    std::cout << "\n";
}


int* read_1d(std::string filename, int N) {
    std::ifstream file(filename);
    int* freqs = new int[N];
    std::string line;
    getline(file, line);
    int col = 0;
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(',')) != std::string::npos && col < N) {
        token = line.substr(0, pos);
        freqs[col] = std::stoi(token);
        line.erase(0, pos + 1);
        col++;
    }
    if (col < N) {
        freqs[col] = std::stoi(line);
    }
    // print_1d(freqs, N);
    return freqs;
}

void print_1d(int* matrix, int N) {
    for (int i = 0; i < N; i++) {
        std::cout << "\n";
        std::cout << matrix[i] << " ";
    }
    std::cout << "\n";
}


void save_results(int score,int* arr,int n){
    
    std::stringstream ss;
    ss << "scores/" <<n<<"_"<<score << ".txt";
    std::string file_name = ss.str();
    std::ofstream output(file_name);
    // write the contents of the array to the file, separated by commas
    for (int i = 0; i < n; i++) {
        output << arr[i];
        if (i < n - 1) {
            output << ",";
        }
    }
    output<<std::endl;

    // close the file
    output.close();
}