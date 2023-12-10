#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Global variables
int N;
vector<vector<int>> H, V, D, H_div, V_div, group_list, group_edge, dist;
vector<int> group_root;

// Function to perform rectangle division
void rectangleDivision() {
    H_div.resize(N - 1, vector<int>(N, 0));
    V_div.resize(N, vector<int>(N - 1, 0));

    // Horizontal division
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N; ++j) {
            if (H_div[i][j] == 1 || H[i][j] == 0) {
                continue;
            }

            H_div[i][j] = 1;
            int jj = j - 1;

            while (jj >= 0) {
                if (V[i][jj] == 1 && V[i + 1][jj] == 1) {
                    break;
                }

                H_div[i][jj] = 1;
                --jj;
            }

            jj = j + 1;
            while (jj < N) {
                if (V[i][jj - 1] == 1 && V[i + 1][jj - 1] == 1) {
                    break;
                }

                H_div[i][jj] = 1;
                ++jj;
            }
        }
    }

    // Vertical division
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            if (V_div[i][j] == 1 || V[i][j] == 0) {
                continue;
            }

            V_div[i][j] = 1;
            int ii = i - 1;

            while (ii >= 0) {
                if (H[ii][j] == 1 || H[ii][j + 1] == 1) {
                    break;
                }

                V_div[ii][j] = 1;
                --ii;
            }

            ii = i + 1;
            while (ii < N) {
                if (H[ii - 1][j] == 1 || H[ii - 1][j + 1] == 1) {
                    break;
                }

                V_div[ii][j] = 1;
                ++ii;
            }
        }
    }
}

// Function to perform grouping using BFS
void groupBFS() {
    group_list = vector<vector<int>>(N, vector<int>(N, -1));
    group_edge.clear();
    int group_num = 0;
    vector<vector<int>> dij = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (group_list[i][j] != -1) {
                continue;
            }

            group_edge.push_back({i, j, i, j});
            group_list[i][j] = group_num;
            queue<pair<int, int>> que;
            que.push({i, j});

            while (!que.empty()) {
                pair<int, int> cur = que.front();
                que.pop();
                int x = cur.first;
                int y = cur.second;

                group_edge.back()[0] = min(group_edge.back()[0], x);
                group_edge.back()[1] = min(group_edge.back()[1], y);
                group_edge.back()[2] = max(group_edge.back()[2], x);
                group_edge.back()[3] = max(group_edge.back()[3], y);

                for (int k = 0; k < 4; ++k) {
                    int nx = x + dij[k][0];
                    int ny = y + dij[k][1];

                    if (nx >= 0 && ny >= 0 && nx < N && ny < N && group_list[nx][ny] == -1) {
                        if ((dij[k][0] == 0 && V_div[x][min(y, ny)] == 0) || (dij[k][1] == 0 && H_div[min(x, nx)][y] == 0)) {
                            group_list[nx][ny] = group_num;
                            que.push({nx, ny});
                        }
                    }
                }
            }
            group_num += 1;
        }
    }
}
bool isElementInVector(const vector<int>& vec, int element) {
    for (const auto& el : vec) {
        if (el == element) {
            return true;
        }
    }
    return false;
}

// Function to perform DFS to determine group roots
void groupRootDFS(int i, int j) {
    dist[i][j] = 1;
    vector<vector<int>> dij = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};

    for (int d = 0; d < 4; ++d) {
        int ni = i + dij[d][0];
        int nj = j + dij[d][1];

        if (ni >= 0 && ni < N && nj >= 0 && nj < N && dist[ni][nj] == 0) {
            if (!isElementInVector(group_root, group_list[ni][nj])) {
                group_root.push_back(group_list[ni][nj]);
            }
            groupRootDFS(ni, nj);
        }
    }
}

int main() {
    // Read input
    cin >> N;

    H = vector<vector<int>>(N - 1, vector<int>(N));
    V = vector<vector<int>>(N, vector<int>(N - 1));
    D = vector<vector<int>>(N, vector<int>(N));
    string s;
    for (int i = 0; i < N - 1; ++i) {
        cin >> s;
        for (int j = 0; j < N; ++j) {
            H[i][j] = (int)(s[j] - '0');
        }
    }

    for (int i = 0; i < N; ++i) {
        cin >> s;
        for (int j = 0; j < N - 1; ++j) {
            V[i][j] = (int)(s[j] - '0');
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> D[i][j];
        }
    }

    // Perform rectangle division
    rectangleDivision();
    // Perform grouping
    groupBFS();

    // Initial solution
    group_root = {0};
    dist = vector<vector<int>>(N, vector<int>(N, 0));

    // DFS to determine group roots
    groupRootDFS(0, 0);
    for(int i=0; i<group_root.size(); i++){
        cout << group_root[i] << " ";
    }
    cout << endl;
    for(int i=0; i<group_edge.size(); i++){
        for(int j=0; j<group_list.size(); j++){
            for(int k=0; k<group_list[j].size(); k++){
                cout << group_list[j][k] << " ";
            }
            cout << endl;
        }
    }
    return 0;
}
