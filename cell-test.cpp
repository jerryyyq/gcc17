#include <assert.h>
#include <map>
#include <limits>
#include <iostream>
#include <ctime>

using namespace std;

#define random(a,b) (rand()%(b-a+1)+a)

template<class K, class V>
class interval_map {
    friend void IntervalMapTest();

private:
    std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    interval_map( V const& val) {
        cout << "interval_map std::numeric_limits<K>::lowest() = " << std::numeric_limits<K>::lowest() << ", val = " << val << endl;
        m_map.insert(m_map.begin(),std::make_pair(std::numeric_limits<K>::lowest(),val));
    }

    // Assign value val to interval [keyBegin, keyEnd). 
    // Overwrite previous values in this interval. 
    // Do not change values outside this interval.
    // Conforming to the C++ Standard Library conventions, the interval 
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval, 
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        cout << " ++++++++ assign 0 keyBegin = " << keyBegin << ", keyEnd = " << keyEnd << ", V = " << val << endl;
        // auto it_blow = m_map.lower_bound (keyBegin);
        // cout << "assign -1 it_blow->first = " << it_blow->first << endl;
        if( keyBegin >= keyEnd ) {
            return;

            // "and assign must do nothing." what's mean?
            // or:
            auto val_add = m_map.end()->second;
            auto keyEnd_add = keyBegin;
            auto keyBegin_add = keyEnd;

            if( keyEnd == keyBegin )
                keyEnd_add = keyBegin + 1;

            auto vEnd = (*this)[keyEnd_add];
            auto it_del_first = m_map.upper_bound( keyBegin_add );
            auto it_del_last = m_map.lower_bound( keyEnd_add );
            m_map.erase( it_del_first, it_del_last );
            m_map.insert({keyBegin_add, val_add});
            m_map.insert({keyEnd_add, vEnd});

            return;
        }

        auto vEnd = (*this)[keyEnd];
        auto it_del_first = m_map.lower_bound( keyBegin );
        auto it_del_last = m_map.upper_bound( keyEnd );

        cout << "assign 1 it_del_first->first = " << it_del_first->first << ", it_del_first->second = " << it_del_first->second << endl;
        cout << "assign 1 it_del_last->first = " << it_del_last->first << ", it_del_last->second = " << it_del_last->second << endl;

        // erase old duplicate key
        bool bInsertBegin = true, bInsertEnd = true;
        auto it_befor_begin = --m_map.upper_bound( keyBegin );
        cout << "assign 2 it_befor_begin->first = " << it_befor_begin->first << ", it_befor_begin->second = " << it_befor_begin->second << endl;

        if( it_befor_begin != m_map.end() ) {
            auto vBeginBefor = it_befor_begin->second;
            if( vBeginBefor == val )
            {
                it_del_first = ++it_befor_begin;
                cout << "assign 10 it_del_first->first = " << it_del_first->first << ", it_del_first->second = " << it_del_first->second << endl;
                bInsertBegin = false;
            }
        }

        cout << "assign 3 vEnd = " << vEnd << endl;
        if( vEnd == val ) {
            //it_del_last = m_map.upper_bound( keyEnd );
            bInsertEnd = false;
        }

        // delete old key between keyBegin and keyEnd
        cout << "assign 4 it_del_first->first = " << it_del_first->first << ", it_del_first->second = " << it_del_first->second << endl;
        cout << "assign 4 it_del_last->first = " << it_del_last->first << ", it_del_last->second = " << it_del_last->second << endl;

        if( it_del_first != m_map.end() && it_del_first->first < keyEnd && \
                ( it_del_first->first <= it_del_last->first || it_del_last == m_map.end() ) ) {
            cout << "---------------erase" << endl;
            if( it_del_first->first == it_del_last->first )
                m_map.erase( it_del_first );
            else
                m_map.erase( it_del_first, it_del_last );
        }

        /*
        while( keyBegin < it_bup->first && it_bup->first < keyEnd ) {
            m_map.erase(it_bup);
            it_bup = m_map.upper_bound( keyBegin );  
        }
        */

        if( bInsertBegin )
            m_map.insert({keyBegin, val});

        if( bInsertEnd )
            m_map.insert({keyEnd, vEnd});

        print_map();
    }


    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        return ( --m_map.upper_bound(key) )->second;
    }

    void print_map()
    {
        auto it = m_map.begin();
        while(it != m_map.end())
        {
            cout << "print_map it->first = " << it->first << ", it->second " << it->second << endl;

            it ++;         
        }
    }
};


