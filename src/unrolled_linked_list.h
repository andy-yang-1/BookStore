
#ifndef BOOKSTORE_2021_MAIN_UNROLLED_LINKED_LIST_H
#define BOOKSTORE_2021_MAIN_UNROLLED_LINKED_LIST_H

#include "all_header.h"

#define merge_limit 50
#define split_limit 200
using namespace std ;

class List
{
public:

    char filename[30] = {0} ;

public:

    List() ;

    List( const char *file_name ) ;

    void add_key( element &add_element , pair< int , pair<int,int> > search_pos ) ;

    void del_key( pair< int , pair<int,int> > search_pos ) ;

    pair< int , pair<int,int> > search_key( key_type KeyType , element &search_element ) ; // todo search_pos的三个元素分别是 offset block_pos element_pos

    int get_key( pair< int , pair<int,int> > search_pos ) ; // 将search的结果传入get_key中，返回get_element的offset

    void split_block( key_type KeyType , int offset ) ; // 若要split_block的话，put_block需要在split内完成

    void merge_block( key_type KeyType , int first_block_pos , int second_block_pos ) ;

    void show_key( key_type KeyType , const char* main_key ) ;

};


#endif //BOOKSTORE_2021_MAIN_UNROLLED_LINKED_LIST_H
