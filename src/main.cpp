#include "functions.cpp"

using namespace std;

tuple<int, int, int, vector<vector<string>>, int, unordered_map<string, int>> readData() {
    int buffer_size;
    int mRows;
    int nCols;
    vector<vector<string>> matrix_sequence;
    int nSequences;
    unordered_map<string, int> sequences_rewards;
    int mode;

    cout << "WELCOME CHOOMS." << endl;
    cout << "Wake up Samurai! New Tucil just dropped." << endl;

    cout << "Mode of operation: " << endl;
    cout << "1. File Mode" << endl;
    cout << "2. Manual Mode" << endl;
    cout << "Please input the mode: ";
    cin >> mode;

    while (mode != 1 && mode != 2) {
        cout << "Invalid mode. Please input 1 or 2." << endl;
        cin >> mode;
    }

    if (mode == 1) {
        string filename;

        cout << "Please input the file name: ";
        cin >> filename;
        ifstream inputFile("../test/"+filename+".txt");

        if (!inputFile.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            exit(EXIT_FAILURE);
        }

        inputFile >> buffer_size;
        inputFile >> mRows;
        inputFile >> nCols;
        matrix_sequence.resize(mRows, vector<string>(nCols));
        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < nCols; j++) {
                inputFile >> matrix_sequence[i][j];
            }
        }
        inputFile >> nSequences;
        for (int i = 0; i < nSequences; i++) {
            inputFile.ignore();
            string Sequence;
            int reward;
            getline(inputFile, Sequence);
            Sequence.erase(remove_if(Sequence.begin(), Sequence.end(), ::isspace), Sequence.end());
            inputFile >> reward;
            sequences_rewards[Sequence] = reward;
        }

        inputFile.close();
    } else {
        vector<string> tokens;
        int nTokens;
        int maxLength;
        while (!(std::cout << "Please input the number of unique tokens: " && std::cin >> nTokens)) {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Invalid input. Please input a valid number.\n";
    }
        for (int i = 0; i < nTokens; i++) {
            string token;
            cout << "Please input token " << i+1 << ": ";
            cin >> token;
            while (token.size() != 2) {
                cout << "Invalid token. Please input a 2-character token: ";
                cin >> token;
            }
            tokens.push_back(token);
        }
        
        int* variables[] = {&buffer_size, &mRows, &nCols, &nSequences, &maxLength};
        const char* prompts[] = {"buffer size", "number of rows", "number of columns", "number of sequences", "sequences max length"};

        for (int i = 0; i < 5; ++i) {
            while (!(std::cout << "Please input the " << prompts[i] << ": " && std::cin >> *variables[i])) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please input a valid number.\n";
            }
        }

        matrix_sequence = randomMatrix(tokens);
        sequences_rewards = randomSequence(tokens, nSequences, maxLength);
    }

    cout << "Buffer Size: " << buffer_size << endl;
    cout << "Matrix Size: " << mRows << "x" << nCols << endl;
    cout << "Matrix: " << endl;
    for (const auto& i : matrix_sequence) {
        for (const auto& j : i) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Number of sequences: " << nSequences << endl;
    cout << "All sequences and rewards:" << endl;
    for (const auto& i : sequences_rewards) {
        cout << addSpace(i.first) << ": " << i.second << endl;
    }
    cout << endl;

    return make_tuple(buffer_size, mRows, nCols, matrix_sequence, nSequences, sequences_rewards);
}

int main() {

    int buffer_size, mRows, nCols, nSequences;
    vector<vector<string>> matrix_sequence;
    unordered_map<string, int> sequences_rewards;
    pair<int, int> start;
    bool isHorizontal;
    PossibleSequence MaxPossible;

    tie(buffer_size, mRows, nCols, matrix_sequence, nSequences, sequences_rewards) = readData();

    auto begin = chrono::high_resolution_clock::now();

    PossibleSequence result = GetHighestScore(start, isHorizontal, matrix_sequence, buffer_size, MaxPossible, sequences_rewards);

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);

    cout << "Max Point: " << result.Point << endl;
    cout << "Pattern: " << addSpace(result.Pattern) << endl;
    for (const auto& point : result.steps) {
        cout << "<" << point.first+1 << "," << point.second+1 << "> " << endl;
    }
    cout << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;

    cout << endl;

    cout << "Do you want to save the result to a file? (y/n): ";
    char choice;
    cin >> choice;
    while (choice != 'y' && choice != 'n') {
        cout << "Invalid input. Please input y or n: ";
        cin >> choice;
    }

    if (choice == 'y') {
        cout << "Please input the filename: ";
        string filename;
        cin >> filename;
        ofstream outputFile("../test/"+ filename +".txt");
        if (!outputFile.is_open()) {
            cerr << "Error: Unable to open file " << "test/"+filename+".txt" << endl;
            exit(EXIT_FAILURE);
        }
        outputFile << "Max Point: " << result.Point << endl;
        outputFile << "Pattern: " << addSpace(result.Pattern) << endl;
        for (const auto& point : result.steps) {
            outputFile << "<" << point.first+1 << "," << point.second+1 << "> " << endl;
        }
        outputFile << endl;
        outputFile << "Execution time: " << duration.count() << " ms" << endl;
        outputFile.close();
        cout << "Result has been saved to " << "test/"+filename+".txt" << endl;
    } else {
        cout << "Result has not been saved." << endl;
    }

    return 0;
}
