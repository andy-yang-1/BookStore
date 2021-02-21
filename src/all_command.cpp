#include "all_command.h"


command::command()
{
    user temp_user ;
    user_stack.push_back(temp_user) ;
    select_stack.push_back(-1) ;
    command_stream.str("") ;
    command_stream.clear() ;
}

void command::run_command()
{
    string real_command ;
    command_stream >> real_command ;
    if ( real_command == "" ) return ; // 空行命令
    else if ( real_command == "su" ) { this->SU() ; }
    else if ( real_command == "logout" ) { this->LOGOUT() ; }
    else if ( real_command == "useradd" ) { this->USERADD() ; }
    else if ( real_command == "register" ) { this->REGISTER() ; }
    else if ( real_command == "delete" ) { this->DELETE() ; }
    else if ( real_command == "passwd" ) { this->PASSWD() ; }
    else if ( real_command == "select" ) { this->SELECT() ; }
    else if ( real_command == "modify" ) { this->MODIFY() ; }
    else if ( real_command == "import" ) { this->IMPORT() ; }
    else if ( real_command == "show" ) { this->SHOW() ; }
    else if ( real_command == "buy" ) { this->BUY() ; }
    else if ( real_command == "report" ) { this->REPORT() ; }
    else if ( real_command == "log" ) { this->LOG() ; }
    else if ( real_command == "exit" || real_command == "quit" ) { exit(0) ; }
    else { throw invalid_argument("run_command: illegal command") ; }
    command_stream.str("") ;
    command_stream.clear() ;
}

void command::get_command(string command_str)
{
    command_stream.str("") ;
    command_stream.clear() ;
    command_stream << command_str ;
}

bool command::has_more_token()
{
    if ( !command_stream.eof() ) return true ;
    else return false ;
}

void command::check_privilege(int limit)
{
    user log_in_user = user_stack[user_stack.size()-1] ;
    if ( log_in_user.privilege < limit ){
        throw invalid_argument("check_privilege: without permission") ;
    }
}

void command::check_selected()
{
    if ( select_stack[select_stack.size()-1] == -1 ) throw invalid_argument("no selected book") ;
}

int command::select_offset()
{
    return select_stack[select_stack.size()-1] ;
}

void command::check_num()
{
    int p = command_stream.tellg() ;
    string temp ;
    command_stream >> temp ;
    for ( int i = 0 ; i < temp.length() ; i++ ){
        if ( ( temp[i] < '0' || temp[i] > '9' ) && temp[i] != '.' ) throw invalid_argument("check_num: need number") ;
    }
    command_stream.clear() ;
    command_stream.seekg(p) ;
}

void command::SU()
{
    check_privilege(0) ;
    List user_list(USER_ID_FILE) ;
    user temp_user ;
    if ( !has_more_token() ) throw invalid_argument("su: need user_id") ;

    char id[31] = {0} , pw[31] = {0} ;
    command_stream >> id ;
    element id_element(id,id,0) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = user_list.search_key(USER_ID_TYPE,id_element) ;
    if ( search_pos.first == 0 ) throw invalid_argument("su: no such user") ;

    int offset = user_list.get_key(search_pos) ;
    temp_user.get_user(offset) ;

    if ( !has_more_token() ){
        check_privilege(7) ;
        user_stack.push_back(temp_user) ;
        select_stack.push_back(-1) ;
        return ;
    }
    else {
        command_stream >> pw ;
        if ( !temp_user.right_password(pw) ) throw invalid_argument("su: wrong password") ;
        user_stack.push_back(temp_user) ;
        select_stack.push_back(-1) ;
    }
}

void command::LOGOUT()
{
    check_privilege(1) ;
    user_stack.pop_back() ;
    select_stack.pop_back() ;
}

