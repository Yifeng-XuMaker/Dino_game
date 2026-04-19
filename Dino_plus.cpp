#include <iostream>
#include <string>
using namespace std;
int h=0;//儲存第一行障礙物
int obs1[8];
int obs2[8];
int ceils[9];
int p=0;//二段跳與一段跳區分

// dino 狀態
struct dino{
    int y;          // 恐龍底部位置
    int h;          // 恐龍高度 (1=蹲下, 2=站立)
    int jumplevl;   // 跳躍階段
    bool crouching; // 是否蹲下
    bool jumping;   // 是否跳躍
    bool UP;        // 是否在上層
    bool falling;   // 是否掉落
};

// 將輸入字串轉為整數陣列
void inputstr(string num,int a[],int len){
    for(int i=0;i<len;i++)a[i]=num[i]-'0';
}

// 畫面顯示
void fps(dino d){
    for(int i=7;i>=4;i--){
        for(int j=0;j<=8;j++){
            cout<<"|";
            if(j==0){
                if(i>=d.y&&i<d.y+d.h)cout<<" D";
                else cout<<"  ";
            }else{
                int k=obs2[j-1];
                int i2=i-4;
                if((k==1&&i2==0)||(k==2&&(i2==0||i2==1)))cout<<" C";
                else if((k==3&&i2==1)||(k==4&&i2==2))cout<<" P";
                else cout<<"  ";
            }
        }
        cout<<"|"<<endl;
    }
    for(int j=0;j<=8;j++){
        if(ceils[j]==1)
            if(j==8)cout<<"====";
            else cout<<"===";
        else
            if(j==8)cout<<"=  =";
            else cout<<"=  ";
    }
    cout<<endl;
    for(int i=3;i>=0;i--){
        for(int j=0;j<=8;j++){
            cout<<"|";
            if(j==0){
                if(i>=d.y&&i<d.y+d.h)cout<<" D";
                else if(h==1&&i==0)cout<<" C";
                else if(h==3&&i==1)cout<<" P";
                else if(h==4&&i==2)cout<<" P";
                else if((h==2&&i==0)||(h==2&&i==1))cout<<" C";
                else cout<<"  ";
            }else{
                int k=obs1[j-1];
                if((k==1&&i==0)||(k==2&&(i==0||i==1)))cout<<" C";
                else if((k==3&&i==1)||(k==4&&i==2))cout<<" P";
                else cout<<"  ";
            }
        }
        cout<<"|"<<endl;
    }
    cout<<endl;
}

// 狀態更新
void condition(dino &d,int c){
    if(d.y==4&&ceils[1]==0)d.falling=true;
    if(d.falling){//掉落
        d.y--;
        if(d.y<=0){
            d.y=0;
            d.falling=false;
            d.UP=false;
            d.jumplevl=0;
            d.h=2;
        }
    }
    if(c==2){//蹲下判定
        if(d.UP==1){
            if(ceils[1]==1){
                d.y=4;
                d.h=1;
                d.crouching=true;
                d.jumping=false;
                d.jumplevl=0;
            }else{
                d.y=0;
                d.crouching=true;
                d.h=1;
            }
        }else{
            d.h=1;
            d.y=0;
            d.crouching=true;
            d.jumping=false;
            d.jumplevl=0;
        }
        return;
    }
    if(c==1){
        if(d.jumping==0&&d.falling==0){
            d.jumping=true;
            d.jumplevl=0;
            d.crouching=false;
            d.h=2;
        }else if(d.jumping==1&&d.UP==0&&d.y==2&&ceils[1]==0){
            d.UP=true;
            d.y=2;
            d.h=2;
            p=1;
        }
    }
    if(c==0&&d.crouching==1){
        d.crouching=false;
        d.h=2;
    }
    if(d.jumping==1){
        switch(d.jumplevl){
            case 0:
                d.y++;
                break;
            case 1:
                d.y++;
                break;
            case 2:
                if(p==0)d.y--;
                else d.y++;
                break;
            case 3:
                if(p==0){d.y--; d.jumping=false; d.jumplevl=0;}
                else d.y++;
                break;
            case 4:
                if(d.falling==1)d.jumping=false;
            break;
        }
        d.jumplevl++;
    }
    if(d.UP==1&&d.jumping==0&&ceils[1]==0)
        d.falling=true;
}

