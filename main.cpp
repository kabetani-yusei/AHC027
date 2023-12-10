#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
using namespace std;

// Global variables
int N;
vector<vector<vector<vector<vector<int>>>>> group_dist;
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

// 各グループ間の最短距離を求める関数
vector<vector<vector<int>>> calgroup_dist(const vector<int>& pre, const vector<int>& nex) {
    vector<vector<int>> sum(2, vector<int>(2, 2000));
    vector<int> lookList = {0, 1, 0, 3, 2, 1, 2, 3};
    vector<vector<int>> dij = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    vector<vector<int>> rootFirst, rootSecond;
    int rootFlag = 0;
    vector<vector<int>> set;

    for (int k = 0; k < 4; ++k) {
        int flag = 4;
        vector<vector<int>> dist(N, vector<int>(N, -1));
        int i1 = pre[lookList[k * 2]];
        int j1 = pre[lookList[k * 2 + 1]];
        
        if (find(set.begin(), set.end(), vector<int>{i1, j1}) != set.end())
            continue;

        set.push_back(vector<int>{i1, j1});
        queue<vector<int>> que;
        que.push(vector<int>{i1, j1});
        dist[i1][j1] = 0;

        while (!que.empty()) {
            vector<int> cur = que.front();
            que.pop();
            int x = cur[0];
            int y = cur[1];

            for (int m = 0; m < 4; ++m) {
                int moveX = x + dij[m][0];
                int moveY = y + dij[m][1];

                if (moveX >= 0 && moveY >= 0 && moveX < N && moveY < N && dist[moveX][moveY] == -1) {
                    if ((dij[m][0] == 0 && V[x][min(y, moveY)] == 0) || (dij[m][1] == 0 && H[min(x, moveX)][y] == 0)) {
                        dist[moveX][moveY] = dist[x][y] + 1;
                        que.push(vector<int>{moveX, moveY});
                    }
                }
            }
        }

        int kk = 0;
        int nowValue = dist[nex[lookList[kk]]][nex[lookList[kk + 1]]];
        for (int kkk = 1; kkk < 3; ++kkk) {
            if (nowValue > dist[nex[lookList[kkk * 2]]][nex[lookList[kkk * 2 + 1]]]) {
                nowValue = dist[nex[lookList[kkk * 2]]][nex[lookList[kkk * 2 + 1]]];
                kk = kkk;
            }
        }

        if (sum[0][0] > dist[nex[lookList[kk * 2]]][nex[lookList[kk * 2 + 1]]]) {
            sum[0][0] = dist[nex[lookList[kk * 2]]][nex[lookList[kk * 2 + 1]]];
            flag = kk;
            rootFlag = 0;
        } else if (sum[1][0] > dist[nex[lookList[kk * 2]]][nex[lookList[kk * 2 + 1]]]) {
            sum[1][0] = dist[nex[lookList[kk * 2]]][nex[lookList[kk * 2 + 1]]];
            flag = kk;
            rootFlag = 1;
        }

        if (flag == 4)
            continue;

        int iii = nex[lookList[flag * 2]];
        int jjj = nex[lookList[flag * 2 + 1]];
        int score = dist[iii][jjj];
        vector<vector<int>> root = {vector<int>{iii, jjj}};
        int x = iii;
        int y = jjj;

        while (score > 0) {
            for (int k = 0; k < 4; ++k) {
                int moveX = x + dij[k][0];
                int moveY = y + dij[k][1];

                if (moveX >= 0 && moveY >= 0 && moveX < N && moveY < N) {
                    if ((dij[k][0] == 0 && V[x][min(y, moveY)] == 0) || (dij[k][1] == 0 && H[min(x, moveX)][y] == 0)) {
                        if (dist[moveX][moveY] == score - 1) {
                            root.push_back(vector<int>{moveX, moveY});
                            x = moveX;
                            y = moveY;
                            --score;
                            break;
                        }
                    }
                }
            }
        }

        if (rootFlag == 0) {
            rootSecond = rootFirst;
            rootFirst = root;
            reverse(rootFirst.begin(), rootFirst.end());
        } else if (rootFlag == 1) {
            rootSecond = root;
            reverse(rootSecond.begin(), rootSecond.end());
        }
    }

    if (rootSecond.empty()) {
        rootSecond = rootFirst;
    }

    return {rootFirst, rootSecond};
}

int rootDistance(const vector<int>& root) {
    int nowDist = 0;
    int now, pre;
    vector<int> nowPoint = {0, 0};
    for (int i = 1; i < root.size(); ++i) {
        now = root[i];
        pre = root[i - 1];

        if (nowPoint == group_dist[pre][now][0][0]) {
            nowDist += group_dist[pre][now][1].size();
            nowPoint = group_dist[pre][now][1].back();
        } else {
            nowDist += group_dist[pre][now][0].size();
            nowPoint = group_dist[pre][now][0].back();
        }
    }

    now = root.back();
    if (nowPoint == group_dist[now][now][0][0]) {
        nowDist += group_dist[now][now][1].size();
    } else {
        nowDist += group_dist[now][now][0].size();
    }

    return nowDist;
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



    //ここまでok
    // 修正された行
    group_dist.resize(group_root.size(), vector<vector<vector<vector<int>>>>(group_root.size(), vector<vector<vector<int>>>(2, vector<vector<int>>())));

    for (int i = 0; i < group_root.size(); ++i) {
        for (int j = i + 1; j < group_root.size(); ++j) {
            group_dist[i][j] = calgroup_dist(group_edge[i], group_edge[j]);
            group_dist[j][i] = calgroup_dist(group_edge[j], group_edge[i]);
        }
    }

    for (int i = 0; i < group_root.size(); ++i) {
        group_dist[i][i] = calgroup_dist(group_edge[i], vector<int>{0, 0, 0, 0});
    }
    //ここまでok
    srand(time(0));

    int nowDist = rootDistance(group_root);
    time_t startTime = time(0);

    while (time(0) - startTime < 0.2) {
        vector<int> groupRootTemp = group_root;
        int i = rand() % group_root.size();
        int j = rand() % group_root.size();

        if (i == j)
            continue;

        if (i > j)
            swap(i, j);

        groupRootTemp.erase(groupRootTemp.begin() + i, groupRootTemp.begin() + j + 1);
        groupRootTemp.insert(groupRootTemp.begin() + i, group_root.begin() + i, group_root.begin() + j + 1);

        int tempDist = rootDistance(groupRootTemp);

        if (tempDist < nowDist) {
            nowDist = tempDist;
            group_root = groupRootTemp;
        }
    }
    cout << nowDist << endl;
    return 0;
}
