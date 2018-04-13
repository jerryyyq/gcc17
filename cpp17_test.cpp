// sudo docker run -i -t -v /home/yangyuqi/gcc17:/root/gcc17 --privileged=true 352b3df66999 /bin/bash
// g++ -std=c++17 -pthread -o a.out cpp17_test.cpp

#include <memory>
#include <iostream>
#include <future>
#include <thread>
#include <string_view>
#include <map>
#include <vector>
// #include <execution>  // 目前 std 里没有
#include <future>
#include <thread>
#include <variant>
#include <any>

using namespace std;

template<typename T>
shared_ptr<T> make_shared_array(size_t size)
{
    // return make_shared< T[], default_delete<T[]> > ( size );  // 编译不通过
    return shared_ptr<T>( new T[size], default_delete<T[]>() );
}

tuple<int, std::unique_ptr<unsigned char[]>> modifyBuffer( std::unique_ptr<unsigned char[]> source_buf )
{
    for(int i = 0; i < 1000; i++)
    {
        source_buf[i] = (unsigned char)2;
    }

    return make_tuple( 1000, std::move(source_buf) );
}

      
tuple<int, std::shared_ptr<unsigned char>> modifyBuffer2( std::shared_ptr<unsigned char> source_buf )
{
    for(int i = 0; i < 1000; i++)
    {
        source_buf.get()[i] = (unsigned char)2;
    }

    return make_tuple( source_buf.use_count(), source_buf );
}

void printAnyString(std::string_view strIn)
{
    cout << "strIn len = " << strIn.length() << ", strIn = " << strIn << endl;
}

auto changeVecotr(vector<int> source)
{
    source[0] = 2;
    source[1] = 2;

    return source;
}

auto & changeVecotr2(vector<int> & source)
{
    source[0] = 2;
    source[1] = 2;

    return source;
}




