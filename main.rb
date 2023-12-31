#盤面の大きさN×N 20<=N<=40
N = gets.to_i
#壁の多さはrand(1,N)
#上下の壁
H = (N-1).times.map{gets.chomp.split("").map(&:to_i)}
#左右の壁
V = N.times.map{gets.chomp.split("").map(&:to_i)}
#汚れやすさ 0<=d<=1000
D = N.times.map{gets.split.map(&:to_i)}



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
            if V[i][jj] == 1 && V[i+1][jj] == 1
                break
            end
            H_div[i][jj] = 1
            jj -= 1
        end
        jj = j + 1
        while(jj < N)
            if V[i][jj-1] == 1 && V[i+1][jj-1] == 1
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



#グループ分け
#どのグループに属しているか
group_list = Array.new(N){Array.new(N, -1)}
#各グループのiのmin, jのmin, iのmax, jのmax
group_edge = []
group_num = 0


#幅優先探索によるグループ分け
dij = [[0, 1], [0, -1], [1, 0], [-1, 0]]
for i in 0...N
  for j in 0...N
    next if group_list[i][j] != -1
    group_edge << [i, j, i, j]
    group_list[i][j] = group_num
    que = [[i, j]]
    while !que.empty?
      x, y = que.shift
      #グループの端を更新
      group_edge[-1][0] = [group_edge[-1][0], x].min
      group_edge[-1][1] = [group_edge[-1][1], y].min
      group_edge[-1][2] = [group_edge[-1][2], x].max
      group_edge[-1][3] = [group_edge[-1][3], y].max

      #幅優先探索
      for k in 0..3
        move = dij[k]
        nx = x + move[0]; ny = y + move[1]
        next if nx < 0 || ny < 0 || nx >= N || ny >= N
        next if group_list[nx][ny] != -1
  
        if (move[0] == 0 && V_div[x][[y, ny].min] == 0) || (move[1] == 0 && H_div[[x, nx].min][y] == 0)
          group_list[nx][ny] = group_num
          que << [nx, ny]
        end
      end
    end
    group_num += 1
  end
end


#長方形ごとに何番目に通るかを考える
#初期解
$group_root = [0]
$dist = Array.new(N){Array.new(N, 0)}
def group_root_dfs(i,j,group_list)
  $dist[i][j] = 1
  dij = [[1, 0], [0, 1], [0, -1], [-1, 0]]
  4.times do |d|
    di, dj = dij[d]
    i2, j2 = i + di, j + dj
    if 0 <= i2 && i2 < N && 0 <= j2 && j2 < N && $dist[i2][j2] == 0
      if !($group_root.include?(group_list[i2][j2]))
        $group_root << group_list[i2][j2]
      end
      group_root_dfs(i2,j2,group_list)
    end
  end
end
group_root_dfs(0,0,group_list)




