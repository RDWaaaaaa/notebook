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

int dx[] = {0, 0, -1, 1},dy[] = {1, -1, 0, 0};
int boat_idx;
int d1[robot_num+1][N][N],d2[berth_num+1][N][N];
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
    // ofstream log("log.txt", std::ios_base::app); 
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

void get_road(int idx,int robot_id){
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
}
int isempty(int robot_id, int xx, int yy){
    int x = robot[robot_id].x, y = robot[robot_id].y;
    for(int i = 0; i < robot_id; i ++ ){
        if(robot_pos[i].ne_x == xx && robot_pos[i].ne_y == yy)return robot_num + 2;
        if(robot_pos[i].now_x == xx && robot_pos[i].now_y == yy && robot_pos[i].ne_x == x && robot_pos[i].ne_y == y)return i;
    }
    return robot_num + 1;
}

int robo_move[robot_num][1];

void change(int s,int t,int type)
{
    if(type == 0)
    {
        swap(robot[s].target_id, robot[t].target_id);
        if(robot[s].goods == 1 )return ;
        for(int i = 0;i < n;i++)
        {
            for(int j = 0;j < n;j++)
            {
                swap(d1[s][i][j],d1[t][i][j]);
            }
        }
        robot_pos[s].ne_x = robot_pos[s].now_x;
        robot_pos[s].ne_y = robot_pos[s].now_y;
        robot_pos[t].ne_x = robot_pos[t].now_x;
        robot_pos[t].ne_y = robot_pos[t].now_y;
        robo_move[s][0] = robo_move[t][0] = -1;
    }
    else{
        swap(robot[s].target_id, robot[t].target_id);
        for(int i = 0;i < n;i++)
        {
            for(int j = 0;j < n;j++)
            {
                swap(d1[s][i][j],d1[t][i][j]);
            }
        }
        if(robot[s].goods == 0) swap(s,t);
        printf("pull %d\n",s);
        for(int i = 0;i < 4; i ++)
        {
            int xx = robot[s].x + dx[i], yy = robot[s].y + dy[i];
            if(xx<0||xx>=n||yy<0||yy>=n)continue;
            if(ch[xx][yy] == '*' || ch[xx][yy] == '#')continue;
            if(xx != robot[t].x || yy!= robot[t].y)
            {
                printf("move %d %d\n",s,i);
                robot_pos[s].ne_x = xx;
                robot_pos[s].ne_y = yy;
                robo_move[s][0] = -1;
                break;
            } 
        }
        for(int i = 0;i < 4; i ++)
        {
            int xx = robot[t].x + dx[i], yy = robot[t].y + dy[i];
            if(xx == robot[s].x && yy== robot[s].y)
            {
                printf("move %d %d\n",s,i);
                robot_pos[t].ne_x = xx;
                robot_pos[t].ne_y = yy;
                robo_move[t][0] = -1;
                break;
            } 
        }
        printf("get %d\n",t);
    }
    return ;
}

