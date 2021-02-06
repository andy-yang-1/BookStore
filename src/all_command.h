
#ifndef BOOKSTORE_2021_MAIN_ALL_COMMAND_H
#define BOOKSTORE_2021_MAIN_ALL_COMMAND_H

#include "unrolled_linked_list.h"

class command
{
public:

    stringstream command_stream ;
    vector<user> user_stack ;
    vector<int> select_stack ;
//    pair< bool , pair<int,int> > search_pos ;

public:

    command() ;

    void run_command() ;

    void get_command( string command_str ) ;

    bool has_more_token() ;

    void check_privilege( int limit ) ;

    void check_selected() ;

    int select_offset() ;

    void del_ISBN() ; //将选中书目的所有索引删除

    void del_name() ;

    void del_author() ;

    void del_keyword() ;

    void add_ISBN() ;

    void add_name() ;

    void add_author() ;

    void add_keyword() ;

    void check_existing_ISBN( book temp_book ) ;

    void SU() ;

    void LOGOUT() ;

    void USERADD() ;

    void REGISTER() ;

    void DELETE() ;

    void PASSWD() ;

    void SELECT() ;

    void MODIFY() ;

    void IMPORT() ;

    void SHOW() ;

    void BUY() ;

    void REPORT() ;

};

int get_strip( string line ) ;

key_type StringToKey ( string &line ) ; // 返回正确的类型同时将line变为需要的内容

void get_keyword_main( element &put_element , string line ) ;

#endif //BOOKSTORE_2021_MAIN_ALL_COMMAND_H