#各グループ間の最短距離を求める
def cal_group_dist(pre, nex)
  sum = [2000, 2000]
  look_list = [[0,1], [0,3], [2,1], [2,3]]
  dij = [[0, 1], [0, -1], [1, 0], [-1, 0]]
  root_first = []
  root_second = []
  root_flag = 0
  set = []
  for k in 0..3
    flag = 4
    dist = Array.new(N){Array.new(N,-1)}
    i1, j1 = pre[look_list[k][0]], pre[look_list[k][1]]
    next if set.include?([i1,j1])
    set << [i1, j1]
    que = [[i1, j1]]
    dist[i1][j1] = 0
    while !que.empty?
      x, y = que.shift
      #幅優先探索
      for m in 0..3
        move = dij[m]
        nx = x + move[0]; ny = y + move[1]
        next if nx < 0 || ny < 0 || nx >= N || ny >= N
        next if dist[nx][ny] != -1
        if (move[0] == 0 && V[x][[y, ny].min] == 0) || (move[1] == 0 && H[[x, nx].min][y] == 0)
          dist[nx][ny] = dist[x][y] + 1
          que << [nx, ny]
        end
      end
    end
    kk = 0
    now_value = dist[nex[look_list[kk][0]]][nex[look_list[kk][1]]]
    for kkk in 1..3
      if now_value > dist[nex[look_list[kkk][0]]][nex[look_list[kkk][1]]]
        now_value = dist[nex[look_list[kkk][0]]][nex[look_list[kkk][1]]]
        kk = kkk
      end
    end
    if sum[0] > dist[nex[look_list[kk][0]]][nex[look_list[kk][1]]]
      sum[0] = dist[nex[look_list[kk][0]]][nex[look_list[kk][1]]]
      flag = kk
      root_flag = 0
     elsif sum[1] > dist[nex[look_list[kk][0]]][nex[look_list[kk][1]]]
      sum[1] = dist[nex[look_list[kk][0]]][nex[look_list[kk][1]]]
      flag = kk
      root_flag = 1
     end
    next if flag == 4

    #最短距離の頂点を取得
    iii, jjj = nex[look_list[flag][0]], nex[look_list[flag][1]]
    score = dist[iii][jjj]
    root = [[iii, jjj]]
    x, y = iii, jjj
    #復元
    while score > 0
      for k in 0..3
        move = dij[k]
        nx = x + move[0]; ny = y + move[1]
        next if nx < 0 || ny < 0 || nx >= N || ny >= N
        if (move[0] == 0 && V[x][[y, ny].min] == 0) || (move[1] == 0 && H[[x, nx].min][y] == 0)
          if dist[nx][ny] == score - 1
            root << [nx, ny]
            x, y = nx, ny
            score -= 1
            break
          end
        end
      end
    end
    if root_flag == 0
      root_second = root_first.map(&:dup)
      root_first = root.map(&:dup)
      root_first.reverse!
    elsif root_flag == 1
      root_second = root.map(&:dup)
      root_second.reverse!
    end
  end
  if root_second == []
    root_second = root_first.map(&:dup)
  end
  return [root_first, root_second]
end

$group_dist = Array.new($group_root.size){Array.new($group_root.size){Array.new()}}
for i in 0...$group_root.size
  for j in (i+1)...$group_root.size
    $group_dist[i][j] = cal_group_dist(group_edge[i], group_edge[j])
    $group_dist[j][i] = cal_group_dist(group_edge[j], group_edge[i])
  end
end
#0,0に戻るときの距離
for i in 0...$group_root.size
  $group_dist[i][i] = cal_group_dist(group_edge[i], [0,0,0,0])
end



#初期解
def root_distance(root)
  now_dist = 0
  now_point = [0,0]#現在の座標
  for i in 1...root.size
    now = root[i]
    pre = root[i-1]
    if now_point == $group_dist[pre][now][0][0]
      now_dist += $group_dist[pre][now][1].size
      now_point = $group_dist[pre][now][1][-1]
    else
      now_dist += $group_dist[pre][now][0].size
      now_point = $group_dist[pre][now][0][-1]
    end
  end
  #最後に0,0に戻るからその分も足さないといけない
  now = root[-1]
  if now_point == $group_dist[now][now][0][0]
    now_dist += $group_dist[now][now][1].size
  else
    now_dist += $group_dist[now][now][0].size
  end
  return now_dist
end
now_dist = root_distance($group_root)


#2-opt法
time = Time.now
size = $group_root.size
if size > 1
while Time.now - time < 0.2
  group_root_temp = $group_root.dup
  i = rand(1...size)
  j = rand(1...size)
  next if i == j
  i, j = j, i if i > j
  group_root_temp = group_root_temp[0...i] + group_root_temp[i..j].reverse + group_root_temp[j+1...size]

  temp_dist = root_distance(group_root_temp)

  #変更後のルートの方が短ければ更新
  if temp_dist < now_dist
    now_dist = temp_dist
    $group_root = group_root_temp.dup
  end
end
end





