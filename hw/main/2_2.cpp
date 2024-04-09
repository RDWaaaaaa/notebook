#include <bits/stdc++.h>
#include<iostream>
using namespace std;
typedef pair<int,int> PII;
const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int boat_num = 5;
const int N = 210;
const int M = 150010;

int dx[] = {0, 0, -1, 1},dy[] = {1, -1, 0, 0};//右左上下
int boat_idx;
int d1[robot_num+1][N][N],d2[berth_num+1][N][N];
int try_get[robot_num + 10];
struct robot_pos{
    int now_x,now_y,ne_x,ne_y;
}robot_pos[robot_num+10];
int g[N][N],gcnt;
bool is_target[berth_num];
priority_queue<PII,vector<PII>,greater<PII> > priq[10];

int sum = 0;

struct Goods
{
    int x, y;
    int val;
    int appear;
    bool is_get;
}goods[berth_num+1][M];
int goods_cnt[berth_num+1] ,now_idx[berth_num+1];

struct Robot
{
    int x, y, goods;
    int status;
    // int mbx, mby;
    int target_id;
    int val;
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
}robot[robot_num + 10];

struct Berth
{
    int x;
    int y;
    int transport_time;
    int loading_speed;
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
    int val;
}berth[berth_num + 10];

struct Boat
{
    int num, pos, status;
}boat[10];

int money, boat_capacity, id;
char ch[N][N];
int gds[N][N];
int robo_move[robot_num][2];

void berth_to_point(){
    for(int i = 0; i < berth_num; i ++ ){//不可达
        for(int j = 0; j < n; j ++){
            for(int k = 0; k < n; k ++ ){
                d2[i][j][k] = n*n;
            }
        }
    }
    for(int i = 0; i < berth_num; i ++ ){
        queue< pair<PII,int> > q;
        for(int j = 0; j < 4; j ++ ){//泊位都是0
            for(int k = 0; k < 4; k ++){
                
                int x = berth[i].x + j, y = berth[i].y + k;
                d2[i][x][y] = 0;
                q.push({{x,y},0});
            }
        }
        while(!q.empty()){//bfs
            auto t = q.front();
            q.pop();
            int d = t.second;
            int x = t.first.first;
            int y = t.first.second;
            for(int j = 0; j < 4; j ++ ){
                int xx = x + dx[j], yy = y + dy[j];
                if(xx<0||xx>=n||yy<0||yy>=n){
                    continue;
                }
                if(ch[xx][yy] == '*' || ch[xx][yy] == '#'||ch[xx][yy] == 'B'){
                    continue;
                }
                if(d+1<d2[i][xx][yy]){
                    d2[i][xx][yy] = d + 1;
                    q.push({{xx,yy}, d + 1});
                }
            }
        }
    }
}

void init_robot(){
    for(int i = 0; i < robot_num; i ++ ){
        robot[i].target_id = -1;
    }
}

void init_berth()
{
    for(int i =0 ;i <berth_num;i++)
    {
        is_target[i] = 0;
        berth[i].val = 0;
    }
    return ;
}

void robot_get_target_berth(int idx)
{
    robot[idx].target_id = 0;
    int min_idx=0;
    int x = robot[idx].x, y = robot[idx].y;
    for(int i = 0;i<berth_num;i++){
        if(d2[i][x][y] < d2[min_idx][x][y]){
            min_idx = i;
        }
    }
    robot[idx].target_id = min_idx;
}

int boat_get_target()
{
    for(int i =0 ;i< berth_num;i++)
    {
        if(berth[i].val > 0  && is_target[i] == 0)
        {
            is_target[i] = 1;
            return i;
        }
    }
    return 0;
}