void command::USERADD()
{
    check_privilege(3) ;
    char id[31] = {0} , pw[31] = {0} , n[31] = {0} ;
    int p = 0 , offset = -1 ;
    List user_list(USER_ID_FILE) ;

    if ( !has_more_token() ) throw invalid_argument("useradd: need id") ; command_stream >> id ;
    if ( !has_more_token() ) throw invalid_argument("useradd: need pw") ; command_stream >> pw ;
    if ( !has_more_token() ) throw invalid_argument("useradd: need p") ; check_num() ; command_stream >> p ;
    if ( !has_more_token() ) throw invalid_argument("useradd: need n") ; command_stream >> n ;
    check_privilege(p+1) ; if ( p == 0 ) throw invalid_argument("useradd: no zero privilege") ;

    element id_element( id , id , 0 ) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = user_list.search_key(USER_ID_TYPE,id_element) ;
    if ( search_pos.first == 1 ) throw invalid_argument("useradd: existing user") ;

    user new_user(id,pw,n,p) ;
    id_element.offset = add_user(new_user) ;
    user_list.add_key( id_element , search_pos ) ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
}

void command::REGISTER()
{
    char id[31] = {0} , pw[31] = {0} , n[31] = {0} ;
    List user_list(USER_ID_FILE) ;

    if ( !has_more_token() ) throw invalid_argument("register: need id") ; command_stream >> id ;
    if ( !has_more_token() ) throw invalid_argument("register: need pw") ; command_stream >> pw ;
    if ( !has_more_token() ) throw invalid_argument("register: need n") ; command_stream >> n ;
    if ( has_more_token() ) throw invalid_argument("register: too many token") ;

    element id_element( id , id , 0 ) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = user_list.search_key(USER_ID_TYPE,id_element) ;
    if ( search_pos.first == 1 ) throw invalid_argument("register: existing user") ;

    user new_user(id,pw,n,1) ;
    id_element.offset = add_user(new_user) ;
    user_list.add_key(id_element,search_pos) ;
}

void command::DELETE()
{
    check_privilege(7) ;
    List user_list(USER_ID_FILE) ;
    char id[31] = {0};
    if ( !has_more_token() ) throw invalid_argument("delete: need id") ; command_stream >> id ;

    user temp_user(id) ;
    for ( int i = 0 ; i < user_stack.size() ; i++ ){
        if ( temp_user == user_stack[i] ) throw invalid_argument("delete: logging user") ;
    }

    element id_element( id , id , 0 ) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = user_list.search_key( USER_ID_TYPE , id_element ) ;
    if ( search_pos.first == 0 ) throw invalid_argument("delete: no existing user") ;

    user_list.del_key(search_pos) ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
}

void command::PASSWD()
{
    check_privilege(1) ;
    List user_list(USER_ID_FILE) ;
    char id[31] = {0} , pw[31] = {0} , n_pw[31] = {0} ;
    if ( !has_more_token() ) throw invalid_argument("passwd: need id") ; command_stream >> id ;
    if ( !has_more_token() ) throw invalid_argument("passwd: need pw") ; command_stream >> pw ;
    if ( !has_more_token() ) {
        check_privilege(7) ;
        strcpy( n_pw , pw ) ;
    } command_stream >> n_pw ;

    element id_element( id , id , 0 ) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = user_list.search_key( USER_ID_TYPE , id_element ) ;
    if ( search_pos.first == 0 ) throw invalid_argument("passwd: no existing user") ;

    int offset = user_list.get_key(search_pos) ;
    user temp_user ;
    temp_user.get_user(offset) ;

    if ( !temp_user.right_password(pw) ){ // 密码不对，判断是否为老板账号
        check_privilege(7) ;
    }

    strcpy( temp_user.passwd , n_pw ) ;
    temp_user.put_user(offset) ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
}

void command::SELECT()
{
    check_privilege(3) ;
    char isbn[61] = {0} ;
    if ( !has_more_token() ) throw invalid_argument("select: need ISBN") ; command_stream >> isbn ;

    element isbn_element( isbn , isbn , 0 ) ; pair< bool , pair<int,int> > search_pos ;
    List book_list(ISBN_FILE) ;
    search_pos = book_list.search_key( ISBN_TYPE , isbn_element ) ;

    book temp_book(isbn) ;
    if ( search_pos.first == 0 ){
        isbn_element.offset = add_book(temp_book) ;
        book_list.add_key(isbn_element,search_pos) ;
    }
    int offset = book_list.get_key(search_pos) ;
    select_stack[select_stack.size()-1] = offset ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
}

void command::del_name()
{
    book temp_book ;
    List name_list(NAME_FILE) ;
    temp_book.get_book(select_offset()) ;
    element name_element(temp_book.name,temp_book.ISBN,0) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = name_list.search_key(NAME_TYPE,name_element) ;
    name_list.del_key(search_pos) ;
}

