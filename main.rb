#盤面の大きさN×N 20<=N<=40
N = gets.to_i
#壁の多さはrand(1,N)
#上下の壁
H = (N-1).times.map{gets.split.map(&:to_i)}
#左右の壁
V = N.times.map{gets.split.map(&:to_i)}
#汚れやすさ 0<=d<=1000
D = N.times.map{gets.split.map(&:to_i)}


#探索済みかどうか 0:未探索 1:探索済み
dist = Array.new(N){Array.new(N, 0)}
ans_root = []
ans_root_str = ["U", "D", "L", "R"]


#長方形分割
H_div = Array.new(N-1){Array.new(N, 0)}
V_div = Array.new(N){Array.new(N-1, 0)}
for i in 0...(N-1)
    for j in 0...N
        next if H_div[i][j] == 1
        next if H[i][j] == 0
        H_div[i][j] = 1
        jj = j - 1
        while(jj >= 0)
            if V[i][jj] == 1 || V[i+1][jj] == 1
                break
            end
            H_div[i][jj] = 1
            jj -= 1
        end
        jj = j + 1
        while(jj < N)
            if V[i][jj-1] == 1 || V[i+1][jj-1] == 1
                break
            end
            H_div[i][jj] = 1
            jj += 1
        end
    end
end
for i in 0...N
    for j in 0...(N-1)
        next if V_div[i][j] == 1
        next if V[i][j] == 0
        V_div[i][j] = 1
        ii = i - 1
        while(ii >= 0)
            if H[ii][j] == 1 || H[ii][j+1] == 1
                break
            end
            V_div[ii][j] = 1
            ii -= 1
        end
        ii = i + 1
        while(ii < N)
            if H[ii-1][j] == 1 || H[ii-1][j+1] == 1
                break
            end
            V_div[ii][j] = 1
            ii += 1
        end
    end
end
p H_div, V_div