void init_graph(){
    ofstream log("graph.txt", std::ios_base::app); 
    queue<PII>q;
    for(int i = 0; i < berth_num; i ++ ){
        if(g[berth[i].x][berth[i].y])continue;
        gcnt++;
        g[berth[i].x][berth[i].y] = gcnt;
        q.push({berth[i].x,berth[i].y});
        while(!q.empty()){//bfs
            PII t = q.front();
            q.pop();
            int x = t.first, y = t.second;
            log<<"check"<<x<<","<<y<<endl;
            for(int j = 0; j < 4; j ++){
                int xx = x + dx[j], yy = y + dy[j];
                log<<endl;
                if(xx<0||xx>=n||yy<0||yy>=n){
                    log<<"case1";
                    continue;
                }
                if(ch[xx][yy] == '*' || ch[xx][yy] == '#'){
                    log<<"case2";
                    continue;
                }
                if(g[xx][yy]){
                    log<<"case3";
                    continue;
                }
                log<<"push"<<xx<<","<<yy;
                g[xx][yy] = gcnt;
                q.push({xx,yy});
            }
        }
    }
    log<<endl;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            log<<g[i][j];
        }
        log<<endl;
    }
    log.close();
}

void Init()
{
    for(int i = 0; i < n; i ++)
        scanf("%s", ch[i]);
    
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    init_graph();
    berth_to_point();
    init_robot();
    init_berth();
    for(int i = 0; i < boat_num ; i ++){
        boat[i].num = 0;
    }
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for(int i = 1; i <= num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        int gg = g[x][y];
        goods[gg][goods_cnt[gg]].x = x;
        goods[gg][goods_cnt[gg]].y = y;
        goods[gg][goods_cnt[gg]].val = val;
        goods[gg][goods_cnt[gg]].appear = id;
        goods[gg][goods_cnt[gg]].is_get = 0;
        //int min_d = 1e9;
        for(int j = 0; j< berth_num;j++)
        {
            if(g[berth[j].x][berth[j].y] == gg){
                priq[j].push({d2[j][x][y],goods_cnt[gg]});
            }
        }
        goods_cnt[gg]++;
    }
    for(int i = 0; i < robot_num; i ++)
    {
        int sts;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
    }
    for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
    char okk[100];
    scanf("%s", okk);
    return id;
}

int get_road(int idx,int robot_id){
    for(int i = 0; i < n; i ++ ){
        for(int j = 0; j < n; j ++ ){
            d1[robot_id][i][j] = n*n;
        }
    }
    queue< pair<PII, int> > q;
    int gg = g[robot[robot_id].x][robot[robot_id].y];
    d1[robot_id][goods[gg][idx].x][goods[gg][idx].y] = 0;
    q.push({{goods[gg][idx].x,goods[gg][idx].y},0});
    while(!q.empty()){//bfs
        auto t = q.front();
        q.pop();
        int d = t.second;
        int x = t.first.first;
        int y = t.first.second;
        for(int j = 0; j < 4; j ++ ){
            int xx = x + dx[j], yy = y + dy[j];
            if(xx<0||xx>=n||yy<0||yy>=n)continue;
            if(ch[xx][yy] == '*' || ch[xx][yy] == '#')continue;
            if(d1[robot_id][xx][yy] >= 0 && d1[robot_id][xx][yy] <= d + 1)continue;
            d1[robot_id][xx][yy] = d + 1;
            q.push({{xx,yy}, d + 1});
        }
    }
    return d1[robot_id][robot[robot_id].x][robot[robot_id].y];
}

int isempty(int robot_id, int xx, int yy){
    int x = robot[robot_id].x, y = robot[robot_id].y;
    for(int i = 0; i < robot_num; i ++ ){
        if(i == robot_id) continue;
        if(robot_pos[i].ne_x == xx && robot_pos[i].ne_y == yy)return robot_num + 2;
        if(robot_pos[i].now_x == xx && robot_pos[i].now_y == yy && robot_pos[i].ne_x == x && robot_pos[i].ne_y == y)return i;
    }
    return robot_num + 1;
}