void command::del_author()
{
    book temp_book ;
    List author_list(AUTHOR_FILE) ;
    temp_book.get_book(select_offset()) ;
    element author_element( temp_book.author , temp_book.ISBN , 0 ) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = author_list.search_key(AUTHOR_TYPE,author_element) ;
    author_list.del_key(search_pos) ;
}

void command::del_keyword()
{
    book temp_book ;
    List keyword_list(KEYWORD_FILE) ;
    temp_book.get_book(select_offset()) ;
    string keyword_sequence = temp_book.keyword ;

    int counter = get_strip( keyword_sequence ) ;
    stringstream temp_stream , change_to_char_stream ;
    element keyword_element(temp_book.keyword,temp_book.ISBN,0) ; pair< bool , pair<int,int> > search_pos ;
    temp_stream << keyword_sequence ;

    for ( int i = 0 ; i < counter ; i++ ){
        getline(temp_stream,keyword_sequence,'|') ;
        change_to_char_stream.str("") ;
        change_to_char_stream.clear() ;
        change_to_char_stream << keyword_sequence ;
        change_to_char_stream >> keyword_element.keyword_main ;
        search_pos = keyword_list.search_key(KEYWORD_TYPE,keyword_element) ;
        keyword_list.del_key(search_pos) ;
    }

    temp_stream >> keyword_element.keyword_main ;
    search_pos = keyword_list.search_key(KEYWORD_TYPE,keyword_element) ;
    keyword_list.del_key(search_pos) ;
}

void command::del_ISBN()
{
    book temp_book ;
    List ISBN_list(ISBN_FILE) ;
    temp_book.get_book(select_offset()) ;
    element ISBN_element(temp_book.ISBN,temp_book.ISBN,0) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = ISBN_list.search_key(ISBN_TYPE,ISBN_element) ;
    ISBN_list.del_key(search_pos) ;
}

void command::add_ISBN()
{
    book temp_book ;
    List ISBN_list(ISBN_FILE) ;
    temp_book.get_book(select_offset()) ;
    element ISBN_element(temp_book.ISBN,temp_book.ISBN,select_offset()) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = ISBN_list.search_key(ISBN_TYPE,ISBN_element) ;
    ISBN_list.add_key(ISBN_element,search_pos) ;
}

void command::add_name()
{
    book temp_book ;
    List name_list(NAME_FILE) ;
    temp_book.get_book(select_offset()) ;
    element name_element(temp_book.name,temp_book.ISBN,select_offset()) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = name_list.search_key(NAME_TYPE,name_element) ;
    name_list.add_key(name_element,search_pos) ;
}

void command::add_author()
{
    book temp_book ;
    List author_list(AUTHOR_FILE) ;
    temp_book.get_book(select_offset()) ;
    element author_element(temp_book.author,temp_book.ISBN,select_offset()) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = author_list.search_key(AUTHOR_TYPE,author_element) ;
    author_list.add_key(author_element,search_pos) ;
}

void command::add_keyword()
{
    book temp_book ;
    List keyword_list(KEYWORD_FILE) ;
    temp_book.get_book(select_offset()) ;
    string keyword_sequence = temp_book.keyword ;

    int counter = get_strip(keyword_sequence) ;
    stringstream temp_stream , change_to_char_stream ;
    element keyword_element(temp_book.keyword,temp_book.ISBN,select_offset()) ; pair< bool , pair<int,int> > search_pos ;
    temp_stream << keyword_sequence ;

    for ( int i = 0 ; i < counter ; i++ ){
        getline(temp_stream,keyword_sequence,'|') ;
        change_to_char_stream.str("") ;
        change_to_char_stream.clear() ;
        change_to_char_stream << keyword_sequence ;
        change_to_char_stream >> keyword_element.keyword_main ;
        search_pos = keyword_list.search_key(KEYWORD_TYPE,keyword_element) ;
        keyword_list.add_key(keyword_element,search_pos) ;
    }
    temp_stream >> keyword_element.keyword_main ;
    search_pos = keyword_list.search_key(KEYWORD_TYPE,keyword_element) ;
    keyword_list.add_key(keyword_element,search_pos) ;
}

