#include <iostream>
#include <vector>

using namespace std;

enum Parenthesage {
    CROCHET,
    ACCOLADE,
    PARENTHESE
};

int main() {
    string str;
    cin >> str; cin.ignore();
    bool result = true;
    vector<Parenthesage> expression;

    for(int i = 0 ; i < str.size(); i++) {
        char c = str[i];

        if(c == '(')expression.push_back(PARENTHESE);
        else if(c == '[')expression.push_back(CROCHET);
        else if(c == '{')expression.push_back(ACCOLADE);

        else if(c == ')') {
            if(!expression.empty() && expression.back() == PARENTHESE)
                expression.pop_back();
            else {
                result = false;
                break;
            }
        }else if(c == ']') {
            if(!expression.empty() && expression.back() == CROCHET)
                expression.pop_back();
            else {
                result = false;
                break;
            }
        }else if(c == '}') {
            if(!expression.empty() && expression.back() == ACCOLADE)
                expression.pop_back();
            else {
                result = false;
                break;
            }
        }
    }
    if(result && !expression.empty())
        result = false;

    cout << (result?"true":"false") << endl;
}