int change_op(int s,ofstream&log){
    try_get[s] = 0;
    log<<" change_op ";
    //log<<robot[s]
    for(int i = 0; i < 4; i++){
        int xx = robot[s].x + dx[i], yy = robot[s].y + dy[i];
        if (xx < 0 || xx >= n || yy < 0 || yy >= n) continue;
        if (ch[xx][yy] == '*' || ch[xx][yy] == '#') continue;
        if(isempty(s,xx,yy) == robot_num + 1){
            // printf("move %d %d\n",s,i);
            // log<<"move "<<i;
            robo_move[s][0] = 3;
            robo_move[s][1] = i;
            robot_pos[s] = { robot[s].x, robot[s].y, xx, yy };
            return i;
        }
    }
    return -1;
}

void other_move(int preid, int robot_id, ofstream &log)
{
    int st[4];
    log<<robot_id<<" remove"<<endl;
    for(int i = 0;i< 4;i++)
    {
        int xx = robot[robot_id].x + dx[i];
        int yy = robot[robot_id].y + dy[i];
        st[i] = isempty(robot_id,xx,yy);
        if(st[i] == robot_num + 1)
        {
            if(robo_move[robot_id][0] != 3)
            {
                robo_move[robot_id][0] = 3;
            }
            robo_move[robot_id][1] = i;
            robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
            return ;
        }
    }
    for(int i = 0;i<4;i++)
    {
        if(st[i]!=preid)
        {
            other_move(robot_id, st[i],log);
            break;
        }
    }
    return ;
}