#つなげていく
def group_inner_dfs(now_point, next_point, cover_range, flag)
  i, j = now_point
  $dist[i][j] = 1
  imin, jmin, imax, jmax = cover_range
  #最後の2マス間隔のときにflagを変える場合もある
  if flag == 1 && [jmin, jmax].include?(j)
    if (next_point[0] - i).abs == 1 && (next_point[1] - j).abs != 0
      if $change_flag == 0
        $change_flag = 1
        flag = 2
      end
    end
  elsif flag == 2 && [imin, imax].include?(i)
    if (next_point[1] - j).abs == 1 && (next_point[0] - i).abs != 0
      if $change_flag == 0
        $change_flag = 1
        flag = 1
      end
    end
  end

  if flag == 1#横優先
    dij = [[0, 1], [0, -1], [1, 0], [-1, 0]]
  else#縦優先
    dij = [[1, 0], [-1, 0], [0, 1], [0, -1]]
  end
  4.times do |d|
    di, dj = dij[d]
    i2, j2 = i + di, j + dj
    if imin <= i2 && i2 <= imax && jmin <= j2 && j2 <= jmax && $dist[i2][j2] == 0
      $group_inner_root_list << [i2, j2]
      group_inner_dfs([i2, j2], next_point, cover_range, flag)
    end
  end
end
def group_inner_root(now_point,next_point,cover_range)
  #例外処理
  if now_point == next_point
    return []
  end
  #now_pointからnext_pointへの移動
  $group_inner_root_list = []
  $dist = Array.new(N){Array.new(N, 0)}
  $change_flag = 0
  if now_point[0] == next_point[0]
    #縦から始めたい
    group_inner_dfs(now_point, next_point, cover_range, 2)
  else
    group_inner_dfs(now_point, next_point, cover_range, 1)
  end

  if $group_inner_root_list[-1] != next_point
    $group_inner_root_list << next_point
  end
  return $group_inner_root_list
end

#0,0から0,0が含まれる長方形の頂点への移動
ans_root = [[0,0]]
now_point = [0,0]
root_flag = 0
if now_point == $group_dist[0][$group_root[1]][0][0]
  next_point = $group_dist[0][$group_root[1]][1][0]
  root_flag = 1
else
  next_point = $group_dist[0][$group_root[1]][0][0]
end
#頂点から頂点への移動
inner_root = group_inner_root(now_point, next_point, group_edge[0])
inner_root.each{|ir| ans_root << ir}

#group->groupを繋げる
outer_root = $group_dist[0][$group_root[1]][root_flag].map(&:dup)
outer_root.shift
outer_root.each{|io| ans_root << io}
#now_pointの更新
now_point = ans_root[-1]

#頂点->頂点、塗りつぶしを繰り返す
for group_num in 2...$group_root.size
  root_flag = 0
  now_group = $group_root[group_num - 1]
  next_group = $group_root[group_num]
  if now_point == $group_dist[now_group][next_group][0][0]
    next_point = $group_dist[now_group][next_group][1][0]
    root_flag = 1
  else
    next_point = $group_dist[now_group][next_group][0][0]
  end
  #頂点から頂点への移動
  inner_root = group_inner_root(now_point, next_point, group_edge[now_group])
  inner_root.each{|ir| ans_root << ir}
  #group->groupを繋げる
  outer_root = $group_dist[now_group][next_group][root_flag].map(&:dup)
  outer_root.shift
  outer_root.each{|io| ans_root << io}
  #now_pointの更新
  now_point = ans_root[-1]
end


#最後の頂点->0,0
root_flag = 0
last_group = $group_root[-1]
if now_point == $group_dist[last_group][last_group][0][0]
  next_point = $group_dist[last_group][last_group][1][0]
  root_flag = 1
else
  next_point = $group_dist[last_group][last_group][0][0]
end
#頂点から頂点への移動
inner_root = group_inner_root(now_point, next_point, group_edge[last_group])
inner_root.each{|ir| ans_root << ir}
#group->[0,0]を繋げる
outer_root = $group_dist[last_group][last_group][root_flag].map(&:dup)
outer_root.shift
outer_root.each{|io| ans_root << io}



#答えを出力
ans_str = []
now = [0,0]
for i in 1...ans_root.size
  if ans_root[i][0] == now[0]
    if ans_root[i][1] > now[1]
      ans_str << "R"
    else
      ans_str << "L"
    end
  else
    if ans_root[i][0] > now[0]
      ans_str << "D"
    else
      ans_str << "U"
    end
  end
  now =ans_root[i]
end
puts ans_str.join("")