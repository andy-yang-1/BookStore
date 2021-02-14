
#ifndef BOOKSTORE_2021_MAIN_ALL_HEADER_H
#define BOOKSTORE_2021_MAIN_ALL_HEADER_H


#define BOOK_FILE "book_file.dat"
#define USER_FILE "user_file.dat"
#define USER_ID_FILE "user_id_file.dat"
#define ISBN_FILE "ISBN_file.dat"
#define NAME_FILE "name_file.dat"
#define AUTHOR_FILE "author_file.dat"
#define KEYWORD_FILE "keyword_file.dat"
#define FINANCE_FILE "finance_file.dat"
#define CORE_FILE "core_file.dat"
#define BLOCK_SIZE 320
#define MERGE_LIMIT 160

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>

enum element_type { BOOK_TYPE , USER_TYPE }; // 两种元素类型

enum key_type { USER_ID_TYPE , ISBN_TYPE , NAME_TYPE , AUTHOR_TYPE , KEYWORD_TYPE , PRICE_TYPE };

int get_block_num( key_type KeyType ) ; // todo 用add_block函数代替

void change_block_num( key_type KeyType , int num ) ; // todo 不需要block_num的统计

int get_element_num( element_type ElementType ) ; // todo 干脆不再使用

void change_element_num( element_type ElementType , int num ) ; // todo 干脆不再使用

char* get_file_name( key_type KeyType ) ; // 将element_type转化成文件名,是new出来的

key_type getKeyType( const char* filename ) ; // 将filename变为key_type


class user
{
public:
    char user_id[31] = {0} ;
    char passwd[31] = {0};
    char name[31] = {0};
    int privilege = 0 ;

public:

    user() ;

    user( const char u[] ) ;

    user( const char u[] , const char pa[] , const char n[] , int pr ) ;

    user( const user &other ) ;

    user &operator=( const user &other ) ;

    bool operator== ( const user &other ) ;

    void get_user( int offset ) ;

    void put_user( int offset ) ;

    bool right_password( const char *pw ) ;
};



class book
{
public:

    char ISBN[21] = {0} ;
    double price = 0 ;
    char name[61] = {0};
    char author[61] = {0};
    char keyword[61] = {0} ;
    int quantity = 0 ;

public:

    book() ;

    book( char isbn[] ) ;

    book( char isbn[] , double p , char n[] , char a[] , char k[] ) ;

    book( const book &other ) ;

    book &operator=( const book &other ) ;

    void get_book( int offset ) ;

    void put_book( int offset ) ;

    void print_book() ;

    bool has_name() ;

    bool has_author() ;

    bool has_keyword() ;
};


class block ;

class element
{
public:

    char keyword_main[61] = {0} ;
    char keyword_auxiliary[61] = {0};  // keyword_main作为主搜索，keyword_auxiliary作为确认搜索(一般为ISBN号)
    int offset = -1 ;

public:

    element() ;

    element( const char *k1 , const char *k2 , int os ) ;

    element( const element &other ) ;

    element &operator= ( const element &other ) ;

    bool operator< ( const element &other ) ;

    bool operator< ( const block &other ) ;

    bool operator== ( const element &other ) ; // 判断auxiliary相等

    bool operator== ( const block &other ) ;

    bool operator> ( const element &other ) ;

    bool equal_with( const element &other ) ; // 判断main相等
};

class block
{
public:

    int pos = -1 ;
    int up = -1 ;
    int down = -1 ;
    int length = 0 ;
    element data[BLOCK_SIZE] ;

public:

    void get_block( key_type KeyType , int offset ) ; // 读入block
    void put_block( key_type KeyType , int offset ) ; // 放回block

    void add_block( key_type KeyType ) ;

};

int get_finance_times() ;

void change_finance_times( int num ) ; // todo 用get_finance_times来代替

void change_finance( double flux ) ; // 正值为收入，负值为支出

void print_finance( int times = -1 ) ; // -1次指打印出所有的账单

void big_show() ; // 无参数show

int add_book( book &add_element ) ;  // 返回offset

int add_user( user &add_element ) ; // 返回offset

void file_initialize() ;

void data_initialize() ;

#endif //BOOKSTORE_2021_MAIN_ALL_HEADER_H
