#include "viz.hpp"

#define START 1
#define END 2

vector<string> split(string s) {
    vector<string> ret;

    string cur = "";
    int j;
    for (u_int32_t i = 0; i < s.size(); i += j) {
        j = 1;
        if (s[i] != ' ') {
            while (i+j < s.size() && s[i + j] != ' ')
                j++;
            ret.push_back(s.substr(i, j));
        }
    }

    return ret;
}

void parse(map<string, anthill> &graph, vector<vector<string>> &paths, string &start, string &end) {
    int n;
    cin >> n;
    cin.ignore();
    dbg(n);
    string line = "pouet";
    vector<string> cur;
    while (1) {
        getline(cin, line);
        if (line[0] == '#')
            continue;
        cur = split(line);
        dbg(cur);
        if (cur.size() != 3)
            break;
        graph[cur[0]] = anthill({stoi(cur[1]), stoi(cur[2])});
    }

    while (line != "") {
        int idx = line.find('-');
        if (idx == line.npos) {
            cerr << "error: invalid line" << endl;
            exit(1);
        }
        string u = line.substr(0, idx);
        string v = line.substr(idx + 1);
        graph[u].out.push_back(v);
        graph[v].out.push_back(u);
        getline(cin, line);
   }

    for (auto &it: graph) {
        cout << it.first << ": " << endl;
        for (auto &it2: it.second.out)
            cout << it2 << " ";
        cout << endl;
    }


}