#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>
using namespace std;
const int MaxN = 22;
const int MaxT = 6000;
//fstream in("in.txt");
int M,N,K,T;
/*每个司令部一开始都有M个生命元( 1 <= M <= 100000)
两个司令部之间一共有N个城市( 1 <= N <= 20 )
lion每前进一步，忠诚度就降低K。(0<=K<=100)
要求输出从0时0分开始，到时间T为止(包括T) 的所有事件。T以分钟为单位，0 <= T <= 6000*/
int WHPElement;
int EHPElement;
int RCreate;//红方制造武士的顺序指针
int BCreate;//蓝方制造武士的顺序指针
int RedWID;
int BlueWID;
int Hours;
int Minutes;
void Time(){//时间输出
    printf("%03d:%02d ",Hours,Minutes);
    return;
}
class Weapon{//武器
public:
    int ID;
    int Number;
    int ATK;
    Weapon(int id,int n,int atk):ID(id),Number(n),ATK(atk){}
    virtual void Effect(){return;}//武器效果
};
class sword:public Weapon{//剑
public:
    sword(int atk):Weapon(0,1,2/10*atk){//20%ATK,Number1
    }
};
class bomb:public Weapon{//炸弹
public:
    bomb(int atk):Weapon(1,1,4/10*atk){//40%ATK,Number1
    }
};
class arrow:public Weapon{//箭矢
public:
    arrow(int atk):Weapon(2,2,3/10*atk){//30%ATK,Number2
    }
};
bool comp0(const Weapon* a){//是否是剑
    return a->ID == 0;
}
bool comp1(const Weapon* a){//是否是炸弹
    return a->ID == 1;
}
bool comp2(const Weapon* a){//是否是箭矢
    return a->ID == 2;
}
bool comp3(const Weapon* a,const Weapon* b){//武器排序比较函数，箭少的在前，战斗使用
    if(a->ID < b->ID)
        return true;
    if(a->ID == b->ID && a->ID == 2){//多支arrow
        if(a->Number<b->Number)
            return true;
    }
    return false;
}
bool comp4(const Weapon* a,const Weapon* b){//武器排序比较函数，箭多的在前,缴获和抢夺使用
    if(a->ID < b->ID)
        return true;
    if(a->ID == b->ID && a->ID == 2){//多支arrow
        if(a->Number>b->Number)
            return true;
    }
    return false;
}
class Warrior{//武士
public:
    int ID;//编号
    int HP;//血量
    int type;//类型，派生类判断
    int ATK;//实体攻击力
    vector<Weapon*>::iterator W;//战斗用武器指针
    vector<Weapon*> Take;//武器数组
    Warrior(int id,int hp,int t,int atk):ID(id),HP(hp),type(t),ATK(atk){}
    void WeaponSort(int n){//武器排序
        if(!Take.empty()){//STL算法Sort排序，自定义一个比较函数
            if(n == 3)sort(Take.begin(),Take.end(),comp3);
            if(n == 4)sort(Take.begin(),Take.end(),comp4);
        }
        return;
    }
    void report(){//报告武器情况
        cout<<ID<<" has ";
        cout<<count_if(Take.begin(),Take.end(),comp0)<<" sword ";
        cout<<count_if(Take.begin(),Take.end(),comp1)<<" bomb ";
        cout<<count_if(Take.begin(),Take.end(),comp2)<<" arrow ";
        cout<<"and "<<HP<<" elements"<<endl;
        return;
    }
    void Attack(Warrior* Enemy){
        Weapon* U = *W;//用一件武器
        switch(U->ID){
            case 0://用剑
                U->ATK = ATK*2/10;
                break;
            case 1://用炸弹
                U->ATK = ATK*4/10;
                if(type!=2)//不是niniia
                    HP -= U->ATK/2;//炸弹对自身造成一半伤害
                U->Number--;
                break;
            case 2://用箭矢
                U->ATK = ATK*3/10;
                U->Number--;
                break;
        }
        Enemy->HP -= U->ATK;
        if(!U->Number){//使用次数没了
            vector<Weapon*>::iterator D = W;
            //W++;
            Take.erase(D);
        }
        else
            W++;
        if(W == Take.end())//一轮武器用完重置;
            W = Take.begin();
        return;
    }
    void Seize(Warrior* Aim){//缴获操作
        Aim->WeaponSort(4);//目标武器接受排序
        while(Take.size()<10&&!Aim->Take.empty()){//抢夺过程
            Take.push_back(*Aim->Take.begin());//抢入复制
            Aim->Take.erase(Aim->Take.begin());//目标删除
        }
        return;
    }
    virtual void loyal(){return;}
    virtual bool Escape(){return false;}
    virtual void Move(){return;}
    virtual void Rob(Warrior* Aim){return;}
    virtual void name(){return;};
    virtual ~Warrior(){
        for(int i = 0;i<Take.size();i++)//手工释放内存
            delete Take[i];
        //delete &Take;
    }
};
class dragon:public Warrior{
public:
    static int OriginalHP;//初始生命值
    static int ATK;//固定攻击力
    dragon(int id):Warrior(id,OriginalHP,1,ATK){
        //获得武器
        switch(id%3){
            case 0:
                Take.push_back(new sword(ATK));
                break;
            case 1:
                Take.push_back(new bomb(ATK));
                break;
            case 2:
                Take.push_back(new arrow(ATK));
                break;
        }
    }
    void name(){
        cout<<"dragon";
        return;
    }
};
int dragon::OriginalHP = 0;
int dragon::ATK = 0;
class ninjia:public Warrior{
public:
    static int OriginalHP;//初始生命值
    static int ATK;//固定攻击力
    ninjia(int id):Warrior(id,OriginalHP,2,ATK){
        //获得武器
        switch(id%3){
            case 0:
                Take.push_back(new sword(ATK));
                Take.push_back(new bomb(ATK));
                break;
            case 1:
                Take.push_back(new bomb(ATK));
                Take.push_back(new arrow(ATK));
                break;
            case 2:
                Take.push_back(new arrow(ATK));
                Take.push_back(new sword(ATK));
                break;
        }
    }
    void name(){
        cout<<"ninja";
        return;
    }
    //炸弹自己不受伤
};
int ninjia::OriginalHP = 0;
int ninjia::ATK = 0;
class iceman:public Warrior{
public:
    static int OriginalHP;//初始生命值
    static int ATK;//固定攻击力
    iceman(int id):Warrior(id,OriginalHP,3,ATK){
        //获得武器
        switch(id%3){
            case 0:
                Take.push_back(new sword(ATK));
                break;
            case 1:
                Take.push_back(new bomb(ATK));
                break;
            case 2:
                Take.push_back(new arrow(ATK));
                break;
        }
    }
    void Move(){//血量减少虚函数
        HP -= HP/10;
        return;
    }
    void name(){
        cout<<"iceman";
        return;
    }
};
int iceman::OriginalHP = 0;
int iceman::ATK = 0;
class lion:public Warrior{
    int loyalty;//忠诚度
public:
    static int OriginalHP;//初始生命值
    static int ATK;//固定攻击力
    lion(int id,int k):Warrior(id,OriginalHP,4,ATK),loyalty(k){
        //获得武器
        switch(id%3){
            case 0:
                Take.push_back(new sword(ATK));
                break;
            case 1:
                Take.push_back(new bomb(ATK));
                break;
            case 2:
                Take.push_back(new arrow(ATK));
                break;
        }
    }
    void Move(){//忠诚度减少虚函数
        loyalty -= K;
        return;
    }
    void loyal(){
        cout<<loyalty;
        return;
    }
    bool Escape(){//逃跑
        if(loyalty <= 0)
            return true;
        return false;
    }
    void name(){
        cout<<"lion";
        return;
    }
};
int lion::OriginalHP = 0;
int lion::ATK = 0;
class wolf:public Warrior{
public:
    static int OriginalHP;//初始生命值
    static int ATK;//固定攻击力
    wolf(int id):Warrior(id,OriginalHP,5,ATK){}
    void Rob(Warrior* Aim){//抢夺武器
        Aim->WeaponSort(4);//目标武器接受排序
        int RType = Aim->Take[0]->ID;//记录抢夺种类
        int number = 0;//记录抢夺个数
        while(Take.size()<10&&!Aim->Take.empty()&&RType == Aim->Take[0]->ID){//抢夺过程
            Take.push_back(*Aim->Take.begin());//抢入复制
            Aim->Take.erase(Aim->Take.begin());//目标删除
            number++;
        }
        //报告抢夺结果
        cout<<"took "<<number<<" ";
        switch(RType){
            case 0:
                cout<<"sword";
                break;
            case 1:
                cout<<"bomb";
                break;
            case 2:
                cout<<"arrow";
                break;
        }
        return;
    }
    void name(){
        cout<<"wolf";
        return;
    }
};
int wolf::OriginalHP = 0;
int wolf::ATK = 0;
class City{//城市
public:
    int ID;
    Warrior* Blue;//蓝方武士
    Warrior* Red;//红方武士
    //状态记录变量（偷个懒，应该使用结构体封装）
    int turn;//回合数
    int ROHP;
    int BOHP;
    int RTN;
    int BTN;
    City(int n):ID(n),Blue(NULL),Red(NULL){}
    void Create(int n){//如果司令部中的生命元不足以制造某本该造的武士，那就从此停止制造武士。
        if(n == 0){//作为红方制造一个武士
            switch(RCreate){
                case 0://iceman
                    if(WHPElement>=iceman::OriginalHP){
                       Red = new iceman(RedWID);
                       WHPElement -= iceman::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       cout<<"red ";Red->name();cout<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
                case 1://lion
                    if(WHPElement>=lion::OriginalHP){
                       WHPElement -= lion::OriginalHP;
                       Red = new lion(RedWID,WHPElement);
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       cout<<"red ";Red->name();cout<<" "<<Red->ID<<" born"<<endl;
                       cout<<"Its loyalty is ";Red->loyal();cout<<endl;
                    }
                    break;
                case 2://wolf
                    if(WHPElement>=wolf::OriginalHP){
                       Red = new wolf(RedWID);
                       WHPElement -= wolf::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       cout<<"red ";Red->name();cout<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
                case 3://ninjia
                    if(WHPElement>=ninjia::OriginalHP){
                       Red = new ninjia(RedWID);
                       WHPElement -= ninjia::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       cout<<"red ";Red->name();cout<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
                case 4://dragon
                    if(WHPElement>=dragon::OriginalHP){
                       Red = new dragon(RedWID);
                       WHPElement -= dragon::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       cout<<"red ";Red->name();cout<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
            }
        }
        else if(n == 1){//作为蓝方制造一个武士
            switch(BCreate){
                case 0://lion
                    if(EHPElement>=lion::OriginalHP){
                       EHPElement -= lion::OriginalHP;
                       Blue = new lion(BlueWID,EHPElement);
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       cout<<"blue ";Blue->name();cout<<" "<<Blue->ID<<" born"<<endl;
                       cout<<"Its loyalty is ";Blue->loyal();cout<<endl;
                    }

                    break;
                case 1://dragon
                    if(EHPElement>=dragon::OriginalHP){
                       Blue = new dragon(BlueWID);
                       EHPElement -= dragon::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       cout<<"blue ";Blue->name();cout<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
                case 2://ninjia
                    if(EHPElement>ninjia::OriginalHP){
                       Blue = new ninjia(BlueWID);
                       EHPElement -= ninjia::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       cout<<"blue ";Blue->name();cout<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
                case 3://iceman
                    if(EHPElement>=iceman::OriginalHP){
                       Blue = new iceman(BlueWID);
                       EHPElement -= iceman::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       cout<<"blue ";Blue->name();cout<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
                case 4://wolf
                    if(EHPElement>=wolf::OriginalHP){
                       Blue = new wolf(BlueWID);
                       EHPElement -= wolf::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       cout<<"blue ";Blue->name();cout<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
            }
        }
        return;
    }
    void Escape(){//lion逃跑
        if(Red!=NULL){
            if(Red->Escape()){
                Time();cout<<"blue ";Red->name();
                cout<<" "<<Red->ID<<" ran away"<<endl;
                delete Red;
                Red = NULL;
            }
        }
        if(Blue!=NULL){
            if(Blue->Escape()){
                Time();cout<<"blue ";Blue->name();
                cout<<" "<<Blue->ID<<" ran away"<<endl;
                delete Blue;
                Blue = NULL;
            }
        }
        return;
    }
    void Rob(){//有狼的话抢劫
        if(Blue!=NULL&&Red!=NULL){
            if(Blue->type == 5&&Red->type != 5&&!Red->Take.empty()){//蓝抢红
                Time();
                cout<<"blue wolf "<<Blue->ID<<" ";
                Blue->Rob(Red);
                cout<<" from red ";Red->name();
                cout<<" "<<Red->ID<<" in city "<<ID<<endl;
            }
            else if(Blue->type != 5&&Red->type == 5&&!Blue->Take.empty()){//红抢蓝
                Time();
                cout<<"red wolf "<<Red->ID<<" ";
                Red->Rob(Blue);
                cout<<" from blue ";Blue->name();
                cout<<" "<<Blue->ID<<" in city "<<ID<<endl;
            }
        }
        return;
    }
    void Battle(){//城市中执行战斗
        if(Red!=NULL&&Blue!=NULL){//发生战斗
            //平局判断
            if(Red->Take.empty()&&Blue->Take.empty()){
                BattleResult(1);
                return;
            }
            //武器排序
            Red->WeaponSort(3);Blue->WeaponSort(3);
            //决定先后手,设置战斗用变量
            Warrior* first = NULL;
            Warrior* second = NULL;
            if(ID%2){//红1
                first = Red;
                second = Blue;
            }
            else{//蓝0
                first = Blue;
                second = Red;
            }
            first->W = first->Take.begin(); //武器指针归零
            second->W = second->Take.begin(); //武器指针归零
            turn = 1;
            ROHP = Red->HP;
            BOHP = Blue->HP;
            RTN = Red->Take.size();
            BTN = Blue->Take.size();
            while(!BattleJudge()){//while()先后手交替使用
                //战斗过程，使用一件武器，扣血核心机制和特殊技能处理
                if(!first->Take.empty())
                    first->Attack(second);
                //交换行动权
                Warrior* temp = first;
                first = second;
                second = temp;
                turn++;
            }
            BattleResult(BattleJudge());
        }
        return;
    }
    int BattleJudge(){//胜负平局判断,0表示还没结束
        if(Red->HP<=0&&Blue->HP<=0)//双方都死了，平局
            return 1;
        if(Red->HP<=0)//红方死了，蓝方胜
            return 3;
        if(Blue->HP<=0)//蓝方死了，红方胜
            return 2;
        if(Red->Take.empty()&&Blue->Take.empty())//双方都活着且武器都没了，平局
            return 1;
        if(turn%10 == 0){//双方状态不发生变化，平局
            if(Red->HP == ROHP &&
               Blue->HP == BOHP &&
               Red->Take.size() == RTN &&
               Blue->Take.size() == BTN)
                return 1;
            else{//记录数据
                ROHP = Red->HP;
                BOHP = Blue->HP;
                RTN = Red->Take.size();
                BTN = Blue->Take.size();
            }
        }
        return 0;//没打完
    }
    void BattleResult(int n){//1平局，2红胜，3蓝胜
        //胜负结果输出
        Time();
        switch(n){
            case 1:
                if(Red->HP>0){
                    cout<<"both red ";Red->name();
                    cout<<" "<<Red->ID<<" and blue ";Blue->name();
                    cout<<" "<<Blue->ID<<" were alive in city "<<ID<<endl;
                }
                else{
                    cout<<"both red ";Red->name();
                    cout<<" "<<Red->ID<<" and blue ";Blue->name();
                    cout<<" "<<Blue->ID<<" died in city "<<ID<<endl;
                }
                break;
            case 2:
                cout<<"red ";Red->name();
                cout<<" "<<Red->ID<<" killed blue ";Blue->name();
                cout<<" "<<Blue->ID<<" in city "<<ID<<" remaining "<<Red->HP<<" elements"<<endl;
                Red->Seize(Blue);//缴获处理
                break;
            case 3:
                cout<<"blue ";Blue->name();
                cout<<" "<<Blue->ID<<" killed red ";Red->name();
                cout<<" "<<Red->ID<<" in city "<<ID<<" remaining "<<Blue->HP<<" elements"<<endl;
                Blue->Seize(Red);//缴获处理
                break;
        }
        //dragon欢呼
        if(Red->HP>0&&Red->type == 1){
            Time();
            cout<<"red ";Red->name();
            cout<<" "<<Red->ID<<" yelled in city "<<ID<<endl;
        }
        if(Blue->HP>0&&Blue->type == 1){
            Time();
            cout<<"blue ";Blue->name();
            cout<<" "<<Blue->ID<<" yelled in city "<<ID<<endl;
        }
        return;
    }
    void death(){//死亡处理
        if(Red!=NULL&&Red->HP<=0){
            delete Red;
            Red = NULL;
        }
        if(Blue!=NULL&&Blue->HP<=0){
            delete Blue;
            Blue = NULL;
        }
        return;
    }
    void Report(){//有武士的话汇报
        if(Red!=NULL){
            Time();
            cout<<"red ";
            Red->name();
            cout<<" ";
            Red->report();
        }
        if(Blue!=NULL){
            Time();
            cout<<"blue ";
            Blue->name();
            cout<<" ";
            Blue->report();
        }
        return;
    }
    ~City(){
        delete Red;
        delete Blue;
        Red = Blue = NULL;
    }
};
City* BG[MaxN];//城市数组
class Events{//事件时钟
public:
    City* Red;//红方司令部
    City* Blue;//蓝方司令部
    Events(){}
    //执行各事件
    void Start(int n){//数据输入
        cin>>M>>N>>K>>T;
        BCreate = RCreate = 0;//依照次序从头开始制造武士
        RedWID = BlueWID = 1;//编号
        Hours = Minutes = 0;//时间从零开始
        WHPElement = EHPElement = M;
        cin>>dragon::OriginalHP>>ninjia::OriginalHP>>iceman::OriginalHP>>lion::OriginalHP>>wolf::OriginalHP;
        cin>>dragon::ATK>>ninjia::ATK>>iceman::ATK>>lion::ATK>>wolf::ATK;
        for(int i=0;i<N+2;i++)//初始化城市数组
            BG[i] = new City(i);
        Red = BG[0];Blue = BG[N+1];
        Process();
        return;
    }
    bool TimeJudge(){//是否超时
        if(Hours*60+Minutes>T)
            return true;
        return false;
    }
    void Process(){//事件过程
        while(true){//时间到或者任意司令部被占领就退出循环
            Minutes = 0;if(TimeJudge())break;//在每个整点，即每个小时的第0分， 双方的司令部中各有一个武士降生。
            Red->Create(0);//红方司令部按照iceman、lion、wolf、ninja、dragon 的顺序制造武士。
            Blue->Create(1);//蓝方司令部按照lion、dragon、ninja、iceman、wolf 的顺序制造武士。
            Minutes = 5;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//在每个小时的第5分，该逃跑的lion就在这一时刻逃跑了。
                BG[i]->Escape();
            Minutes = 10;if(TimeJudge())break;
            Warrior* R = NULL;//设置一个红方缓存来表示西边的城市有人需要移动
            for(int i=0;i<N+2;i++){////在每个小时的第10分：所有的武士朝敌人司令部方向前进一步。
                if(i==0){//蓝方去红方司令部
                    if(BG[i]->Red!=NULL){//红方缓存
                        R = BG[i]->Red;
                        BG[i]->Red = NULL;
                    }
                    if(BG[i+1]->Blue!=NULL){
                        BG[i]->Blue = BG[i+1]->Blue;
                        BG[i+1]->Blue = NULL;
                        BG[i]->Blue->Move();
                        Time();
                        cout<<"blue ";BG[i]->Blue->name();cout<<" "<<BG[i]->Blue->ID<<" reached red headquarter ";
                        cout<<"with "<<BG[i]->Blue->HP<<" elements and force "<<BG[i]->Blue->ATK<<endl;
                        Time();cout<<"red headquarter was taken"<<endl;
                    }
                    continue;
                }
                if(i==N+1){//红方去蓝方司令部
                    if(R!=NULL){
                        BG[i]->Red = R;
                        R = NULL;
                        BG[i]->Red->Move();
                        Time();
                        cout<<"red ";BG[i]->Red->name();cout<<" "<<BG[i]->Red->ID<<" reached blue headquarter ";
                        cout<<"with "<<BG[i]->Red->HP<<" elements and force "<<BG[i]->Red->ATK<<endl;
                        Time();cout<<"blue headquarter was taken"<<endl;
                    }
                    continue;
                }
                //以i城为目的地,红方从西,交换R和BG[i]->Red的指向
                Warrior* temp = R;
                R = BG[i]->Red;
                BG[i]->Red = temp;
                if(BG[i]->Red != NULL){
                    BG[i]->Red->Move();
                    Time();
                    cout<<"red ";BG[i]->Red->name();cout<<" "<<BG[i]->Red->ID<<" marched to city "<<i;
                    cout<<" with "<<BG[i]->Red->HP<<" elements and force "<<BG[i]->Red->ATK<<endl;
                }
                if(BG[i+1]->Blue!=NULL){//蓝方从东
                    BG[i]->Blue = BG[i+1]->Blue;
                    BG[i+1]->Blue = NULL;
                    BG[i]->Blue->Move();
                    Time();
                    cout<<"blue ";BG[i]->Blue->name();cout<<" "<<BG[i]->Blue->ID<<" marched to city "<<i;
                    cout<<" with "<<BG[i]->Blue->HP<<" elements and force "<<BG[i]->Blue->ATK<<endl;
                }
            }
            if(Red->Blue!=NULL||Blue->Red!=NULL)break;//此处是占领&结束判断
            Minutes = 35;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//在每个小时的第35分：在有wolf及其敌人的城市，wolf要抢夺对方的武器。
                BG[i]->Rob();
            Minutes = 40;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//在每个小时的第40分：在有两个武士的城市，会发生战斗。
                BG[i]->Battle();
            for(int i=0;i<N+2;i++)//死亡处理
                BG[i]->death();
            Minutes = 50;if(TimeJudge())break;
            //在每个小时的第50分，司令部报告它拥有的生命元数量。
            Time();cout<<WHPElement<<" elements in red headquarter"<<endl;
            Time();cout<<EHPElement<<" elements in blue headquarter"<<endl;
            Minutes = 55;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//在每个小时的第55分，每个武士报告其拥有的武器情况。
                BG[i]->Report();//每个城市的武士报告
            Hours++;
        }
        return;
    }
    ~Events(){//删除一切，清空内存,重置变量
        for(int i=0;i<N+2;i++){
            delete BG[i];
        }
        Red = NULL;Blue = NULL;
    }
};
int main(){//主进程
    int n;cin>>n;
    for(int i=1;i<=n;i++){//循环用例
        cout<<"Case "<<i<<":"<<endl;
        Events* events = new Events();//建立本次程序的事件
        events->Start(n);//输入数据
        delete events;//结束后清理一切
    }
    return 0;
}
