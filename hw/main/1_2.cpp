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

bool is_target[berth_num];


/*

for(int i =0 ;i <berth_num;i++)
{
    is_target[i] = 0;
}

int robot_get_target_berth()
{
    for(int i =0; i <berth_num;i++)
    {
        if(is_target[i])return i;
    }
    return 0;
}

int boat_get_target()
{
    for(int i =0 ;i< berth_num;i++)
    {
        if(berth[i] > 0  && is_target[i] == 0)
        {
            is_target[i] = 1;
            return i;
        }
    }
    return 0;
}
*/



struct Goods
{
    int x, y;
    int val;
    int appear;

}goods[M];
int goods_cnt = 0 ,now_idx = 0;

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
        for(int j = 0; j < 3; j ++ ){//泊位都是0
            for(int k = 0; k < 3; k ++){
                
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
    // for(int i =0; i <berth_num;i++)
    // {
    //     if(is_target[i])return i;
    // }

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
        goods[goods_cnt].x = x;
        goods[goods_cnt].y = y;
        goods[goods_cnt].val = val;
        goods[goods_cnt].appear = id;
        goods_cnt++;
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
    d1[robot_id][goods[idx].x][goods[idx].y] = 0;
    q.push({{goods[idx].x,goods[idx].y},0});
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
    for(int i = 0; i < robot_id; i ++ ){
        if(robot_pos[i].ne_x == xx && robot_pos[i].ne_y == yy)return robot_num + 2;
        if(robot_pos[i].now_x == xx && robot_pos[i].now_y == yy && robot_pos[i].ne_x == x && robot_pos[i].ne_y == y)return i;
    }
    return robot_num + 1;
}


void change_op(int s,ofstream&log){
    try_get[s] = 0;
    log<<" change_op ";
    for(int i = 0; i < 4; i++){
        int xx = robot[s].x + dx[i], yy = robot[s].y + dy[i];
        if (xx < 0 || xx >= n || yy < 0 || yy >= n) continue;
        if (ch[xx][yy] == '*' || ch[xx][yy] == '#') continue;
        if(isempty(s,xx,yy) == robot_num + 1){
            printf("move %d %d\n",s,i);
            log<<"move "<<i;
            robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
            break;
        }
    }
}
int main()
{   
    ofstream log("log.txt", std::ios_base::app); 
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        log << "zhen"<<zhen <<"a"<<endl;
        int id = Input();

        for(int i = 0; i < robot_num; i ++){
            log<<"机器人"<<i<<"-";
            robot_pos[i] = { robot[i].x, robot[i].y, robot[i].x, robot[i].y };
            if(robot[i].target_id == -1){
                try_get[i] = 0;
                log <<"fenpei";        
                if(now_idx < goods_cnt){
                    while(now_idx< goods_cnt && (id - goods[now_idx].appear) >=500 )now_idx++;
                    robot[i].target_id = now_idx;
                    int t = get_road(now_idx,i);
                    now_idx++;
                    if(t>1000-(id - goods[now_idx].appear)){
                        robot[i].target_id = now_idx;
                        int t = get_road(now_idx,i);
                        now_idx++;
                    }
                }
            }
            else if(robot[i].status == 0){
                log<<"stop "<<endl;
            }
            else if(robot[i].goods == 1 && d2[robot[i].target_id][robot[i].x][robot[i].y] == 0){
                log<<2;
                try_get[i] = 0;
                int st = isempty(i,robot[i].x,robot[i].y);
                log<<" st="<<st<<" ";
                if(st == robot_num + 1){
                    printf("pull %d\n",i);
                    log<<"pull "<<endl;
                    if(now_idx < goods_cnt){
                        while(now_idx< goods_cnt && (id - goods[now_idx].appear) >=500 )now_idx++;
                        robot[i].target_id = now_idx;
                        int t = get_road(now_idx,i);
                        now_idx++;
                        if(t>1000-(id - goods[now_idx].appear)){
                            robot[i].target_id = now_idx;
                            int t = get_road(now_idx,i);
                            now_idx++;
                        }
                    }
                }
                else {
                    change_op(i,log);
                }
                
            }
            else if(robot[i].goods == 0 && d1[i][robot[i].x][robot[i].y] == 0){
                log<<3;
                int st = isempty(i,robot[i].x,robot[i].y);
                log<<" st="<<st<<" ";
                if(st == robot_num + 1 ){
                    if( try_get[i] == 0){
                        printf("get %d\n",i);
                        try_get[i] = 1;
                        log<<"get "<<endl;
                        robot_get_target_berth(i);
                    }
                    else{
                        if(now_idx < goods_cnt){
                            try_get[i] = 0;
                            while(now_idx< goods_cnt && (id - goods[now_idx].appear) >=500 )now_idx++;
                            robot[i].target_id = now_idx;
                            int t = get_road(now_idx,i);
                            now_idx++;
                            if(t>1000-(id - goods[now_idx].appear)){
                                robot[i].target_id = now_idx;
                                int t = get_road(now_idx,i);
                                now_idx++;
                            }
                        }
                    }
                }
                else{
                    change_op(i,log);
                }
            }
            else if(robot[i].goods  == 1 ){
                log<<4;
                int flag = 0;
                for(int j = 0; j < 4; j ++ ){
                    int xx = robot[i].x + dx[j], yy = robot[i].y + dy[j];
                    if(d2[robot[i].target_id][xx][yy] < d2[robot[i].target_id][robot[i].x][robot[i].y]){
                        int st = isempty(i,xx,yy);
                        log<<" st="<<st<<" ";
                        if(st == robot_num + 1){
                            try_get[i] = 0;
                            printf("move %d %d\n",i,j);
                            log<<"move "<<j;
                            robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
                            flag = 1;
                            break;
                        }
                    }
                }
                if(!flag){
                    int st = isempty(i,robot[i].x,robot[i].y);
                    if(st == robot_num + 1){
                        try_get[i] = 0;
                        log<<"wait "<<endl;
                        ;
                    }
                    else{
                        change_op(i,log);
                    }
                }
            }
            else if(robot[i].goods == 0){
                log<<5 <<" "<<d1[i][robot[i].x][robot[i].y];
                int flag = 0;
                for(int j = 0; j < 4; j ++ ){
                    int xx = robot[i].x + dx[j], yy = robot[i].y + dy[j];
                    if(d1[i][xx][yy] < d1[i][robot[i].x][robot[i].y] ){
                        int st = isempty(i,xx,yy);
                        if(st == robot_num + 1)
                        {
                            try_get[i] = 0;
                            log<<" st="<<st<<" ";
                            printf("move %d %d\n",i,j);
                            log<<"move "<<j;
                            flag = 1;
                            robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
                            break;
                        }
                    }
                }
                if(!flag){
                    int st = isempty(i,robot[i].x, robot[i].y);
                    if(st == robot_num + 1){
                        try_get[i] = 0;
                        log<<"wait "<<endl;
                    }
                    else{
                        log<<" st="<<st<<" ";
                        change_op(i,log);
                    }
                }
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

    log.close();
    return 0;
}