int main()
{
    //////////////////// test unique_ptr /////////////////    
    cout << endl << "///////////// unique_ptr /////////////////" << endl;
    auto source = std::make_unique< unsigned char[] >( 100 * 1024 * 1024 ); //100 M
    cout << "source address: " << static_cast<const void *>( &source ) << endl;
    cout << "source ptr: " << static_cast<const void *>( source.get() ) << endl;

    auto[len, target] = modifyBuffer( std::move(source) );
    cout << "target address: " << static_cast<const void *>( &target ) << endl;
    cout << "target ptr: " << static_cast<const void *>( target.get() ) << endl;
    cout << "target[1] = " << target[1] << ", target.get()[1] = " << target.get()[1] << endl;
    cout << "source.get() = " << static_cast<const void *>( source.get() ) << endl;


    //////////////////// test shared_ptr /////////////////
    cout << endl << "///////////// shared_ptr /////////////////" << endl;

    auto source2 = make_shared_array< unsigned char >( 100 * 1024 * 1024 ); //100 M
    cout << "source2 use_count: " << source2.use_count() << endl;
    cout << "source2 ptr: " << static_cast<const void *>( source2.get() ) << endl;

    auto [ret_count, target2] = modifyBuffer2( source2 );
    cout << "target2 address: " << static_cast<const void *>( &target2 ) << endl;
    cout << "target2 use_count: " << target2.use_count() << ", ret_count:" << ret_count << ", target2[1] = " << target2.get()[1] << endl;
    cout << "target2 ptr: " << static_cast<const void *>( target2.get() ) << endl;
    cout << "source2.get() = " << static_cast<const void *>( source2.get() ) << endl;


    //////////////////// test string_view /////////////////
    cout << endl << "///////////// string_view /////////////////" << endl;

    // auto pTest1 = "char * string test1";          // 这样是可以的
    // constexpr char * pTest1 = "char * string test1";  // 编译时初始化 不能编译通过，因为 constexpr 仅指 指针本身是常量
    const char * const pTest1 = "char * string test1";
    std::string strTest2 = "std::string test2";
    printAnyString(pTest1);
    printAnyString(strTest2);


    //////////////////// test map /////////////////
    cout << endl << "///////////// map /////////////////" << endl;
    std::map<string, string> mapStudent;
    mapStudent.insert( {"xiao_ming", "xiao_ming's data"} );
    mapStudent.insert( {"xiao_hong", "xiao_hong's data"} );
    std::map<string, string>::iterator iter;  
    for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
        cout << iter->first << ' ' << iter->second << endl;

    cout << mapStudent.find("xiao_ming")->second << endl;

    //////////////////// test CTAD /////////////////
    cout << endl << "///////////// CTAD /////////////////" << endl;
    vector v17 = {1.2, 3.4};
    cout << "v17[0] = " << v17[0] << ", v17[1] = " << v17[1] << endl;


    //////////////////// test RVO & NRVO /////////////////
    cout << endl << "///////////// RVO & NRVO /////////////////" << endl;
    vector huge_vector(100 * 1024 * 1024, 1);
    cout << "huge_vector address = " << static_cast<const void *>( &huge_vector ) << endl;
    cout << "huge_vector[0] address = " << static_cast<const void *>( &huge_vector[0] ) << ", value = " << huge_vector[0] << endl;
    cout << "huge_vector[1] address = " << static_cast<const void *>( &huge_vector[1] ) << ", value = " << huge_vector[1] << endl;
    cout << "huge_vector[2] address = " << static_cast<const void *>( &huge_vector[2] ) << ", value = " << huge_vector[2] << endl;

    auto target_vector = changeVecotr( huge_vector );
    cout << "target_vector address = " << static_cast<const void *>( &target_vector ) << endl;
    cout << "target_vector[0] address = " << static_cast<const void *>( &target_vector[0] ) << ", value = " << target_vector[0] << endl;
    cout << "target_vector[1] address = " << static_cast<const void *>( &target_vector[1] ) << ", value = " << target_vector[1] << endl;
    cout << "target_vector[2] address = " << static_cast<const void *>( &target_vector[2] ) << ", value = " << target_vector[2] << endl;

    /* 这段放开会产生误导性的影响，误以为 changeVecotr2 会优化，其实 changeVecotr2 也没有优化
    huge_vector = changeVecotr( huge_vector );
    cout << "huge_vector address = " << static_cast<const void *>( &huge_vector ) << endl;
    cout << "huge_vector[0] address = " << static_cast<const void *>( &huge_vector[0] ) << ", value = " << huge_vector[0] << endl;
    cout << "huge_vector[1] address = " << static_cast<const void *>( &huge_vector[1] ) << ", value = " << huge_vector[1] << endl;
    cout << "huge_vector[2] address = " << static_cast<const void *>( &huge_vector[2] ) << ", value = " << huge_vector[2] << endl;
    */

    auto target_vector2 = changeVecotr2( huge_vector );
    cout << "target_vector2 address = " << static_cast<const void *>( &target_vector2 ) << endl;
    cout << "target_vector2[0] address = " << static_cast<const void *>( &target_vector2[0] ) << ", value = " << target_vector2[0] << endl;
    cout << "target_vector2[1] address = " << static_cast<const void *>( &target_vector2[1] ) << ", value = " << target_vector2[1] << endl;
    cout << "target_vector2[2] address = " << static_cast<const void *>( &target_vector2[2] ) << ", value = " << target_vector2[2] << endl;
    cout << "结论：对于修改入参的内容，如果以返回值返回，未见优化结果，还是拷贝了一份。对于大数据要修改的，还是应该用引用" << endl;

    ////////////////// async /////////////////
    cout << endl << "///////////// async /////////////////" << endl;
	std::future<int> f1 = std::async( std::launch::async, [](int x, int y){
		return x + y;
	}, 2, 3 );
	cout << "f1 ret = " << f1.get() << endl;


    /* ////////////////// narrow_cast ///////////////// 目前编译不支持
    char c = narrow_cast<char>(1000);
    cout << "narrow_cast c = " << c << endl;
    */

    /*
    int x = 0;
    std::mutex m;
    int a[] = {1,2};
    std::for_each( std::execution::par, std::begin(a), std::end(a), [&](int) {
        std::lock_guard<std::mutex> guard(m); // Error: lock_guard constructor calls m.lock()
        ++x;
    });
    */

    ////////////////// std::any /////////////////
    cout << endl << "///////////// std::any /////////////////" << endl;
    auto a = any(12);
    cout << "a = " << any_cast<int>(a) << endl;

    try {
        cout << "a = " << any_cast<string>(a) << endl;
    }
    catch(const std::bad_any_cast& e)
    {
        std::cout << e.what() << endl;
    }
 
    // advanced example
    a = std::string("hello");
    auto& ra = std::any_cast<std::string&>(a); //< reference
    ra[1] = 'o';
    std::cout << "a = " << std::any_cast<const std::string&>(a) << endl; //< const reference

    ////////////////// std::variant /////////////////
    cout << endl << "///////////// std::variant /////////////////" << endl;
    // std::variant<int, double, std::wstring> var1{ 1.0 };
    std::variant<int, float> v, w;
    v = 12; // v contains int
    auto i = std::get<int>(v);
    cout << "i = " << i << endl;

    w = v;
    try
    {
        auto wf = std::get<float>(w); // w contains int, not float: will throw
        cout << "wf = " << wf << endl;

    }
    catch (const std::bad_variant_access& e) 
    {
        std::cout << "get wf fail: " << e.what() << endl;
    }

    auto pval = std::get_if<0>(&v);
    cout << "variant value: " << *pval << endl;

    std::variant<int, double, std::wstring> var2{ L"23" };
    std::variant<int, std::wstring> var3{ 3 };

    std::wcout << L"the var2 = " << std::get<2>(var2) << std::endl;


    return 0;
}


// 在线编译器：https://godbolt.org/