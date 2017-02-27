#include <iostream>
#include <vector>
#include <algorithm>

/**
 * convertir une chaîne de caractères en blocs de 0
 */

using namespace std;

string printBinary(char c) {
    string result;
    for (int i = 6 ; i >= 0 ; --i) {
        result += ((c & (1 << i))? '1' : '0');
    }
    return result;
}

int main()
{
    /*
     * Get the message
     */
    string MESSAGE;
    getline(cin, MESSAGE);
    cerr << "Message to encode : " << MESSAGE << endl;
    
    /*
     * Transform into binary message
     */
    string binary;
    for(int i = 0 ; i < MESSAGE.length() ; i++) {
       binary += printBinary(MESSAGE[i]);
    }

    /*
     * Chuck Norris encoding
     */
    // pair<bool, int> = pair<0 or 1 ; nbr of consecutives binaries>
    vector<pair<bool, int>> infos;
    
    int nbr;
    for(int i = 0 ; i < binary.length() ; i += nbr) {
        bool bit = binary[i] - '0';
        nbr = 1;
        for(int j = i + 1 ; j < binary.length() ; j++) {
            if(bit == binary[j] - '0')
                nbr++;
            else
                break;
        }
        infos.push_back(make_pair(bit, nbr));
    }
    
    /*
     * Send blocks messages
     */
    string answer;
    for(auto test : infos) {
        if(test.first == 0)
            answer += "00";
        else
            answer += "0";
        answer += ' ';
        
        for(int i = 0 ; i < test.second ; i++) {
            answer += '0';
        }
        answer += ' ';
    }
    answer.pop_back(); // space at the end
    cout << answer << endl;
}