void IntervalMapTest() {
    interval_map<int, char> tmap('A');

    srand((unsigned)time(NULL));
    auto keyBegin = 0, keyEnd = 0;
    for( int i=0; i<1000; i++ ) {
        keyBegin = random(0, 10);
        keyEnd = random(0, 10);
        tmap.assign(keyBegin, keyEnd, 'A' + keyBegin);
        cout << "---------------------------------- i = " << i << endl << endl;
    }
    return;



    cout << "1 => " << tmap[1] << endl;

    tmap.assign(8, 15, 'B');
    cout << "8 => " << tmap[8] << endl;
    cout << "15 => " << tmap[15] << endl;

    tmap.assign(20, 25, 'C');
    cout << "20 => " << tmap[20] << endl;
    cout << "25 => " << tmap[25] << endl;

    tmap.assign(3, 10, 'E');

    tmap.assign(-100, 2, 'F');
    tmap.assign(15, 4, 'H');
    tmap.assign(0, 22, 'J');
    tmap.assign(std::numeric_limits<int>::lowest(), 1000, 'T');    
    tmap.assign(0, 22, 'T');
    tmap.assign(2000, 3000, 'T');
    tmap.assign(2000, 3000, 'T');
    tmap.assign(2000, 3000, 'U');
    tmap.assign(2000, 3000, 'T');
    tmap.assign(900, 1100, 'T');
    tmap.assign(1900, 2200, 'T');
    tmap.assign(1800, 3200, 'T');

    /*
    std::map<int, char>::iterator iter;  
    for(iter = tmap.begin(); iter != tmap.end(); iter++) {
        cout << iter->first << ' ' << iter->second << endl;
    }
    */
    cout << "1 => " << tmap[1] << endl;
    cout << "2 => " << tmap[2] << endl;
    cout << "3 => " << tmap[3] << endl;
    cout << "4 => " << tmap[4] << endl;
    cout << "8 => " << tmap[8] << endl;
    cout << "10 => " << tmap[10] << endl;
    cout << "15 => " << tmap[15] << endl;
    cout << "20 => " << tmap[20] << endl;
    cout << "22 => " << tmap[22] << endl;
    cout << "25 => " << tmap[25] << endl;
    cout << "1000 => " << tmap[1000] << endl;
    cout << "-10 => " << tmap[-10] << endl;
    cout << "-1000 => " << tmap[-1000] << endl;
}


int main() {

    std::map<int, char> test_map;
    test_map.insert({std::numeric_limits<int>::lowest(), 'O'});
    
    test_map.insert({5, 'A'});
    test_map.insert({10, 'B'});

    auto it_first = test_map.lower_bound( 8 );
    auto it_last = test_map.upper_bound( 8 );

    if( it_first == test_map.end() )
        cout << "it_first is end ";
    if( it_last == test_map.end() )
        cout << "it_last is end ";
    if( it_first == test_map.begin() )
        cout << "it_first is begin ";
    if( it_last == test_map.begin() )
        cout << "it_first is end ";

    cout << "test 1 it_first->first = " << it_first->first << ", it_first->second = " << it_first->second << endl;
    cout << "test 1 it_last->first = " << it_last->first << ", it_last->second = " << it_last->second << endl << endl;


    it_first = test_map.lower_bound( 1 );
    it_last = test_map.upper_bound( 1 );

    if( it_first == test_map.end() )
        cout << "it_first is end ";
    if( it_last == test_map.end() )
        cout << "it_last is end ";
    if( it_first == test_map.begin() )
        cout << "it_first is begin ";
    if( it_last == test_map.begin() )
        cout << "it_first is end ";

    cout << "test 2 it_first->first = " << it_first->first << ", it_first->second = " << it_first->second << endl;
    cout << "test 2 it_last->first = " << it_last->first << ", it_last->second = " << it_last->second << endl << endl;


    it_first = test_map.lower_bound( 5 );
    it_last = --test_map.upper_bound( 25 );

    if( it_first == test_map.end() )
        cout << "it_first is end ";
    if( it_last == test_map.end() )
        cout << "it_last is end ";
    if( it_first == test_map.begin() )
        cout << "it_first is begin ";
    if( it_last == test_map.begin() )
        cout << "it_first is end ";

    cout << "test 1 it_first->first = " << it_first->first << ", it_first->second = " << it_first->second << endl;
    cout << "test 1 it_last->first = " << it_last->first << ", it_last->second = " << it_last->second << endl;


    test_map.insert({200, 'X'});
    test_map.insert({210, 'Y'});
    test_map.insert({220, 'Z'});
    auto it_e = test_map.lower_bound( 200 );
    cout << "test 2 it_e->first = " << it_e->first << ", it_e->second = " << it_e->second << endl;
    auto it_e2 = test_map.lower_bound( 210 );
    cout << "test 2 it_e2->first = " << it_e2->first << ", it_e2->second = " << it_e2->second << endl;

    test_map.erase(it_e2);
    test_map.insert({220, 'T'});

    auto it = test_map.begin();
    while(it != test_map.end())
    {
        cout << "print_map it->first = " << it->first << ", it->second " << it->second << endl;

        it ++;         
    }

    cout << "================================================================= " << endl << endl;


















    IntervalMapTest();
}