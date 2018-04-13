#include <assert.h>
#include <map>
#include <limits>
#include <iostream>

using namespace std;

template<class K, class V>
class interval_map {
    friend void IntervalMapTest();

private:
    std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    interval_map( V const& val) {
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
        cout << "assign 0 keyBegin = " << keyBegin << ", keyEnd = " << keyEnd << ", V = " << val << endl;
        // auto it_blow = m_map.lower_bound (keyBegin);
        // cout << "assign -1 it_blow->first = " << it_blow->first << endl;

        
        
        auto it_bup = m_map.upper_bound (keyBegin);
        cout << "assign 1 it_bup->first = " << it_bup->first << endl;

        auto vEnd = (*this)[keyEnd];
        cout << "assign 2 vEnd = " << vEnd << endl;

        while( keyBegin < it_bup->first && it_bup->first < keyEnd ) {
            m_map.erase(it_bup);
            it_bup = m_map.upper_bound (keyBegin);  
        }

        m_map.insert({keyBegin, val});
        m_map.insert({keyEnd, vEnd});
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        return ( --m_map.upper_bound(key) )->second;
    }
};


void IntervalMapTest() {
    interval_map<int, char> tmap('A');
    cout << "1 => " << tmap[1] << endl;

    tmap.assign(8, 15, 'B');
    cout << "8 => " << tmap[8] << endl;
    cout << "15 => " << tmap[15] << endl;

    tmap.assign(20, 25, 'C');
    cout << "20 => " << tmap[20] << endl;
    cout << "25 => " << tmap[25] << endl;

    tmap.assign(3, 10, 'E');

    /*
    std::map<int, char>::iterator iter;  
    for(iter = tmap.begin(); iter != tmap.end(); iter++) {
        cout << iter->first << ' ' << iter->second << endl;
    }
    */
    cout << "1 => " << tmap[1] << endl;
    cout << "3 => " << tmap[3] << endl;
    cout << "8 => " << tmap[8] << endl;
    cout << "10 => " << tmap[10] << endl;
    cout << "15 => " << tmap[15] << endl;
    cout << "20 => " << tmap[20] << endl;
    cout << "25 => " << tmap[25] << endl;
    cout << "30 => " << tmap[30] << endl;
}

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a function IntervalMapTest() here that tests the
// functionality of the interval_map, for example using a map of unsigned int
// intervals to char.

int main() {
    IntervalMapTest();
}