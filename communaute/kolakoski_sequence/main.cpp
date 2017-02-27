#include <iostream>

#define NBR_TESTS 4

#ifdef _CLION_
#include <fstream>
#endif

using namespace std;

int main(int argc, char **argv) {
#ifdef _CLION_
    bool testsOk = true;
    for(int test = 1 ; test <= NBR_TESTS ; test++) {
        string path(argv[0]);
        ifstream TestFile(path.substr(0, path.find_last_of("/")) + "/Test" + to_string(test) + ".txt", ios::in);
        if (!TestFile) return 1;
        cin.rdbuf(TestFile.rdbuf());
#endif

        int N;
        cin >> N;
        cin.ignore();
        int A, B;
        cin >> A >> B;
        cin.ignore();

        string answer;
        for(int i = 0 ; i < A ; i++)
            answer += to_string(A);

        bool premierCaractere = false;
        int position = 1;

        while (answer.size() < N) {
            for (int i = 0; i < (answer.size() > position ? answer[position] - '0' : B); i++)
                answer += (premierCaractere ? to_string(A) : to_string(B));
            premierCaractere = !premierCaractere;
            position++;
        }

        answer = answer.substr(0, N);
        cout << answer << endl;

#ifdef _CLION_
        string goodAnswer;
        getline(cin, goodAnswer);
        if (goodAnswer != answer) {
            testsOk = false;
            cerr << "\n\n Trouvé : " << answer << endl;
            cerr << "Attendu : " << goodAnswer << endl;
        }
    }

    if(testsOk)
        cerr << "\n\nTous les tests sont bons =)" << endl;
    else
        cerr << "\n\nWARNING : au moins un test n'est pas bon =(" << endl;
#endif
}