void command::check_existing_ISBN(book temp_book)
{
    element isbn_element(temp_book.ISBN,temp_book.ISBN,0) ; pair< bool , pair<int,int> > search_pos ;
    List ISBN_list(ISBN_FILE) ;
    search_pos = ISBN_list.search_key(ISBN_TYPE,isbn_element) ;
    if ( search_pos.first == 1 ) throw invalid_argument("modify: modify existing ISBN") ;
}

void command::check_repeated_keyword(string line)
{
    vector<string> checker ;
    stringstream temp_stream ;
    string token ;
    temp_stream << line ;
    int counter = get_strip(line) ;
    if ( counter == 0 ) return ;

    for ( int i = 0 ; i < counter ; i++ ){
        getline(temp_stream,token,'|') ;
        for ( int j = 0 ; j < checker.size() ; j++ ){
            if ( token == checker[j] ) throw invalid_argument("check_repeated_keyword: repeated keyword") ;
        }
        checker.push_back(token) ;
    } temp_stream >> token ;
    for ( int i = 0 ; i < checker.size() ; i++ ) if ( token == checker[i] ) throw invalid_argument("check_repeated_keyword: repeated keyword") ;
}

void command::MODIFY()
{
    check_selected() ;
    bool isbn_m = false , name_m = false , author_m = false , keyword_m = false , price_m = false ;
    string line ;
    stringstream temp_stream ;
    book temp_book , real_book ;
    while (has_more_token()){
        command_stream >> line ;
        temp_stream.str("") ; temp_stream.clear() ;
        switch (StringToKey(line)) {
            case ISBN_TYPE: isbn_m = true ; temp_stream << line ; temp_stream >> temp_book.ISBN ; break ;
            case NAME_TYPE: name_m = true ; temp_stream << line ; temp_stream >> temp_book.name ; break ;
            case AUTHOR_TYPE: author_m = true ; temp_stream << line ; temp_stream >> temp_book.author ; break ;
            case KEYWORD_TYPE: keyword_m = true ; temp_stream << line ; temp_stream >> temp_book.keyword ; break ;
            case PRICE_TYPE: price_m = true ; temp_stream << line ; temp_stream >> temp_book.price ; break ;
        }
    }
    real_book.get_book(select_offset()) ;
    temp_book.quantity = real_book.quantity ;
    if ( keyword_m ) check_repeated_keyword(temp_book.keyword) ;

    if ( !isbn_m ) strcpy( temp_book.ISBN , real_book.ISBN ) ;
    if ( !name_m ) strcpy( temp_book.name , real_book.name ) ;
    if ( !author_m ) strcpy( temp_book.author , real_book.author ) ;
    if ( !keyword_m ) strcpy( temp_book.keyword , real_book.keyword ) ;
    if ( !price_m ) temp_book.price = real_book.price ;

    if ( isbn_m ){
        this->check_existing_ISBN(temp_book) ;
        del_ISBN() ;
        if ( real_book.has_author() ) del_author() ;
        if ( real_book.has_name() ) del_name() ;
        if ( real_book.has_keyword() ) del_keyword() ;
    }else{
        if ( real_book.has_name() && name_m )  del_name() ;
        if ( real_book.has_author() && author_m ) del_author() ;
        if ( real_book.has_keyword() && keyword_m ) del_keyword() ;
    }

    temp_book.put_book(select_offset()) ;

    if ( isbn_m ){
        add_ISBN() ;
        if ( temp_book.has_name() ) add_name() ;
        if ( temp_book.has_author() ) add_author() ;
        if ( temp_book.has_keyword() ) add_keyword() ;
    }else{
        if ( name_m ) add_name() ;
        if ( author_m ) add_author() ;
        if ( keyword_m ) add_keyword() ;
    }
    add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
}

void command::IMPORT()
{
    check_selected() ;
    book temp_book ;
    temp_book.get_book(select_offset()) ;
    int import_quantity = 0 ;
    double all_cost = 0 ;
    if ( !has_more_token() ) throw invalid_argument("import: need quantity") ; check_num() ; command_stream >> import_quantity ;
    if ( !has_more_token() ) throw invalid_argument("import: need cost") ; check_num() ; command_stream >> all_cost ;
    temp_book.quantity += import_quantity ;
    temp_book.put_book(select_offset()) ;
    change_finance( -all_cost ) ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,true) ;
}