int main()
{   
    ofstream log("log.txt", std::ios_base::app); 
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        log << zhen <<endl;
        int id = Input();
        for(int i = 0; i < robot_num; i ++){
            log<<"机器人"<<i<<"-";
            robo_move[i][0]=-1;
            robot_pos[i] = { robot[i].x, robot[i].y, robot[i].x, robot[i].y };
            if(robot[i].target_id == -1){
                log <<0;        
                if(now_idx < goods_cnt){
                    while(now_idx< goods_cnt && (id - goods[now_idx].appear) >=1000  )now_idx++;
                    robot[i].target_id = now_idx;
                    get_road(now_idx,i);
                    now_idx++;
                }
            }
            else if(robot[i].status == 0){
                log<<1;
            }
            else if(robot[i].goods == 1 && d2[robot[i].target_id][robot[i].x][robot[i].y] == 0){
                log<<2;
                printf("pull %d\n",i);
                log<<"pull "<<i<<endl;
                berth[robot[i].target_id].val++;
                if(now_idx < goods_cnt){
                    while(now_idx< goods_cnt && (id - goods[now_idx].appear) >=1000 )now_idx++;
                    robot[i].target_id = now_idx;
                    get_road(now_idx,i);
                    now_idx++;
                }
                
            }
            else if(robot[i].goods == 0 && d1[i][robot[i].x][robot[i].y] == 0){
                log<<3;
                printf("get %d\n",i);
                robot[i].target_id = robot_get_target_berth();
                
            }
            else if(robot[i].goods  == 1 ){
                log<<4;
                for(int j = 0; j < 4; j ++ ){
                    int xx = robot[i].x + dx[j], yy = robot[i].y + dy[j];
                    if(d2[robot[i].target_id][xx][yy] < d2[robot[i].target_id][robot[i].x][robot[i].y]){
                        int st = isempty(i,xx,yy);
                        if(st == robot_num + 1){
                            //printf("move %d %d\n",i,j);
                            robo_move[i][0] = j;
                            robot[i].x = xx, robot[i].y = yy;
                            robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
                            break;
                        }
                        else if(st == robot_num + 2){
                            continue;
                        }
                        else if(st>=0 && st< robot_num){
                            // int  x = robot[i].x;
                            // int y =  robot[i].y;
                            // for(int k = 0; k< 4; k++)
                            // {
                            //     int _x = robot[i].x + dx[j], _y = robot[i].y + dy[j];
                            //     if(d2[robot[i].target_id][xx][yy] < d2[robot[i].target_id][robot[i].x][robot[i].y])
                            // }
                            if(robot[i].goods == robot[st].goods)
                            {
                                change(i,st,0);
                            }
                            else {
                                change(i,st,1);
                            }
                        }
                    }
                }
                
            }
            else if(robot[i].goods == 0){
                log<<5 <<" "<<d1[i][robot[i].x][robot[i].y];
                for(int j = 0; j < 4; j ++ ){
                    int xx = robot[i].x + dx[j], yy = robot[i].y + dy[j];
                    if(d1[i][xx][yy] < d1[i][robot[i].x][robot[i].y] ){
                        int st = isempty(i,xx,yy);
                        if(st == robot_num + 1)
                        {
                            log<< " "<< j;
                            //printf("move %d %d\n",i,j);
                            robo_move[i][0] = j;
                            robot[i].x = xx, robot[i].y = yy;
                            robot_pos[i] = { robot[i].x, robot[i].y, xx, yy };
                            break;
                        }
                        else if( st == robot_num + 2){
                            continue;
                        }
                        else if(st < robot_num){
                            if(robot[i].goods == robot[st].goods)
                            {
                                change(i,st,0);
                            }
                            else {
                                change(i,st,1);
                            }
                        }
                    }
                }
            }
            log<<endl;
        }
        for(int i = 0; i < robot_num;i++)
        {
            if(robo_move[i][0]<=3 && robo_move[i][0]>=0)printf("move %d %d\n",i,robo_move[i][0]);
        }
        for(int i = 0; i < boat_num; i ++){
            log <<"船-"<<boat[i].status;
            if(boat[i].status == 0 || boat[i].status == 2)
            {
                log<<endl;
                continue;
            }
            else if(boat[i].status == 1 && boat[i].pos != -1){
                int cnt = min(berth[boat[i].pos].loading_speed , berth[boat[i].pos].val);
                boat[i].num += cnt;
                berth[boat[i].pos].val -= cnt;
                if(boat[i].num>1)
                {
                    printf("go %d\n",i);
                    boat[i].num = 0;
                    log<<" go";
                }
        
            }
            else if(boat[i].status == 1 && boat[i].pos == -1){
                boat_idx = boat_get_target();
                printf("ship %d %d\n", i, boat_idx);
                //boat_idx = (boat_idx + 1) % 10;
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