int main()
{   
    ofstream log("log.txt"); 
    Init();
    
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        log << "zhen"<<zhen <<"a"<<endl;
        int id = Input();

        for(int i = 0; i < robot_num; i ++){
            log<<"机器人一轮指令"<<i<<"-";
            int gg = g[robot[i].x][robot[i].y];
            robot_pos[i] = { robot[i].x, robot[i].y, robot[i].x, robot[i].y };
            robo_move[i][0]=-1;
            if(robot[i].target_id == -1){
                try_get[i] = 0;
                log <<"fenpei"<<endl;     
                int _g = 0;
                for(int j = 0;j<berth_num;j++)
                {
                    if(g[berth[j].x][berth[j].y] == gg){
                        _g = j;
                        break;
                    }
                }
                while((!priq[_g].empty()) )
                {
                    if(goods[gg][priq[_g].top().second].is_get == 1)
                    {
                        priq[_g].pop();
                    }
                    else break;
                    log<<1;
                } 
                if(priq[_g].empty())
                {
                    robot[i].target_id = -1;
                    continue;
                }
                int idx = priq[_g].top().second;  
                priq[_g].pop();
                int t = get_road(idx,i);
                robot[i].target_id = idx;
                if(t + 10 >1000-(id - goods[gg][idx].appear))
                {
                    idx = priq[_g].top().second;  
                    priq[_g].pop();
                    t = get_road(idx,i);
                    robot[i].target_id = idx;
                }
                goods[gg][idx].is_get = 1;
                robot[i].val = 0;

                // if(now_idx[gg] < goods_cnt[gg]){
                //     while(now_idx[gg]< goods_cnt[gg] && (id - goods[gg][now_idx[gg]].appear) >=500 )now_idx[gg]++;
                //     robot[i].target_id = now_idx[gg];
                //     int t = get_road(now_idx[gg],i);
                //     now_idx[gg]++;
                //     if(t>1000-(id - goods[gg][now_idx[gg]].appear)){
                //         robot[i].target_id = now_idx[gg];
                //         int t = get_road(now_idx[gg],i);
                //         now_idx[gg]++;
                //     }
                // }
            }
            else if(robot[i].status == 0){
                log<<"stop "<<endl;
            }
            else if(robot[i].goods == 1 && d2[robot[i].target_id][robot[i].x][robot[i].y] == 0){
                log<<2;
                try_get[i] = 0;
                robo_move[i][0] = 1;    
            }
            else if(robot[i].goods == 0 && d1[i][robot[i].x][robot[i].y] == 0){
                log<<3;
                robo_move[i][0] = 2;
            }
            else if(robot[i].goods  == 1 ){
                log<<4;
                int flag = 0;
                for(int j = 0; j < 4; j ++ ){
                    int xx = robot[i].x + dx[j], yy = robot[i].y + dy[j];
                    if(d2[robot[i].target_id][xx][yy] < d2[robot[i].target_id][robot[i].x][robot[i].y]){
                        robo_move[i][0] = 3;
                        robo_move[i][1] = j;
                        robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
                        break;
                    }
                }
            }
            else if(robot[i].goods == 0){
                log<<5 ;
                int flag = 0;
                for(int j = 0; j < 4; j ++ ){
                    int xx = robot[i].x + dx[j], yy = robot[i].y + dy[j];
                    if(d1[i][xx][yy] < d1[i][robot[i].x][robot[i].y] ){
                        robo_move[i][0] = 3;
                        robo_move[i][1] = j;
                        robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
                        break;
                    }
                }
            }
            log<<endl;
        }
        int f = 0;
        for(int i = 0; i< robot_num;i++)
        {
            log<<"机器人修正指令"<<i<<"-";
            int gg = g[robot[i].x][robot[i].y];
            if(robo_move[i][0] == 1)
            {
                log<< 1;
                int st = isempty(i,robot[i].x, robot[i].y);
                if(st != robot_num + 1 ){
                    int res = change_op(i,log);
                    if(res == -1)
                    {
                        other_move(i,st,log);
                    }
                    log<<endl;
                }
            }
            else if(robo_move[i][0] == 2)
            {
                log<< 2;
                int st = isempty(i,robot[i].x, robot[i].y);
                if(st != robot_num + 1 ){
                    int res = change_op(i,log);
                    if(res == -1)
                    {
                        other_move(i,st,log);
                    }
                    log<<endl;
                }
            }
            else if(robo_move[i][0] == 3)
            {
                log<< 3;
                int st = isempty(i,robot_pos[i].ne_x , robot_pos[i].ne_y);
                if(st != robot_num + 1 ){
                    int st = isempty(i,robot[i].x,robot[i].y);
                    if(st == robot_num + 1){
                        log<<"wait ";
                        robo_move[i][0] = -1;
                        robot_pos[i] = { robot[i].x, robot[i].y, robot[i].x, robot[i].y };
                    }
                    else{
                        int res = change_op(i,log);
                        if(res != -1)
                        {
                            other_move(i,st,log);
                        }
                        log<<endl;
                    }
                }
            }
            log<<endl;
        }
        for(int i=0;i<robot_num;i++)
        {
            log<<"机器人执行指令"<<i<<"-";
            int gg = g[robot[i].x][robot[i].y];
            if(robo_move[i][0] == 1)
            {
                sum+=robot[i].val;
                robot[i].val = 0;
                printf("pull %d\n",i);
                log<<"pull ";
                try_get[i] = 0;
                int _g = robot[i].target_id;
                while(!priq[_g].empty())
                {
                    if(goods[gg][priq[_g].top().second].is_get == 1)
                        priq[_g].pop(); 
                    else break;
                }
                if(priq[_g].empty())
                {
                    robot[i].target_id = -1;
                    continue;
                }
                
                int idx = priq[_g].top().second;  
                priq[_g].pop();
                int t = get_road(idx,i);
                robot[i].target_id = idx;
                if(t + 10 >1000-(id - goods[gg][idx].appear))
                {
                    idx = priq[_g].top().second;  
                    priq[_g].pop();
                    t = get_road(idx,i);
                    robot[i].target_id = idx;
                }
                goods[gg][idx].is_get =1;
                robot[i].val = 0;

                // if(now_idx < goods_cnt){
                //     while(now_idx[gg]< goods_cnt[gg] && (id - goods[gg][now_idx[gg]].appear) >=500 )now_idx[gg]++;
                //     robot[i].target_id = now_idx[gg];
                //     int t = get_road(now_idx[gg],i);
                //     now_idx[gg]++;
                //     if(t>1000-(id - goods[gg][now_idx[gg]].appear)){
                //         robot[i].target_id = now_idx[gg];
                //         int t = get_road(now_idx[gg],i);
                //         now_idx[gg]++;
                //     }
                // }
            }
            else if(robo_move[i][0] == 2)
            {
                if( try_get[i] == 0){
                    printf("get %d\n",i);
                    try_get[i] = 1;
                    log<<"get ";
                    robot[i].val = goods[gg][robot[i].target_id].val;
                    robot_get_target_berth(i);
                }
                else{
                    int _g = robot[i].target_id;
                    while((!priq[_g].empty()))
                    {
                        if(goods[gg][priq[_g].top().second].is_get == 1)
                            priq[_g].pop(); 
                        else break;
                    }
                    if(priq[_g].empty())
                    {
                        robot[i].target_id = -1;
                        continue;
                    }
                    
                    int idx = priq[_g].top().second;  
                    priq[_g].pop();
                    int t = get_road(idx,i);
                    robot[i].target_id = idx;
                    if(t + 10 >1000-(id - goods[gg][idx].appear))
                    {
                        idx = priq[_g].top().second;  
                        priq[_g].pop();
                        t = get_road(idx,i);
                        robot[i].target_id = idx;
                    }
                    goods[gg][idx].is_get =1;
                    try_get[i] = 0;
                    robot[i].val = 0;

                    // if(now_idx[gg] < goods_cnt[gg]){
                    //     
                    //     while(now_idx[gg]< goods_cnt[gg] && (id - goods[gg][now_idx[gg]].appear) >=500 )now_idx[gg]++;
                    //     robot[i].target_id = now_idx[gg];
                    //     int t = get_road(now_idx[gg],i);
                    //     now_idx[gg]++;
                    //     if(t>1000-(id - goods[gg][ now_idx[gg] ].appear)){
                    //         robot[i].target_id = now_idx[gg];
                    //         int t = get_road(now_idx[gg],i);
                    //         now_idx[gg]++;
                    //     }
                    // }
                }
            }
            else if(robo_move[i][0] == 3)
            {
                try_get[i] = 0;
                printf("move %d %d\n",i,robo_move[i][1]);
                log<<"move "<<robo_move[i][1];
            }
            log<<endl;
        }

        log<<"pos"<<endl;
        for(int i=0;i<robot_num;i++){
            log<<"robot"<<i<<" "<<robot_pos[i].now_x<<" "<<robot_pos[i].now_y<<" "<<robot_pos[i].ne_x<<" "<<robot_pos[i].ne_y<<endl;
        }
        for(int i = 0; i < boat_num; i ++){
            log <<"船-"<<boat[i].status;
            if(boat[i].status == 0 || boat[i].status == 2)
            {
                log<<endl;
                continue;
            }
            else if(boat[i].status == 1 && boat[i].pos != -1){
                boat[i].num ++;
                if(boat[i].num>100)
                {
                    printf("go %d\n",i);
                    boat[i].num = 0;
                    log<<" go";
                }
                else if(15000 - id == berth[boat[i].pos].transport_time + 1)
                {
                    printf("go %d\n",i);
                }
            }
            else if(boat[i].status == 1 && boat[i].pos == -1){
                // boat_idx = boat_get_target();
                printf("ship %d %d\n", i, boat_idx);
                boat_idx = (boat_idx + 1) % 10;
                log<<" ship";
                
            }
            log<<endl;
        }
        puts("OK");
        fflush(stdout);
    }
    log<<sum<<endl;
    log.close();
    return 0;
}
