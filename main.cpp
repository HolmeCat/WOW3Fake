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
fstream in("in.txt");
fstream re("out.txt");
int M,N,K,T;
/*ÿ��˾�һ��ʼ����M������Ԫ( 1 <= M <= 100000)
����˾�֮��һ����N������( 1 <= N <= 20 )
lionÿǰ��һ�����ҳ϶Ⱦͽ���K��(0<=K<=100)
Ҫ�������0ʱ0�ֿ�ʼ����ʱ��TΪֹ(����T) �������¼���T�Է���Ϊ��λ��0 <= T <= 6000*/
int WHPElement;
int EHPElement;
int RCreate;//�췽������ʿ��˳��ָ��
int BCreate;//����������ʿ��˳��ָ��
int RedWID;
int BlueWID;
int Hours;
int Minutes;
void Time(){//ʱ�����
    //printf("%03d:%02d ",Hours,Minutes);
    re<<setw(3)<<setfill('0')<<Hours;
    re<<":";
    re<<setw(2)<<setfill('0')<<Minutes;
    re<<" ";
    return;
}
class Weapon{//����
public:
    int ID;
    int Number;
    int ATK;
    Weapon(int id,int n,int atk):ID(id),Number(n),ATK(atk){}
    virtual void Effect(){return;}//����Ч��
};
class sword:public Weapon{//��
public:
    sword(int atk):Weapon(0,1,2/10*atk){//20%ATK,Number1
    }
};
class bomb:public Weapon{//ը��
public:
    bomb(int atk):Weapon(1,1,4/10*atk){//40%ATK,Number1
    }
};
class arrow:public Weapon{//��ʸ
public:
    arrow(int atk):Weapon(2,2,3/10*atk){//30%ATK,Number2
    }
};
bool comp0(const Weapon* a){//�Ƿ��ǽ�
    return a->ID == 0;
}
bool comp1(const Weapon* a){//�Ƿ���ը��
    return a->ID == 1;
}
bool comp2(const Weapon* a){//�Ƿ��Ǽ�ʸ
    return a->ID == 2;
}
bool comp3(const Weapon* a,const Weapon* b){//��������ȽϺ��������ٵ���ǰ��ս��ʹ��
    if(a->ID < b->ID)
        return true;
    if(a->ID == b->ID && a->ID == 2){//��֧arrow
        if(a->Number<b->Number)
            return true;
    }
    return false;
}
bool comp4(const Weapon* a,const Weapon* b){//��������ȽϺ������������ǰ,�ɻ������ʹ��
    if(a->ID < b->ID)
        return true;
    if(a->ID == b->ID && a->ID == 2){//��֧arrow
        if(a->Number>b->Number)
            return true;
    }
    return false;
}
class Warrior{//��ʿ
public:
    int ID;//���
    int HP;//Ѫ��
    int type;//���ͣ��������ж�
    int ATK;//ʵ�幥����
    vector<Weapon*>::iterator W;//ս��������ָ��
    vector<Weapon*> Take;//��������
    Warrior(int id,int hp,int t,int atk):ID(id),HP(hp),type(t),ATK(atk){}
    void WeaponSort(int n){//��������
        if(!Take.empty()){//STL�㷨Sort�����Զ���һ���ȽϺ���
            if(n == 3)sort(Take.begin(),Take.end(),comp3);
            if(n == 4)sort(Take.begin(),Take.end(),comp4);
        }
        return;
    }
    void report(){//�����������
        re<<ID<<" has ";
        re<<count_if(Take.begin(),Take.end(),comp0)<<" sword ";
        re<<count_if(Take.begin(),Take.end(),comp1)<<" bomb ";
        re<<count_if(Take.begin(),Take.end(),comp2)<<" arrow ";
        re<<"and "<<HP<<" elements"<<endl;
        return;
    }
    void Attack(Warrior* Enemy){
        Weapon* U = *W;//��һ������
        switch(U->ID){
            case 0://�ý�
                U->ATK = ATK*2/10;
                break;
            case 1://��ը��
                U->ATK = ATK*4/10;
                if(type!=2)//����niniia
                    HP -= U->ATK/2;//ը�����������һ���˺�
                U->Number--;
                break;
            case 2://�ü�ʸ
                U->ATK = ATK*3/10;
                U->Number--;
                break;
        }
        Enemy->HP -= U->ATK;
        if(!U->Number){//ʹ�ô���û��
            vector<Weapon*>::iterator D = W;
            //W++;
            Take.erase(D);
        }
        else
            W++;
        if(W == Take.end())//һ��������������;
            W = Take.begin();
        return;
    }
    void Seize(Warrior* Aim){//�ɻ����
        Aim->WeaponSort(4);//Ŀ��������������
        while(Take.size()<10&&!Aim->Take.empty()){//�������
            Take.push_back(*Aim->Take.begin());//���븴��
            Aim->Take.erase(Aim->Take.begin());//Ŀ��ɾ��
        }
        return;
    }
    virtual void loyal(){return;}
    virtual bool Escape(){return false;}
    virtual void Move(){return;}
    virtual void Rob(Warrior* Aim){return;}
    virtual void name(){return;};
    virtual ~Warrior(){
        for(int i = 0;i<Take.size();i++)//�ֹ��ͷ��ڴ�
            delete Take[i];
        //delete &Take;
    }
};
class dragon:public Warrior{
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    dragon(int id):Warrior(id,OriginalHP,1,ATK){
        //�������
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
        re<<"dragon";
        return;
    }
};
int dragon::OriginalHP = 0;
int dragon::ATK = 0;
class ninjia:public Warrior{
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    ninjia(int id):Warrior(id,OriginalHP,2,ATK){
        //�������
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
        re<<"ninja";
        return;
    }
    //ը���Լ�������
};
int ninjia::OriginalHP = 0;
int ninjia::ATK = 0;
class iceman:public Warrior{
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    iceman(int id):Warrior(id,OriginalHP,3,ATK){
        //�������
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
    void Move(){//Ѫ�������麯��
        HP -= HP/10;
        return;
    }
    void name(){
        re<<"iceman";
        return;
    }
};
int iceman::OriginalHP = 0;
int iceman::ATK = 0;
class lion:public Warrior{
    int loyalty;//�ҳ϶�
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    lion(int id,int k):Warrior(id,OriginalHP,4,ATK),loyalty(k){
        //�������
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
    void Move(){//�ҳ϶ȼ����麯��
        loyalty -= K;
        return;
    }
    void loyal(){
        re<<loyalty;
        return;
    }
    bool Escape(){//����
        if(loyalty <= 0)
            return true;
        return false;
    }
    void name(){
        re<<"lion";
        return;
    }
};
int lion::OriginalHP = 0;
int lion::ATK = 0;
class wolf:public Warrior{
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    wolf(int id):Warrior(id,OriginalHP,5,ATK){}
    void Rob(Warrior* Aim){//��������
        Aim->WeaponSort(4);//Ŀ��������������
        int RType = Aim->Take[0]->ID;//��¼��������
        int number = 0;//��¼�������
        while(Take.size()<10&&!Aim->Take.empty()&&RType == Aim->Take[0]->ID){//�������
            Take.push_back(*Aim->Take.begin());//���븴��
            Aim->Take.erase(Aim->Take.begin());//Ŀ��ɾ��
            number++;
        }
        //����������
        re<<"took "<<number<<" ";
        switch(RType){
            case 0:
                re<<"sword";
                break;
            case 1:
                re<<"bomb";
                break;
            case 2:
                re<<"arrow";
                break;
        }
        return;
    }
    void name(){
        re<<"wolf";
        return;
    }
};
int wolf::OriginalHP = 0;
int wolf::ATK = 0;
class City{//����
public:
    int ID;
    Warrior* Blue;//������ʿ
    Warrior* Red;//�췽��ʿ
    //״̬��¼������͵������Ӧ��ʹ�ýṹ���װ��
    int turn;//�غ���
    int ROHP;
    int BOHP;
    int RTN;
    int BTN;
    City(int n):ID(n),Blue(NULL),Red(NULL){}
    void Create(int n){//���˾��е�����Ԫ����������ĳ���������ʿ���ǾʹӴ�ֹͣ������ʿ��
        if(n == 0){//��Ϊ�췽����һ����ʿ
            switch(RCreate){
                case 0://iceman
                    if(WHPElement>=iceman::OriginalHP){
                       Red = new iceman(RedWID);
                       WHPElement -= iceman::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       re<<"red ";Red->name();re<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
                case 1://lion
                    if(WHPElement>=lion::OriginalHP){
                       WHPElement -= lion::OriginalHP;
                       Red = new lion(RedWID,WHPElement);
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       re<<"red ";Red->name();re<<" "<<Red->ID<<" born"<<endl;
                       re<<"Its loyalty is ";Red->loyal();re<<endl;
                    }
                    break;
                case 2://wolf
                    if(WHPElement>=wolf::OriginalHP){
                       Red = new wolf(RedWID);
                       WHPElement -= wolf::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       re<<"red ";Red->name();re<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
                case 3://ninjia
                    if(WHPElement>=ninjia::OriginalHP){
                       Red = new ninjia(RedWID);
                       WHPElement -= ninjia::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       re<<"red ";Red->name();re<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
                case 4://dragon
                    if(WHPElement>=dragon::OriginalHP){
                       Red = new dragon(RedWID);
                       WHPElement -= dragon::OriginalHP;
                       RedWID++;
                       RCreate = (RCreate+1)%5;
                       Time();
                       re<<"red ";Red->name();re<<" "<<Red->ID<<" born"<<endl;
                    }
                    break;
            }
        }
        else if(n == 1){//��Ϊ��������һ����ʿ
            switch(BCreate){
                case 0://lion
                    if(EHPElement>=lion::OriginalHP){
                       EHPElement -= lion::OriginalHP;
                       Blue = new lion(BlueWID,EHPElement);
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       re<<"blue ";Blue->name();re<<" "<<Blue->ID<<" born"<<endl;
                       re<<"Its loyalty is ";Blue->loyal();re<<endl;
                    }

                    break;
                case 1://dragon
                    if(EHPElement>=dragon::OriginalHP){
                       Blue = new dragon(BlueWID);
                       EHPElement -= dragon::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       re<<"blue ";Blue->name();re<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
                case 2://ninjia
                    if(EHPElement>ninjia::OriginalHP){
                       Blue = new ninjia(BlueWID);
                       EHPElement -= ninjia::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       re<<"blue ";Blue->name();re<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
                case 3://iceman
                    if(EHPElement>=iceman::OriginalHP){
                       Blue = new iceman(BlueWID);
                       EHPElement -= iceman::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       re<<"blue ";Blue->name();re<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
                case 4://wolf
                    if(EHPElement>=wolf::OriginalHP){
                       Blue = new wolf(BlueWID);
                       EHPElement -= wolf::OriginalHP;
                       BlueWID++;
                       BCreate = (BCreate+1)%5;
                       Time();
                       re<<"blue ";Blue->name();re<<" "<<Blue->ID<<" born"<<endl;
                    }
                    break;
            }
        }
        return;
    }
    void Escape(){//lion����
        if(Red!=NULL){
            if(Red->Escape()){
                Time();re<<"blue ";Red->name();
                re<<" "<<Red->ID<<" ran away"<<endl;
                delete Red;
                Red = NULL;
            }
        }
        if(Blue!=NULL){
            if(Blue->Escape()){
                Time();re<<"blue ";Blue->name();
                re<<" "<<Blue->ID<<" ran away"<<endl;
                delete Blue;
                Blue = NULL;
            }
        }
        return;
    }
    void Rob(){//���ǵĻ�����
        if(Blue!=NULL&&Red!=NULL){
            if(Blue->type == 5&&Red->type != 5&&!Red->Take.empty()){//������
                Time();
                re<<"blue wolf "<<Blue->ID<<" ";
                Blue->Rob(Red);
                re<<" from red ";Red->name();
                re<<" "<<Red->ID<<" in city "<<ID<<endl;
            }
            else if(Blue->type != 5&&Red->type == 5&&!Blue->Take.empty()){//������
                Time();
                re<<"red wolf "<<Red->ID<<" ";
                Red->Rob(Blue);
                re<<" from blue ";Blue->name();
                re<<" "<<Blue->ID<<" in city "<<ID<<endl;
            }
        }
        return;
    }
    void Battle(){//������ִ��ս��
        if(Red!=NULL&&Blue!=NULL){//����ս��
            //ƽ���ж�
            if(Red->Take.empty()&&Blue->Take.empty()){
                BattleResult(1);
                return;
            }
            //��������
            Red->WeaponSort(3);Blue->WeaponSort(3);
            //�����Ⱥ���,����ս���ñ���
            Warrior* first = NULL;
            Warrior* second = NULL;
            if(ID%2){//��1
                first = Red;
                second = Blue;
            }
            else{//��0
                first = Blue;
                second = Red;
            }
            first->W = first->Take.begin(); //����ָ�����
            second->W = second->Take.begin(); //����ָ�����
            turn = 1;
            ROHP = Red->HP;
            BOHP = Blue->HP;
            RTN = Red->Take.size();
            BTN = Blue->Take.size();
            while(!BattleJudge()){//while()�Ⱥ��ֽ���ʹ��
                //ս�����̣�ʹ��һ����������Ѫ���Ļ��ƺ����⼼�ܴ���
                if(!first->Take.empty())
                    first->Attack(second);
                //�����ж�Ȩ
                Warrior* temp = first;
                first = second;
                second = temp;
                turn++;
            }
            BattleResult(BattleJudge());
        }
        return;
    }
    int BattleJudge(){//ʤ��ƽ���ж�,0��ʾ��û����
        if(Red->HP<=0&&Blue->HP<=0)//˫�������ˣ�ƽ��
            return 1;
        if(Red->HP<=0)//�췽���ˣ�����ʤ
            return 3;
        if(Blue->HP<=0)//�������ˣ��췽ʤ
            return 2;
        if(Red->Take.empty()&&Blue->Take.empty())//˫����������������û�ˣ�ƽ��
            return 1;
        if(turn%10 == 0){//˫��״̬�������仯��ƽ��
            if(Red->HP == ROHP &&
               Blue->HP == BOHP &&
               Red->Take.size() == RTN &&
               Blue->Take.size() == BTN)
                return 1;
            else{//��¼����
                ROHP = Red->HP;
                BOHP = Blue->HP;
                RTN = Red->Take.size();
                BTN = Blue->Take.size();
            }
        }
        return 0;//û����
    }
    void BattleResult(int n){//1ƽ�֣�2��ʤ��3��ʤ
        //ʤ��������
        Time();
        switch(n){
            case 1:
                if(Red->HP>0){
                    re<<"both red ";Red->name();
                    re<<" "<<Red->ID<<" and blue ";Blue->name();
                    re<<" "<<Blue->ID<<" were alive in city "<<ID<<endl;
                }
                else{
                    re<<"both red ";Red->name();
                    re<<" "<<Red->ID<<" and blue ";Blue->name();
                    re<<" "<<Blue->ID<<" died in city "<<ID<<endl;
                }
                break;
            case 2:
                re<<"red ";Red->name();
                re<<" "<<Red->ID<<" killed blue ";Blue->name();
                re<<" "<<Blue->ID<<" in city "<<ID<<" remaining "<<Red->HP<<" elements"<<endl;
                Red->Seize(Blue);//�ɻ���
                break;
            case 3:
                re<<"blue ";Blue->name();
                re<<" "<<Blue->ID<<" killed red ";Red->name();
                re<<" "<<Red->ID<<" in city "<<ID<<" remaining "<<Blue->HP<<" elements"<<endl;
                Blue->Seize(Red);//�ɻ���
                break;
        }
        //dragon����
        if(Red->HP>0&&Red->type == 1){
            Time();
            re<<"red ";Red->name();
            re<<" "<<Red->ID<<" yelled in city "<<ID<<endl;
        }
        if(Blue->HP>0&&Blue->type == 1){
            Time();
            re<<"blue ";Blue->name();
            re<<" "<<Blue->ID<<" yelled in city "<<ID<<endl;
        }
        return;
    }
    void death(){//��������
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
    void Report(){//����ʿ�Ļ��㱨
        if(Red!=NULL){
            Time();
            re<<"red ";
            Red->name();
            re<<" ";
            Red->report();
        }
        if(Blue!=NULL){
            Time();
            re<<"blue ";
            Blue->name();
            re<<" ";
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
City* BG[MaxN];//��������
class Events{//�¼�ʱ��
public:
    City* Red;//�췽˾�
    City* Blue;//����˾�
    Events(){}
    //ִ�и��¼�
    void Start(int n){//��������
        in>>M>>N>>K>>T;
        BCreate = RCreate = 0;//���մ����ͷ��ʼ������ʿ
        RedWID = BlueWID = 1;//���
        Hours = Minutes = 0;//ʱ����㿪ʼ
        WHPElement = EHPElement = M;
        in>>dragon::OriginalHP>>ninjia::OriginalHP>>iceman::OriginalHP>>lion::OriginalHP>>wolf::OriginalHP;
        in>>dragon::ATK>>ninjia::ATK>>iceman::ATK>>lion::ATK>>wolf::ATK;
        for(int i=0;i<N+2;i++)//��ʼ����������
            BG[i] = new City(i);
        Red = BG[0];Blue = BG[N+1];
        Process();
        return;
    }
    bool TimeJudge(){//�Ƿ�ʱ
        if(Hours*60+Minutes>T)
            return true;
        return false;
    }
    void Process(){//�¼�����
        while(true){//ʱ�䵽��������˾���ռ����˳�ѭ��
            Minutes = 0;if(TimeJudge())break;//��ÿ�����㣬��ÿ��Сʱ�ĵ�0�֣� ˫����˾��и���һ����ʿ������
            Red->Create(0);//�췽˾�����iceman��lion��wolf��ninja��dragon ��˳��������ʿ��
            Blue->Create(1);//����˾�����lion��dragon��ninja��iceman��wolf ��˳��������ʿ��
            Minutes = 5;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�5�֣������ܵ�lion������һʱ�������ˡ�
                BG[i]->Escape();
            Minutes = 10;if(TimeJudge())break;
            Warrior* R = NULL;//����һ���췽��������ʾ���ߵĳ���������Ҫ�ƶ�
            for(int i=0;i<N+2;i++){////��ÿ��Сʱ�ĵ�10�֣����е���ʿ������˾�����ǰ��һ����
                if(i==0){//����ȥ�췽˾�
                    if(BG[i]->Red!=NULL){//�췽����
                        R = BG[i]->Red;
                        BG[i]->Red = NULL;
                    }
                    if(BG[i+1]->Blue!=NULL){
                        BG[i]->Blue = BG[i+1]->Blue;
                        BG[i+1]->Blue = NULL;
                        BG[i]->Blue->Move();
                        Time();
                        re<<"blue ";BG[i]->Blue->name();re<<" "<<BG[i]->Blue->ID<<" reached red headquarter ";
                        re<<"with "<<BG[i]->Blue->HP<<" elements and force "<<BG[i]->Blue->ATK<<endl;
                        Time();
                        re<<"red headquarter was taken"<<endl;
                    }
                    continue;
                }
                if(i==N+1){//�췽ȥ����˾�
                    if(R!=NULL){
                        BG[i]->Red = R;
                        R = NULL;
                        BG[i]->Red->Move();
                        Time();
                        re<<"red ";BG[i]->Red->name();re<<" "<<BG[i]->Red->ID<<" reached blue headquarter ";
                        re<<"with "<<BG[i]->Red->HP<<" elements and force "<<BG[i]->Red->ATK<<endl;
                        Time();
                        re<<"blue headquarter was taken"<<endl;
                    }
                    continue;
                }
                //��i��ΪĿ�ĵ�,�췽����,����R��BG[i]->Red��ָ��
                Warrior* temp = R;
                R = BG[i]->Red;
                BG[i]->Red = temp;
                if(BG[i]->Red != NULL){
                    BG[i]->Red->Move();
                    Time();
                    re<<"red ";BG[i]->Red->name();re<<" "<<BG[i]->Red->ID<<" marched to city "<<i;
                    re<<" with "<<BG[i]->Red->HP<<" elements and force "<<BG[i]->Red->ATK<<endl;
                }
                if(BG[i+1]->Blue!=NULL){//�����Ӷ�
                    BG[i]->Blue = BG[i+1]->Blue;
                    BG[i+1]->Blue = NULL;
                    BG[i]->Blue->Move();
                    Time();
                    re<<"blue ";BG[i]->Blue->name();re<<" "<<BG[i]->Blue->ID<<" marched to city "<<i;
                    re<<" with "<<BG[i]->Blue->HP<<" elements and force "<<BG[i]->Blue->ATK<<endl;
                }
            }
            if(Red->Blue!=NULL||Blue->Red!=NULL)break;//�˴���ռ��&�����ж�
            Minutes = 35;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�35�֣�����wolf������˵ĳ��У�wolfҪ����Է���������
                BG[i]->Rob();
            Minutes = 40;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�40�֣�����������ʿ�ĳ��У��ᷢ��ս����
                BG[i]->Battle();
            for(int i=0;i<N+2;i++)//��������
                BG[i]->death();
            Minutes = 50;if(TimeJudge())break;
            //��ÿ��Сʱ�ĵ�50�֣�˾�������ӵ�е�����Ԫ������
            Time();re<<WHPElement<<" elements in red headquarter"<<endl;
            Time();re<<EHPElement<<" elements in blue headquarter"<<endl;
            Minutes = 55;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�55�֣�ÿ����ʿ������ӵ�е����������
                BG[i]->Report();//ÿ�����е���ʿ����
            Hours++;
        }
        return;
    }
    ~Events(){//ɾ��һ�У�����ڴ�,���ñ���
        for(int i=0;i<N+2;i++){
            delete BG[i];
        }
        Red = NULL;Blue = NULL;
    }
};
int main(){//������
    int n;in>>n;
    for(int i=1;i<=n;i++){//ѭ������
        re<<"Case "<<i<<":"<<endl;
        Events* events = new Events();//�������γ�����¼�
        events->Start(n);//��������
        delete events;//����������һ��
    }
    return 0;
}




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
/*ÿ��˾�һ��ʼ����M������Ԫ( 1 <= M <= 100000)
����˾�֮��һ����N������( 1 <= N <= 20 )
lionÿǰ��һ�����ҳ϶Ⱦͽ���K��(0<=K<=100)
Ҫ�������0ʱ0�ֿ�ʼ����ʱ��TΪֹ(����T) �������¼���T�Է���Ϊ��λ��0 <= T <= 6000*/
int WHPElement;
int EHPElement;
int RCreate;//�췽������ʿ��˳��ָ��
int BCreate;//����������ʿ��˳��ָ��
int RedWID;
int BlueWID;
int Hours;
int Minutes;
void Time(){//ʱ�����
    printf("%03d:%02d ",Hours,Minutes);
    return;
}
class Weapon{//����
public:
    int ID;
    int Number;
    int ATK;
    Weapon(int id,int n,int atk):ID(id),Number(n),ATK(atk){}
    virtual void Effect(){return;}//����Ч��
};
class sword:public Weapon{//��
public:
    sword(int atk):Weapon(0,1,2/10*atk){//20%ATK,Number1
    }
};
class bomb:public Weapon{//ը��
public:
    bomb(int atk):Weapon(1,1,4/10*atk){//40%ATK,Number1
    }
};
class arrow:public Weapon{//��ʸ
public:
    arrow(int atk):Weapon(2,2,3/10*atk){//30%ATK,Number2
    }
};
bool comp0(const Weapon* a){//�Ƿ��ǽ�
    return a->ID == 0;
}
bool comp1(const Weapon* a){//�Ƿ���ը��
    return a->ID == 1;
}
bool comp2(const Weapon* a){//�Ƿ��Ǽ�ʸ
    return a->ID == 2;
}
bool comp3(const Weapon* a,const Weapon* b){//��������ȽϺ��������ٵ���ǰ��ս��ʹ��
    if(a->ID < b->ID)
        return true;
    if(a->ID == b->ID && a->ID == 2){//��֧arrow
        if(a->Number<b->Number)
            return true;
    }
    return false;
}
bool comp4(const Weapon* a,const Weapon* b){//��������ȽϺ������������ǰ,�ɻ������ʹ��
    if(a->ID < b->ID)
        return true;
    if(a->ID == b->ID && a->ID == 2){//��֧arrow
        if(a->Number>b->Number)
            return true;
    }
    return false;
}
class Warrior{//��ʿ
public:
    int ID;//���
    int HP;//Ѫ��
    int type;//���ͣ��������ж�
    int ATK;//ʵ�幥����
    vector<Weapon*>::iterator W;//ս��������ָ��
    vector<Weapon*> Take;//��������
    Warrior(int id,int hp,int t,int atk):ID(id),HP(hp),type(t),ATK(atk){}
    void WeaponSort(int n){//��������
        if(!Take.empty()){//STL�㷨Sort�����Զ���һ���ȽϺ���
            if(n == 3)sort(Take.begin(),Take.end(),comp3);
            if(n == 4)sort(Take.begin(),Take.end(),comp4);
        }
        return;
    }
    void report(){//�����������
        cout<<ID<<" has ";
        cout<<count_if(Take.begin(),Take.end(),comp0)<<" sword ";
        cout<<count_if(Take.begin(),Take.end(),comp1)<<" bomb ";
        cout<<count_if(Take.begin(),Take.end(),comp2)<<" arrow ";
        cout<<"and "<<HP<<" elements"<<endl;
        return;
    }
    void Attack(Warrior* Enemy){
        Weapon* U = *W;//��һ������
        switch(U->ID){
            case 0://�ý�
                U->ATK = ATK*2/10;
                break;
            case 1://��ը��
                U->ATK = ATK*4/10;
                if(type!=2)//����niniia
                    HP -= U->ATK/2;//ը�����������һ���˺�
                U->Number--;
                break;
            case 2://�ü�ʸ
                U->ATK = ATK*3/10;
                U->Number--;
                break;
        }
        Enemy->HP -= U->ATK;
        if(!U->Number){//ʹ�ô���û��
            vector<Weapon*>::iterator D = W;
            //W++;
            Take.erase(D);
        }
        else
            W++;
        if(W == Take.end())//һ��������������;
            W = Take.begin();
        return;
    }
    void Seize(Warrior* Aim){//�ɻ����
        Aim->WeaponSort(4);//Ŀ��������������
        while(Take.size()<10&&!Aim->Take.empty()){//�������
            Take.push_back(*Aim->Take.begin());//���븴��
            Aim->Take.erase(Aim->Take.begin());//Ŀ��ɾ��
        }
        return;
    }
    virtual void loyal(){return;}
    virtual bool Escape(){return false;}
    virtual void Move(){return;}
    virtual void Rob(Warrior* Aim){return;}
    virtual void name(){return;};
    virtual ~Warrior(){
        for(int i = 0;i<Take.size();i++)//�ֹ��ͷ��ڴ�
            delete Take[i];
        //delete &Take;
    }
};
class dragon:public Warrior{
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    dragon(int id):Warrior(id,OriginalHP,1,ATK){
        //�������
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
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    ninjia(int id):Warrior(id,OriginalHP,2,ATK){
        //�������
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
    //ը���Լ�������
};
int ninjia::OriginalHP = 0;
int ninjia::ATK = 0;
class iceman:public Warrior{
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    iceman(int id):Warrior(id,OriginalHP,3,ATK){
        //�������
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
    void Move(){//Ѫ�������麯��
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
    int loyalty;//�ҳ϶�
public:
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    lion(int id,int k):Warrior(id,OriginalHP,4,ATK),loyalty(k){
        //�������
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
    void Move(){//�ҳ϶ȼ����麯��
        loyalty -= K;
        return;
    }
    void loyal(){
        cout<<loyalty;
        return;
    }
    bool Escape(){//����
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
    static int OriginalHP;//��ʼ����ֵ
    static int ATK;//�̶�������
    wolf(int id):Warrior(id,OriginalHP,5,ATK){}
    void Rob(Warrior* Aim){//��������
        Aim->WeaponSort(4);//Ŀ��������������
        int RType = Aim->Take[0]->ID;//��¼��������
        int number = 0;//��¼�������
        while(Take.size()<10&&!Aim->Take.empty()&&RType == Aim->Take[0]->ID){//�������
            Take.push_back(*Aim->Take.begin());//���븴��
            Aim->Take.erase(Aim->Take.begin());//Ŀ��ɾ��
            number++;
        }
        //����������
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
class City{//����
public:
    int ID;
    Warrior* Blue;//������ʿ
    Warrior* Red;//�췽��ʿ
    //״̬��¼������͵������Ӧ��ʹ�ýṹ���װ��
    int turn;//�غ���
    int ROHP;
    int BOHP;
    int RTN;
    int BTN;
    City(int n):ID(n),Blue(NULL),Red(NULL){}
    void Create(int n){//���˾��е�����Ԫ����������ĳ���������ʿ���ǾʹӴ�ֹͣ������ʿ��
        if(n == 0){//��Ϊ�췽����һ����ʿ
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
        else if(n == 1){//��Ϊ��������һ����ʿ
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
    void Escape(){//lion����
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
    void Rob(){//���ǵĻ�����
        if(Blue!=NULL&&Red!=NULL){
            if(Blue->type == 5&&Red->type != 5&&!Red->Take.empty()){//������
                Time();
                cout<<"blue wolf "<<Blue->ID<<" ";
                Blue->Rob(Red);
                cout<<" from red ";Red->name();
                cout<<" "<<Red->ID<<" in city "<<ID<<endl;
            }
            else if(Blue->type != 5&&Red->type == 5&&!Blue->Take.empty()){//������
                Time();
                cout<<"red wolf "<<Red->ID<<" ";
                Red->Rob(Blue);
                cout<<" from blue ";Blue->name();
                cout<<" "<<Blue->ID<<" in city "<<ID<<endl;
            }
        }
        return;
    }
    void Battle(){//������ִ��ս��
        if(Red!=NULL&&Blue!=NULL){//����ս��
            //ƽ���ж�
            if(Red->Take.empty()&&Blue->Take.empty()){
                BattleResult(1);
                return;
            }
            //��������
            Red->WeaponSort(3);Blue->WeaponSort(3);
            //�����Ⱥ���,����ս���ñ���
            Warrior* first = NULL;
            Warrior* second = NULL;
            if(ID%2){//��1
                first = Red;
                second = Blue;
            }
            else{//��0
                first = Blue;
                second = Red;
            }
            first->W = first->Take.begin(); //����ָ�����
            second->W = second->Take.begin(); //����ָ�����
            turn = 1;
            ROHP = Red->HP;
            BOHP = Blue->HP;
            RTN = Red->Take.size();
            BTN = Blue->Take.size();
            while(!BattleJudge()){//while()�Ⱥ��ֽ���ʹ��
                //ս�����̣�ʹ��һ����������Ѫ���Ļ��ƺ����⼼�ܴ���
                if(!first->Take.empty())
                    first->Attack(second);
                //�����ж�Ȩ
                Warrior* temp = first;
                first = second;
                second = temp;
                turn++;
            }
            BattleResult(BattleJudge());
        }
        return;
    }
    int BattleJudge(){//ʤ��ƽ���ж�,0��ʾ��û����
        if(Red->HP<=0&&Blue->HP<=0)//˫�������ˣ�ƽ��
            return 1;
        if(Red->HP<=0)//�췽���ˣ�����ʤ
            return 3;
        if(Blue->HP<=0)//�������ˣ��췽ʤ
            return 2;
        if(Red->Take.empty()&&Blue->Take.empty())//˫����������������û�ˣ�ƽ��
            return 1;
        if(turn%10 == 0){//˫��״̬�������仯��ƽ��
            if(Red->HP == ROHP &&
               Blue->HP == BOHP &&
               Red->Take.size() == RTN &&
               Blue->Take.size() == BTN)
                return 1;
            else{//��¼����
                ROHP = Red->HP;
                BOHP = Blue->HP;
                RTN = Red->Take.size();
                BTN = Blue->Take.size();
            }
        }
        return 0;//û����
    }
    void BattleResult(int n){//1ƽ�֣�2��ʤ��3��ʤ
        //ʤ��������
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
                Red->Seize(Blue);//�ɻ���
                break;
            case 3:
                cout<<"blue ";Blue->name();
                cout<<" "<<Blue->ID<<" killed red ";Red->name();
                cout<<" "<<Red->ID<<" in city "<<ID<<" remaining "<<Blue->HP<<" elements"<<endl;
                Blue->Seize(Red);//�ɻ���
                break;
        }
        //dragon����
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
    void death(){//��������
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
    void Report(){//����ʿ�Ļ��㱨
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
City* BG[MaxN];//��������
class Events{//�¼�ʱ��
public:
    City* Red;//�췽˾�
    City* Blue;//����˾�
    Events(){}
    //ִ�и��¼�
    void Start(int n){//��������
        cin>>M>>N>>K>>T;
        BCreate = RCreate = 0;//���մ����ͷ��ʼ������ʿ
        RedWID = BlueWID = 1;//���
        Hours = Minutes = 0;//ʱ����㿪ʼ
        WHPElement = EHPElement = M;
        cin>>dragon::OriginalHP>>ninjia::OriginalHP>>iceman::OriginalHP>>lion::OriginalHP>>wolf::OriginalHP;
        cin>>dragon::ATK>>ninjia::ATK>>iceman::ATK>>lion::ATK>>wolf::ATK;
        for(int i=0;i<N+2;i++)//��ʼ����������
            BG[i] = new City(i);
        Red = BG[0];Blue = BG[N+1];
        Process();
        return;
    }
    bool TimeJudge(){//�Ƿ�ʱ
        if(Hours*60+Minutes>T)
            return true;
        return false;
    }
    void Process(){//�¼�����
        while(true){//ʱ�䵽��������˾���ռ����˳�ѭ��
            Minutes = 0;if(TimeJudge())break;//��ÿ�����㣬��ÿ��Сʱ�ĵ�0�֣� ˫����˾��и���һ����ʿ������
            Red->Create(0);//�췽˾�����iceman��lion��wolf��ninja��dragon ��˳��������ʿ��
            Blue->Create(1);//����˾�����lion��dragon��ninja��iceman��wolf ��˳��������ʿ��
            Minutes = 5;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�5�֣������ܵ�lion������һʱ�������ˡ�
                BG[i]->Escape();
            Minutes = 10;if(TimeJudge())break;
            Warrior* R = NULL;//����һ���췽��������ʾ���ߵĳ���������Ҫ�ƶ�
            for(int i=0;i<N+2;i++){////��ÿ��Сʱ�ĵ�10�֣����е���ʿ������˾�����ǰ��һ����
                if(i==0){//����ȥ�췽˾�
                    if(BG[i]->Red!=NULL){//�췽����
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
                if(i==N+1){//�췽ȥ����˾�
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
                //��i��ΪĿ�ĵ�,�췽����,����R��BG[i]->Red��ָ��
                Warrior* temp = R;
                R = BG[i]->Red;
                BG[i]->Red = temp;
                if(BG[i]->Red != NULL){
                    BG[i]->Red->Move();
                    Time();
                    cout<<"red ";BG[i]->Red->name();cout<<" "<<BG[i]->Red->ID<<" marched to city "<<i;
                    cout<<" with "<<BG[i]->Red->HP<<" elements and force "<<BG[i]->Red->ATK<<endl;
                }
                if(BG[i+1]->Blue!=NULL){//�����Ӷ�
                    BG[i]->Blue = BG[i+1]->Blue;
                    BG[i+1]->Blue = NULL;
                    BG[i]->Blue->Move();
                    Time();
                    cout<<"blue ";BG[i]->Blue->name();cout<<" "<<BG[i]->Blue->ID<<" marched to city "<<i;
                    cout<<" with "<<BG[i]->Blue->HP<<" elements and force "<<BG[i]->Blue->ATK<<endl;
                }
            }
            if(Red->Blue!=NULL||Blue->Red!=NULL)break;//�˴���ռ��&�����ж�
            Minutes = 35;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�35�֣�����wolf������˵ĳ��У�wolfҪ����Է���������
                BG[i]->Rob();
            Minutes = 40;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�40�֣�����������ʿ�ĳ��У��ᷢ��ս����
                BG[i]->Battle();
            for(int i=0;i<N+2;i++)//��������
                BG[i]->death();
            Minutes = 50;if(TimeJudge())break;
            //��ÿ��Сʱ�ĵ�50�֣�˾�������ӵ�е�����Ԫ������
            Time();cout<<WHPElement<<" elements in red headquarter"<<endl;
            Time();cout<<EHPElement<<" elements in blue headquarter"<<endl;
            Minutes = 55;if(TimeJudge())break;
            for(int i=0;i<N+2;i++)//��ÿ��Сʱ�ĵ�55�֣�ÿ����ʿ������ӵ�е����������
                BG[i]->Report();//ÿ�����е���ʿ����
            Hours++;
        }
        return;
    }
    ~Events(){//ɾ��һ�У�����ڴ�,���ñ���
        for(int i=0;i<N+2;i++){
            delete BG[i];
        }
        Red = NULL;Blue = NULL;
    }
};
int main(){//������
    int n;cin>>n;
    for(int i=1;i<=n;i++){//ѭ������
        cout<<"Case "<<i<<":"<<endl;
        Events* events = new Events();//�������γ�����¼�
        events->Start(n);//��������
        delete events;//����������һ��
    }
    return 0;
}
