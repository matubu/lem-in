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
    vector<string> cur;
    string line;
    int n, idx;

    // parse number of ant
    cin >> n;
    cin.ignore();

    // Parse anthills
    while (1) {
        getline(cin, line);
        if (line[0] == '#')
            continue;
        cur = split(line);
        if (start == "") 
            start = cur[0];
        end = cur[0];
        dbg(cur);
        if (cur.size() != 3)
            break;
        graph[cur[0]] = anthill({stoi(cur[1]), stoi(cur[2])});
    }

    // parse connection
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

    paths = vector<vector<string>>(n, vector<string>(1, start));
    
    int cnt = 0;
    while (cnt != n) {
        getline(cin, line);
        cur = split(line);
        int mx = 0;
        for (int i = 0; i < cur.size(); i++) {
            if (cur[i][0] != 'L' || (idx = cur[i].find('-')) == cur[i].npos) {
                die("STOP PUTTING INVALID LINES IN THE INPUT PLS U ARE CRINGE\n");
            }

            int ant = stoi(cur[i].substr(1, idx));
            string hill = cur[i].substr(idx + 1);

            cnt += (hill == end);
            paths[ant - 1].push_back(hill);
            mx = paths[ant - 1].size();
        }

        // Fill in the rest of the paths
        for (int i = 0; i < n; i++)
            if (paths[i].size() < mx)
                paths[i].push_back(paths[i].back());
        cnt++;
    }
}