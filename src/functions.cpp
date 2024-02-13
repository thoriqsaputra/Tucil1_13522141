#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

struct PossibleSequence {
    int Point;
    vector<pair<int,int>> steps;
    string Pattern;
};

vector<vector<pair<int, int>>> findAllPossiblePaths(const vector<vector<string>>& matrix, int bufferSize, pair<int, int> start, bool isHorizontal) {
    int x = start.first;
    int y = start.second;

    vector<vector<pair<int, int>>> paths;
    if (bufferSize == 1) {
        if (isHorizontal) {
            for (int new_y = 0; new_y < matrix[0].size(); ++new_y) {
                if (new_y != y) {
                    paths.push_back({{x, new_y}});
                }
            }
        } else {
            for (int new_x = 0; new_x < matrix.size(); ++new_x) {
                if (new_x != x) {
                    paths.push_back({{new_x, y}});
                }
            }
        }
    } else {
        if (isHorizontal) {
            for (int new_y = 0; new_y < matrix[0].size(); ++new_y) {
                if (new_y != y) {
                    auto smallerPaths = findAllPossiblePaths(matrix, bufferSize - 1, {x, new_y}, false);
                    for (const auto& path : smallerPaths) {
                        vector<pair<int, int>> newPath = {{x, new_y}};
                        newPath.insert(newPath.end(), path.begin(), path.end());
                        paths.push_back(newPath);
                    }
                }
            }
        } else {
            for (int new_x = 0; new_x < matrix.size(); ++new_x) {
                if (new_x != x) {
                    auto smallerPaths = findAllPossiblePaths(matrix, bufferSize - 1, {new_x, y}, true);
                    for (const auto& path : smallerPaths) {
                        vector<pair<int, int>> newPath = {{new_x, y}};
                        newPath.insert(newPath.end(), path.begin(), path.end());
                        paths.push_back(newPath);
                    }
                }
            }
        }
    }
    return paths;
}

string addSpace(const string& input) {
    string spacedString = "";
    for (size_t i = 0; i < input.length(); i += 2) {
        spacedString += input.substr(i, 2) + " ";
    }
    if (!spacedString.empty()) {
        spacedString.pop_back();
    }
    return spacedString;
}

const PossibleSequence &GetHighestScore(std::pair<int, int> &start, bool &isHorizontal, std::vector<std::vector<std::__cxx11::string>> &matrix_sequence, int buffer_size, PossibleSequence &MaxPossible, std::unordered_map<std::__cxx11::string, int> &sequences_rewards)
{
    start = {0, -1};
    isHorizontal = true;
    auto paths = findAllPossiblePaths(matrix_sequence, buffer_size, start, isHorizontal);

    MaxPossible.Point = 0;
    MaxPossible.Pattern = "";
    for (const auto &path : paths)
    {
        string sequence = "";
        int poin = 0;
        for (const auto &point : path)
        {
            sequence += matrix_sequence[point.first][point.second];
        }
        for (const auto &i : sequences_rewards)
        {
            size_t found = sequence.find(i.first);
            while (found != string::npos)
            {
                poin += i.second;
                found = sequence.find(i.first, found + 1);
            }
        }
        if (poin > MaxPossible.Point)
        {
            MaxPossible.Point = poin;
            MaxPossible.steps = path;
            MaxPossible.Pattern = sequence;
        }
    }

    return MaxPossible;
}

unordered_map<string, int> randomSequence(const vector<string>& tokens, int numSequences, int maxLength) {
    unordered_map<string, int> sequences_rewards;

    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < numSequences; ++i) {
        int sequenceLength = uniform_int_distribution<>(2, maxLength)(g);
        int randomNumber = uniform_int_distribution<>(1, 20)(g) * 5; 
        string sequence;

        for (int j = 0; j < sequenceLength; ++j) {
            int index = uniform_int_distribution<>(0, tokens.size() - 1)(g);
            sequence += tokens[index];
        }

        sequences_rewards[sequence] = randomNumber;
    }

    return sequences_rewards;
}

vector<vector<string>> randomMatrix(const vector<string>& tokens) {
    random_device rd;
    mt19937 gen(rd());

    vector<string> shuffledTokens = tokens;
    shuffle(shuffledTokens.begin(), shuffledTokens.end(), gen);

    int numRows = shuffledTokens.size();
    vector<vector<string>> matrix(numRows, vector<string>(numRows));

    int tokenIndex = 0;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numRows; j++) {
            matrix[i][j] = shuffledTokens[tokenIndex];
            tokenIndex = (tokenIndex + 1) % shuffledTokens.size();
        }
    }
    
    shuffle(matrix.begin(), matrix.end(), gen);
    for (int i = 0; i < numRows; i++) {
        shuffle(matrix[i].begin(), matrix[i].end(), gen);
    }

    return matrix;
}

// int main() {
    
//     string filename;
//     cout << "Enter the filename: ";
//     cin >> filename;

//     auto start = chrono::high_resolution_clock::now();
//     PossibleSequence result = processFile(filename);

//     auto end = chrono::high_resolution_clock::now();

//     auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

//     cout << "Max Point: " << result.Point << endl;
//     cout << "Pattern: " << addSpace(result.Pattern) << endl;
//     for (const auto& point : result.steps) {
//         cout << "<" << point.first+1 << "," << point.second+1 << "> " << endl;
//     }
//     cout << endl;
//     cout << "Execution time: " << duration.count() << " ms" << endl;
//     return 0;
// }
