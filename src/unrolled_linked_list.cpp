#include "unrolled_linked_list.h"


List::List() {}

List::List(const char *file_name)
{
    strcpy( filename , file_name ) ;
}

void List::add_key(element &add_element, pair<int, pair<int, int>> search_pos)
{
    block temp_block ;
    temp_block.get_block( getKeyType(filename) , search_pos.second.first ) ;
    if ( search_pos.first != -1 ) cerr << "add_key: existing key" << endl ;

    for ( int i = temp_block.length ; i > search_pos.second.second ; i-- ){
        temp_block.data[i] = temp_block.data[i-1] ;
    }// 将所有数据位移一格

    temp_block.data[search_pos.second.second] = add_element ;
    temp_block.length++ ;
    temp_block.put_block(getKeyType(filename),temp_block.pos) ;

    if ( temp_block.length == BLOCK_SIZE ){
        this->split_block( getKeyType(filename) , temp_block.pos ) ;
    }
}

void List::del_key(pair< int , pair<int,int> > search_pos)
{
    block temp_block , nxt_block ;
    temp_block.get_block( getKeyType(filename) , search_pos.second.first ) ;  // todo 减少不必要的文件读写
    if ( search_pos.first == -1 ) cerr << "del_key: not existing key" << endl ;

    for ( int i = search_pos.second.second ; i < temp_block.length ; i++ ){
        temp_block.data[i] = temp_block.data[i+1] ;
    }

    temp_block.length-- ;
    temp_block.put_block(getKeyType(filename),temp_block.pos) ;

    if ( temp_block.down != -1 ){
        nxt_block.get_block( getKeyType(filename) , temp_block.down ) ;
        if ( temp_block.length + nxt_block.length < MERGE_LIMIT ){
            this->merge_block( getKeyType(filename) , temp_block.pos , nxt_block.pos ) ;
        }
    }

}

pair< int , pair<int, int> > List::search_key(key_type KeyType, element &search_element)
{
    block temp_block ;
    temp_block.get_block( KeyType , 0 ) ; // todo search_key改成二分

    while (true){
        if ( search_element < temp_block || search_element == temp_block || temp_block.down == -1 ) break ;
        temp_block.get_block( KeyType , temp_block.down ) ;
    }

    if ( search_element == temp_block ) {
        return pair< int , pair<int,int> > ( {temp_block.data[0].offset ,{temp_block.pos,0}} ) ;
    }

    if ( search_element < temp_block && temp_block.up != -1 ) {
        temp_block.get_block( KeyType , temp_block.up ) ;
    }

    int counter = 0 ;

    while ( counter < temp_block.length ){
        if ( search_element == temp_block.data[counter] ){ // 找到目标
            return pair< int , pair<int,int> > ( {temp_block.data[counter].offset,{temp_block.pos,counter}} ) ;
        }
        if ( search_element < temp_block.data[counter] ){ // 目标不存在
            return pair< int , pair<int,int> > ( { -1 , {temp_block.pos,counter} } ) ;
        }
        if ( search_element > temp_block.data[counter] ){
            counter++ ;
        }
    }

    return pair< int , pair<int,int> > ( {-1,{temp_block.pos,counter}} ) ; // 未找到，在其最后一位

}

int List::get_key(pair< int , pair<int, int>> search_pos)
{
    return search_pos.first ;
}

void List::split_block(key_type KeyType, int offset)
{
    block origin_block , temp_block , nxt_block ;
    origin_block.get_block(KeyType,offset) ;

    for ( int i = BLOCK_SIZE / 2 ; i < BLOCK_SIZE ; i++ ){
        temp_block.data[i-BLOCK_SIZE/2] = origin_block.data[i] ;
    }

    temp_block.length = BLOCK_SIZE / 2 ;
    origin_block.length = BLOCK_SIZE / 2 ;

    int block_num = get_block_num(KeyType) ;

    temp_block.pos = block_num ;
    temp_block.up = origin_block.pos ;

    if ( origin_block.down == -1 ){
        origin_block.down = temp_block.pos ;
        origin_block.put_block(KeyType,origin_block.pos) ;
        temp_block.add_block(KeyType) ;
    }else{
        nxt_block.get_block(KeyType,origin_block.down) ;
        nxt_block.up = temp_block.pos ;
        temp_block.down = nxt_block.pos ;
        origin_block.down = temp_block.pos ;
        origin_block.put_block(KeyType,origin_block.pos) ;
        temp_block.add_block(KeyType) ;
        nxt_block.put_block(KeyType,nxt_block.pos) ;
    }


}

void List::merge_block(key_type KeyType, int first_block_pos, int second_block_pos)
{
    block up_block , down_block , nxt_block ;
    up_block.get_block(KeyType,first_block_pos) ;
    down_block.get_block(KeyType,second_block_pos) ;

    for ( int i = 0 ; i < down_block.length ; i++ ){
        up_block.data[i+up_block.length] = down_block.data[i] ;
    }

    up_block.down = down_block.down ;
    up_block.length += down_block.length ;

    if ( down_block.down == -1 ){
        up_block.put_block(KeyType,up_block.pos) ;
    }else{
        nxt_block.get_block(KeyType,down_block.down) ;
        nxt_block.up = up_block.pos ;
        up_block.put_block(KeyType,up_block.pos) ;
        nxt_block.put_block(KeyType,nxt_block.pos) ;
    }
}

void List::show_key(key_type KeyType, const char *main_key)
{
    element searched_element( main_key , "" , 0 ) ;
    pair< int , pair<int,int> > start_pos = search_key(KeyType,searched_element) ;
    block temp_block ;
    temp_block.get_block(KeyType,start_pos.second.first) ;
    if ( !temp_block.data[start_pos.second.second].equal_with(searched_element) ){
        if ( start_pos.second.second == temp_block.length && temp_block.down != -1 ){
            temp_block.get_block(KeyType,temp_block.down) ;
            if ( temp_block.data[0].equal_with(searched_element) ){
                start_pos.second.second = 0 ;
            }else{ cout << endl ; return ;}
        }else{ cout << endl ; return ; }
    }
    int start_point = start_pos.second.second , offset ;
    book temp_book ;
    while (true){
        for ( int i = start_point ; i < temp_block.length ; i++ ){
            if ( !temp_block.data[i].equal_with(searched_element) ) return ;
            temp_book.get_book(temp_block.data[i].offset) ;
            temp_book.print_book() ;
        }
        if ( temp_block.down == -1 ) return ;
        temp_block.get_block( KeyType , temp_block.down ) ;
        start_point = 0 ;
    }
}









