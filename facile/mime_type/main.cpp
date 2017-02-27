#include <iostream>
#include <map>
#include <algorithm>

/**
 * associer des noms de fichiers à leur MIME type
 */

using namespace std;

string extensionName(string &name) {
    size_t pos = name.find_last_of(".");
    return pos == -1 ? "UNKNOWN" : name.substr(pos + 1);
}

string tester(string &name, map<string, string> &table) {
    map<string, string>::iterator it = table.find(extensionName(name));
    if(it == table.end())
        return "UNKNOWN";
    return it->second;
}

void lower(string & path) {
    for(int i = 0 ; i < path.length() ; i++) {
        path[i] = tolower(path[i]);
    }
}

int main()
{
    int N; // Number of elements which make up the association table.
    cin >> N; cin.ignore();
    int Q; // Number Q of file names to be analyzed.
    cin >> Q; cin.ignore();
    cerr << "Il y a " << Q << " éléments à tester\n" << endl;
    
    map<string, string> table;
    for (int i = 0; i < N; i++) {
        string EXT, MT; // file extension + MIME type
        cin >> EXT >> MT; cin.ignore();
        
        lower(EXT);
        
        table[EXT] = MT;
    }

    for (int i = 0; i < Q; i++) {
        string FNAME; // One file name per line.
        getline(cin, FNAME);

        lower(FNAME);
        cout << tester(FNAME, table) << endl;
    }
}
