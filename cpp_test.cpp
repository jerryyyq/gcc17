#include <iostream>
#include <cstring>
#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include <math.h>

using namespace std;

class ABC
{
public:
    static int a;
    int b;

    ABC(int aa, int bb)
    {
        a = aa;
        b = bb;
    }

    void inc()
    {
        a++;
        b++;
    }
};

int ABC::a = 6;

class A{
    private:
    int val;
    public:
    A(int i):val(i){
        cout << "Base\n";
    }
};

class B: protected A{
public:
    B(int i):A(i){
        cout << "Der\n";
    }
};

/*
float interest(int p, float r = 2.5, int t){
    return (p*r*t)/100;
}
*/

struct S{
    float f;
    char c;
    int i;
};

int main()
{
    ABC aaa(2, 3);
    ABC bbb(10, 20);
    aaa.inc();
    bbb.inc();

    cout << "aaa.a = " << aaa.a << ", aaa.b =  " << aaa.b << endl;    
    cout << "bbb.a = " << bbb.a << ", bbb.b =  " << bbb.b << endl;    

    B binstance(2);

//    cout << 'X';
//    cout.width(5);
//    cout<<3,4;

//    float i = interest(100, 10, 5);
//    cout << "I :" << i;

    S s1={1.1f, 'a', 99};
    S s2={1.1f, 'a', 99};

    bool bS = memcmp(&s1, &s2, sizeof(S)) == 0;
    cout << bS;


    int *p = 0;
    delete p;
    delete p;
    cout << "HYE";


    int i = 3;
    int j = i<<2;
    int x = i * j;
    cout <<i<<" * " <<j<<" = "<<x << endl;

    int x2 = 3;
    printf("%d %d %d", x2, x2++, x2++);
    cout << endl;

    int c, d,k;

    c = (d = 4) + (k = 9);
    cout << c << endl;


/*
    char str[5];
    cout << "Input : ";
    cin >> str;

    for(int ii =  strlen(str); ii >=0; ii--)
        for(int jj = 0; jj < 2; jj++)
            cout << str[--ii];

    getch();
*/
    int re = 2, base = 2;
    int power = 10;
    power--;
    while(power)
    {
        re = re * base;
        power--;
    }

    cout << re % 10000 << endl;

    return 0;
}