// 碰撞判定
bool hit(dino d){
    if(d.UP==0){
        int k=obs1[0];
        bool con1=(d.y<=0&&d.y+d.h>0);//AND-gate的概念
        bool con2=(d.y<=1&&d.y+d.h>1);
        bool con3=(d.y<=2&&d.y+d.h>2);
        if(k==0)return false;
        if(k==1)return con1;
        if(k==2)return con1||con2;
        if(k==3)return con2;
        if(k==4)return con3;
    }else{
        int k=obs2[0];
        bool con4=(d.y<=4&&d.y+d.h>4);
        bool con5=(d.y<=5&&d.y+d.h>5);
        bool con6=(d.y<=6&&d.y+d.h>6);
        if(k==0)return false;
        if(k==1)return con4;
        if(k==2)return con4||con5;
        if(k==3)return con5;
        if(k==4)return con6;
    }
    return false;
}

// 更新障礙與天花板
void changefps(int next1,int next2,int nextceil){
    h=obs1[0];
    for(int i=0;i<7;i++){
        obs1[i]=obs1[i+1];
        obs2[i]=obs2[i+1];
    }
    for(int i=0;i<8;i++)ceils[i]=ceils[i+1];
    obs1[7]=next1;
    obs2[7]=next2;
    ceils[8]=nextceil;
}

// 主程式
int main(){
    cout<<"======Welcome to the Dinosaur Game======"<<endl;
    cout<<"Please enter the first layer initial obstacle setup of the Dinosaur Game. Enter -1 to quit: ";
    string s1;
    cin>>s1;
    if(s1=="-1"){
        cout<<"The game ends when it just started"<<endl;
        cout<<"======Thanks for playing Dinosaur Game, see you next time======"<<endl;
        return 0;
    }
    inputstr(s1,obs1,8);
    cout<<"Please enter the second layer initial obstacle setup of the Dinosaur Game: ";
    string s2;
    cin>>s2;
    inputstr(s2,obs2,8);
    cout<<"Please enter the initial distribution of gap in the ceiling, 0 for gap, 1 for complete ceiling: ";
    string s3;
    cin>>s3;
    inputstr(s3,ceils,9);
    dino d={0,2,0,false,false,false,false};
    while(true){
        fps(d);
        cout<<"Please enter a command to control the dinosaur, 0 for no action, 1=jump, 2=crouch: ";
        int c;
        cin>>c;
        while(true){
            if(c==0||c==2)break;
            if(c==1){
                if((d.jumping&&d.y!=2)||d.falling){
                    cout<<"The dinosaur cannot jump again, please enter another command: ";
                    cin>>c;
                    continue;
                }
                break;
            }
            cout<<"The dinosaur doesn't understand this command, try another one: ";
            cin>>c;
        }
        condition(d,c);
        cout<<"Please enter the next round's first layer obstacle setup of the Dinosaur Game. Enter -1 to quit: ";
        string ns1;
        cin>>ns1;
        if(ns1=="-1"){
            cout<<"Congratulations, you WIN the game!!!"<<endl;
            cout<<"======Thanks for playing Dinosaur Game, see you next time======"<<endl;
            break;
        }
        cout<<"Please enter the next round's second layer obstacle setup of the Dinosaur Game: ";
        string ns2;
        cin>>ns2;
        cout<<"Please enter the next round's ceiling or gap status: ";
        string ns3;
        cin>>ns3;
        int n1=ns1[0]-'0';
        int n2=ns2[0]-'0';
        int nc=ns3[0]-'0';
        if(hit(d)){
            cout<<"What a pity, it was close :("<<endl;
            cout<<"======Thanks for playing Dinosaur Game, see you next time======"<<endl;
            break;
        }
        changefps(n1,n2,nc);
    }
    return 0;
}