void command::BUY()
{
    check_privilege(1) ;
    book temp_book ;
    List ISBN_list(ISBN_FILE) ;
    int all_quantity = 0 ;
    if ( !has_more_token() ) throw invalid_argument("buy: need isbn") ; command_stream >> temp_book.ISBN ;
    if ( !has_more_token() ) throw invalid_argument("buy: need quantity") ; check_num() ; command_stream >> all_quantity ;
    element ISBN_element(temp_book.ISBN,temp_book.ISBN,0) ; pair< bool , pair<int,int> > search_pos ;
    search_pos = ISBN_list.search_key(ISBN_TYPE,ISBN_element) ;
    if ( search_pos.first == 0 ) throw invalid_argument("buy: no such book") ;

    int offset = ISBN_list.get_key(search_pos) ;
    temp_book.get_book(offset) ;
    if ( temp_book.quantity < all_quantity ) throw invalid_argument("buy: no so many books") ;
    temp_book.quantity -= all_quantity ;
    temp_book.put_book(offset) ;
    change_finance( temp_book.price * all_quantity ) ;
    printf( "%.2f\n" , temp_book.price * all_quantity ) ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,true) ;
}

void command::SHOW()
{
    check_privilege(1) ;
    if ( !has_more_token() ){
        big_show() ;
        add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
        return ;
    }
    string line ;
    command_stream >> line ;

    if ( line == "finance" ){
    	check_privilege(7) ;
        int times = -1 ;
        if ( has_more_token() ) command_stream >> times ;
        print_finance(times) ;
        add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
        return ;
    }
    if ( has_more_token() ) throw invalid_argument("show: too many requests") ;

    key_type KeyType = StringToKey(line) ;
    char k[61] = {0} ;
    stringstream temp_stream ;
    temp_stream << line ;
    temp_stream >> k ;

    List real_list(get_file_name(KeyType)) ;
    real_list.show_key( KeyType , k ) ;
    add_manipulation(user_stack[user_stack.size()-1],command_stream,false) ;
}

void command::REPORT()
{
    string line ;
    if (!has_more_token()) throw invalid_argument("report: need command") ;
    command_stream >> line ;
    if ( line == "finance" ){
        check_privilege(7) ;
        show_all_finance_log() ;
    }
    if ( line == "employee" ){
        check_privilege(7) ;
        employee_show() ;
    }
    if ( line == "myself" ){
        check_privilege(3) ;
        myself_show(user_stack[user_stack.size()-1]) ;
    }
}

void command::LOG()
{
    check_privilege(7) ;
    log_show() ;
}

int get_strip( string line )
{
    int counter = 0 ;
    for ( int i = 0 ; i < line.length() ; i++ ){
        if ( line[i] == '|' ) counter++ ;
    }
    return counter ;
}

key_type StringToKey ( string &line )
{
    stringstream temp_stream ;
    string temp_line = line ;
    key_type real_type ;
    temp_stream << line ;
    getline( temp_stream , temp_line , '=' ) ;
    if ( temp_line == "-ISBN" ) {
        real_type = ISBN_TYPE ;
        temp_stream.seekg(6,ios::beg) ;
        temp_stream >> line ;
    }else if ( temp_line == "-name" ){
        real_type = NAME_TYPE ;
        temp_stream.seekg(7,ios::beg) ;
    }else if ( temp_line == "-author" ){
        real_type = AUTHOR_TYPE ;
        temp_stream.seekg(9,ios::beg) ;
    }else if ( temp_line == "-keyword" ){
        real_type = KEYWORD_TYPE ;
        temp_stream.seekg(10,ios::beg) ;
    }else if ( temp_line == "-price" ){
        real_type = PRICE_TYPE ;
        temp_stream.seekg(7,ios::beg) ;
        temp_stream >> line ;
    }
    else { throw invalid_argument("StringToKey: wrong token") ; }
    if ( real_type == ISBN_TYPE || real_type == PRICE_TYPE ) return real_type ;
    temp_stream >> line ;
    line[line.length()-1] = 0 ;
    return real_type ;
}











