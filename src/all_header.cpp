#include "all_header.h"
using namespace std ;


user::user() {}

user::user(const char *u)
{
    strcpy( user_id , u ) ;
}

user::user(const char *u, const char *pa, const char *n, int pr)
{
    strcpy( user_id , u ) ;
    strcpy( passwd , pa ) ;
    strcpy( name , n ) ;
    privilege = pr ;
}

user::user(const user &other)
{
    strcpy( user_id , other.user_id ) ;
    strcpy( passwd , other.passwd ) ;
    strcpy( name , other.name ) ;
    privilege = other.privilege ;
}

user &user::operator=(const user &other)
{
    if ( this == &other ) return *this ;
    strcpy( user_id , other.user_id ) ;
    strcpy( passwd , other.passwd ) ;
    strcpy( name , other.name ) ;
    privilege = other.privilege ;
    return *this ;
}

void user::get_user(int offset)
{
    fstream user_file( USER_FILE , ios::in | ios::binary ) ;
    if ( !user_file ) cerr << "get_user: fail to open file" << endl ;
    user_file.seekg( offset * sizeof(user) ) ;
    user_file.read( reinterpret_cast<char*>(this) , sizeof(user) ) ;
    user_file.close() ;
}

void user::put_user(int offset)
{
    fstream user_file( USER_FILE , ios::in|ios::out | ios::binary ) ;
    if ( !user_file ) cerr << "put_user: fail to open file" << endl ;
    user_file.seekp( offset * sizeof(user) ) ;
    user_file.write( reinterpret_cast<char*>(this) , sizeof(user) ) ;
    user_file.close() ;
}

bool user::right_password(const char *pw)
{
    return ( strcmp( passwd , pw ) == 0 ) ;
}

bool user::operator==(const user &other)
{
    return ( strcmp( user_id , other.user_id ) == 0 ) ;
}


book::book(){ }

book::book(char *isbn)
{
    strcpy( ISBN , isbn ) ;
}

book::book(char *isbn, double p, char *n, char *a, char *k)
{
    strcpy( ISBN , isbn ) ;
    price = p ;
    strcpy( name , n ) ;
    strcpy( author , a ) ;
    strcpy( keyword , k ) ;
}

book::book(const book &other)
{
    strcpy( ISBN , other.ISBN ) ;
    price = other.price ;
    strcpy( name , other.name ) ;
    strcpy( author , other.author ) ;
    strcpy( keyword , other.keyword ) ;
}

book &book::operator=(const book &other)
{
    if ( this == &other ) return *this ;
    strcpy( ISBN , other.ISBN ) ;
    price = other.price ;
    strcpy( name , other.name ) ;
    strcpy( author , other.author ) ;
    strcpy( keyword , other.keyword ) ;
    return *this ;
}

void book::get_book(int offset)
{
    fstream book_file(BOOK_FILE,ios::in|ios::binary) ;
    if ( !book_file ) cerr << "get_book: fail to open file" << endl ;
    book_file.seekg( offset * sizeof(book) ) ;
    book_file.read( reinterpret_cast<char*>(this) , sizeof(book) ) ;
    book_file.close() ;
}

void book::print_book()
{
    cout << ISBN << "\t" << name << "\t" << author << "\t" << keyword << "\t" ;
    printf( "%.2f" , price ) ;
    cout << "\t" << quantity << endl ;
}

void book::put_book(int offset)
{
    fstream book_file( BOOK_FILE , ios::in|ios::out|ios::binary ) ;
    if (!book_file) cerr << "put_book: fail to open file" << endl ;
    book_file.seekp( offset * sizeof(book) ) ;
    book_file.write( reinterpret_cast<char*>(this) , sizeof(book) ) ;
    book_file.close() ;
}

bool book::has_name()
{
    if ( name[0] != 0 ) return true ;
    else return false ;
}

bool book::has_author()
{
    if ( author[0] != 0 ) return true ;
    else return false ;
}

bool book::has_keyword()
{
    if ( keyword[0] != 0 ) return true ;
    else return false ;
}


element::element() { }

element::element( const char *k1, const char *k2, int os)
{
    strcpy( keyword_main , k1 ) ;
    strcpy( keyword_auxiliary , k2 ) ;
    offset = os ;
}

element::element(const element &other)
{
    strcpy( keyword_main , other.keyword_main ) ;
    strcpy( keyword_auxiliary , other.keyword_auxiliary ) ;
    offset = other.offset ;
}

element &element::operator=(const element &other)
{
    if ( this == &other ) return *this ;
    strcpy( keyword_main , other.keyword_main ) ;
    strcpy( keyword_auxiliary , other.keyword_auxiliary ) ;
    offset = other.offset ;
    return *this ;
}

bool element::operator<(const element &other)
{
    if ( strcmp( keyword_main , other.keyword_main ) < 0 ) return true ;
    if ( strcmp( keyword_main , other.keyword_main ) == 0 ){
        return ( strcmp( keyword_auxiliary , other.keyword_auxiliary ) < 0 ) ;
    }else return false ;
}

bool element::operator<(const block &other)
{
    if ( strcmp( keyword_main , other.data[0].keyword_main ) < 0 ) return true ;
    if ( strcmp( keyword_main , other.data[0].keyword_main ) == 0 ){
        return ( strcmp( keyword_auxiliary , other.data[0].keyword_auxiliary ) < 0 ) ;
    }else return false ;
}

bool element::operator==(const element &other)
{
    return ( strcmp( keyword_auxiliary , other.keyword_auxiliary ) == 0 && strcmp( keyword_main , other.keyword_main ) == 0 ) ;
}

bool element::operator==( const block &other )
{
    return ( strcmp( keyword_auxiliary , other.data[0].keyword_auxiliary ) == 0 && strcmp( keyword_main , other.data[0].keyword_main ) == 0 ) ;
}

bool element::operator>(const element &other)
{
    if ( *this == other ) return false ;
    if ( *this < other ) return false ;
    return true ;
}

bool element::equal_with(const element &other)
{
    return ( strcmp( keyword_main , other.keyword_main ) == 0 ) ;
}


void block::get_block(key_type ElementType, int offset)
{
    char *file_name = get_file_name( ElementType ) ;
    fstream file( file_name , ios::in | ios::binary ) ;
    if ( !file ) cerr << "get_block: fail to open file" << endl ;
    file.seekg( offset * sizeof(block) ) ;
    file.read( reinterpret_cast<char*>(this) , sizeof(block) ) ;
    file.close() ;
}

void block::put_block(key_type ElementType, int offset)
{
    char *file_name = get_file_name( ElementType ) ;
    fstream file( file_name , ios::in|ios::out | ios::binary ) ;
    if ( !file ) cerr << "put_block: fail to open file" << endl ;
    file.seekp( offset * sizeof(block) ) ;
    file.write( reinterpret_cast<char*>(this) , sizeof(block) ) ;
    file.close() ;
}

char* get_file_name( key_type KeyType )
{
    char *file_name ;
    file_name = new char [30] ;
    switch (KeyType) {

        case USER_ID_TYPE: strcpy( file_name , USER_ID_FILE ) ; break ;

        case ISBN_TYPE: strcpy( file_name , ISBN_FILE ) ; break ;

        case NAME_TYPE: strcpy( file_name , NAME_FILE ) ; break ;

        case AUTHOR_TYPE: strcpy( file_name , AUTHOR_FILE ) ; break ;

        case KEYWORD_TYPE: strcpy( file_name , KEYWORD_FILE ) ; break ;

    }
    return file_name ;
}

key_type getKeyType( const char* filename )
{
    if ( strcmp( filename , USER_ID_FILE ) == 0 ) return USER_ID_TYPE ;
    if ( strcmp( filename , ISBN_FILE ) == 0 ) return ISBN_TYPE ;
    if ( strcmp( filename , NAME_FILE ) == 0 ) return NAME_TYPE ;
    if ( strcmp( filename , AUTHOR_FILE ) == 0 ) return AUTHOR_TYPE ;
    if ( strcmp( filename , KEYWORD_FILE ) == 0 ) return KEYWORD_TYPE ;

    throw invalid_argument("getKeyType: no such type") ;
}

int get_element_num( element_type ElementType )
{
    int element_num = 0 ;
    fstream core_file(CORE_FILE,ios::in|ios::binary) ;                          // core_file的顺序是 user_num book_num user_id_num ISBN_block_num
    if ( !core_file ) cerr << "get_element_num: fail to open file" << endl ;    // name_block_num author_block_num keyword_block_num finance_times
    if ( ElementType == USER_TYPE ){
        core_file.read( reinterpret_cast<char*>(&element_num) , sizeof(int) ) ;
    }else{
        core_file.seekg(sizeof(int)) ;
        core_file.read( reinterpret_cast<char*>(&element_num) , sizeof(int) ) ;
    }
    core_file.close() ;
    return element_num ;
}

void change_element_num( element_type ElementType , int num )
{
    fstream core_file(CORE_FILE,ios::in|ios::out|ios::binary) ;
    if ( !core_file ) cerr << "change_element_num: fail to open file" << endl ;
    if ( ElementType == USER_TYPE ){
        core_file.write( reinterpret_cast<char*>(&num) , sizeof(int) ) ;
    }else{
        core_file.seekp(sizeof(int)) ;
        core_file.write( reinterpret_cast<char*>(&num) , sizeof(int) ) ;
    }
    core_file.close() ;
}

int add_book(book &add_element)
{
    int book_num = get_element_num(BOOK_TYPE) ;
    fstream book_file(BOOK_FILE,ios::in|ios::out|ios::binary) ;
    if ( !book_file ) cerr << "add_book: fail to open file" << endl ;
    book_file.seekp( book_num * sizeof(book) ) ;
    book_file.write( reinterpret_cast<char*>(&add_element) , sizeof(book) ) ;
    change_element_num(BOOK_TYPE , ++book_num ) ;
    book_file.close() ;
    return book_num - 1 ;
}

int add_user(user &add_element)
{
    int user_num = get_element_num(USER_TYPE) ;
    fstream user_file(USER_FILE,ios::in|ios::out|ios::binary) ;  // 注意：不带ios::in会覆盖原有文件
    if ( !user_file ) cerr << "add_user: fail to open file" << endl ;
    user_file.seekp( user_num * sizeof(user) ) ;
    user_file.write( reinterpret_cast<char*>(&add_element) , sizeof(user) ) ;
    change_element_num( USER_TYPE , ++user_num ) ;
    user_file.close() ;
    return user_num - 1 ;
}

int get_block_num( key_type KeyType )
{
    int block_num = 0 ;
    fstream core_file(CORE_FILE,ios::in|ios::binary) ;
    if ( !core_file ) cerr << "get_block_num: fail to open file" << endl ;
    switch (KeyType){

        case USER_ID_TYPE : core_file.seekg( 2 * sizeof(int) ) ; break ;

        case ISBN_TYPE : core_file.seekg( 3 * sizeof(int) ) ; break ;

        case NAME_TYPE : core_file.seekg( 4 * sizeof(int) ) ; break ;

        case AUTHOR_TYPE : core_file.seekg( 5 * sizeof(int) ) ; break ;

        case KEYWORD_TYPE : core_file.seekg( 6 * sizeof(int) ) ; break ;
    }
    core_file.read( reinterpret_cast<char*>(&block_num) , sizeof(int) ) ;
    core_file.close() ;
    return block_num ;
}

void change_block_num( key_type KeyType , int num )
{
    fstream core_file(CORE_FILE,ios::in|ios::out|ios::binary) ;
    if (!core_file) cerr << "change_block_num: fail to open file" << endl ;
    switch (KeyType) {

        case USER_ID_TYPE : core_file.seekp( 2 * sizeof(int) ) ; break ;

        case ISBN_TYPE : core_file.seekp( 3 * sizeof(int) ) ; break ;

        case NAME_TYPE : core_file.seekp( 4 * sizeof(int) ) ; break ;

        case AUTHOR_TYPE : core_file.seekp( 5 * sizeof(int) ) ; break ;

        case KEYWORD_TYPE : core_file.seekp( 6 * sizeof(int) ) ; break ;
    }
    core_file.write( reinterpret_cast<char*>(&num) , sizeof(int) ) ;
    core_file.close() ;
}

int get_finance_times()
{
    int finance_times = 0 ;
    fstream core_file(CORE_FILE,ios::in|ios::binary) ;
    if (!core_file) cerr << "get_finance_times: fail to open file" << endl ;
    core_file.seekg(7* sizeof(int)) ;
    core_file.read( reinterpret_cast<char*>(&finance_times) , sizeof(int) ) ;
    core_file.close() ;
    return finance_times ;
}

void change_finance_times( int num )
{
    fstream core_file(CORE_FILE,ios::in|ios::out|ios::binary) ;
    if (!core_file) cerr << "change_finance_times: fail to open file" << endl ;
    core_file.seekp(7* sizeof(int)) ;
    core_file.write( reinterpret_cast<char*>(&num) , sizeof(int) ) ;
    core_file.close() ;
}

void change_finance( double flux )
{
    fstream finance_file(FINANCE_FILE,ios::in|ios::out|ios::binary) ;
    if (!finance_file) cerr << "change_finance: fail to open file" << endl ;
    int finance_times = get_finance_times() ;
    finance_file.seekp(finance_times* sizeof(double)) ;
    finance_file.write( reinterpret_cast<char*>(&flux) , sizeof(double) ) ;
    finance_file.close() ;
    change_finance_times(++finance_times) ;
}

void print_finance( int times )
{
    double income = 0 , output = 0 , temp = 0 ;
    int finance_times = get_finance_times() ;
    if ( times > finance_times ) throw invalid_argument("print_finance: ask more than record") ;
    if ( times == -1 ) times = finance_times ;
    fstream finance_file(FINANCE_FILE,ios::in|ios::binary) ;
    for ( int i = finance_times - times ; i < finance_times ; i++ ){
        finance_file.seekg(i* sizeof(double),ios::beg) ;
        finance_file.read( reinterpret_cast<char*>(&temp) , sizeof(double) ) ;
        if ( temp > 0 ) income += temp ;
        else output -= temp ;
    }
    printf( "+ %.2f - %.2f\n" , income , output ) ;
}

void big_show()
{
    block temp_block ;
    book temp_book ;
    temp_block.get_block(ISBN_TYPE,0) ;
    if ( temp_block.length == 0 ) cout << endl ; // 空show
    while (true){
        for ( int i = 0 ; i < temp_block.length ; i++ ){
            temp_book.get_book(temp_block.data[i].offset) ;
            temp_book.print_book() ;
        }
        if ( temp_block.down == -1 ) return ;
        temp_block.get_block(ISBN_TYPE,temp_block.down) ;
    }
}

void file_initialize()
{
    fstream temp_file(USER_FILE,ios::in|ios::out) ;
    if ( temp_file ){
        temp_file.close() ;
        return ;
    }
    ofstream user_file(USER_FILE,ios::binary) , book_file(BOOK_FILE,ios::binary) , core_file(CORE_FILE,ios::binary) , log_file(LOG_FILE,ios::binary) ;
    ofstream user_id_file(USER_ID_FILE,ios::binary) , ISBN_file(ISBN_FILE,ios::binary) ;
    ofstream name_file(NAME_FILE,ios::binary) , author_file(AUTHOR_FILE,ios::binary) ;
    ofstream keyword_file(KEYWORD_FILE,ios::binary) , finance_file(FINANCE_FILE,ios::binary) ;
    user_file.close() ; book_file.close() ; core_file.close() ; user_id_file.close() ; ISBN_file.close() ; log_file.close() ;
    name_file.close() ; author_file.close() ; keyword_file.close() ; finance_file.close() ;
    data_initialize() ;
}

void data_initialize()
{
    fstream core_file(CORE_FILE,ios::out|ios::binary) ;
    int num1 = 1 , num0 = 0 ;
    core_file.write( reinterpret_cast<char*>(&num1) , sizeof(int) ) ;  //user_num
    core_file.write( reinterpret_cast<char*>(&num0) , sizeof(int) ) ;  //book_num
    for ( int i = 0 ; i < 5 ; i++ ) core_file.write( reinterpret_cast<char*>(&num1) , sizeof(int) ) ; //block_num
    core_file.write( reinterpret_cast<char*>(&num0) , sizeof(int) ) ;  //finance_times
    block temp_block ;
    temp_block.pos = 0 ;
    temp_block.put_block(USER_ID_TYPE,0) ;
    temp_block.put_block(ISBN_TYPE,0) ;
    temp_block.put_block(NAME_TYPE,0) ;
    temp_block.put_block(AUTHOR_TYPE,0) ;
    temp_block.put_block(KEYWORD_TYPE,0) ;
    user root("root","sjtu","root",7) ;
    element id_element( "root","root",add_user(root)) ;
    temp_block.length = 1 ;
    temp_block.data[0] = id_element ;
    temp_block.put_block(USER_ID_TYPE,0) ;
    core_file.close() ;
}

void add_manipulation( user working_user , std::stringstream &c_stream , bool f )
{
    string temp_string = c_stream.str() ;
    work_record work(working_user,temp_string,f) ;
    work.add_work_record() ;
}

void log_show()
{
    fstream log_file(LOG_FILE,ios::in|ios::binary) ;
    if (!log_file) cerr << "log_show: fail to open file" << endl ;
    work_record temp ;
    while ( log_file ){
        log_file.read( reinterpret_cast<char*>(&temp) , sizeof(work_record) ) ;
        temp.print_work_record() ;
    }
    log_file.close() ;
}

void employee_show()
{
    fstream log_file(LOG_FILE,ios::in|ios::binary) ;
    if (!log_file) cerr << "employee_show: fail to open file" << endl ;
    work_record temp ;
    while ( log_file ){
        log_file.read( reinterpret_cast<char*>(&temp) , sizeof(work_record) ) ;
        if ( temp.working_user.privilege >= 3 && temp.finance == false ){
            temp.print_work_record() ;
        }
    }
    log_file.close() ;
}

void myself_show( user worker )
{
    fstream log_file(LOG_FILE,ios::in|ios::binary) ;
    if (!log_file) cerr << "myself_show: fail to open file" << endl ;
    work_record temp ;
    while (log_file){
        log_file.read( reinterpret_cast<char*>(&temp) , sizeof(work_record) ) ;
        if ( temp.working_user == worker ){
            temp.print_work_record() ;
        }
    }
    log_file.close() ;
}

void show_all_finance_log()
{
    fstream log_file(LOG_FILE,ios::in|ios::binary) ;
    if (!log_file) cerr << "show_all_finance_log: fail to open file" << endl ;
    work_record temp ;
    while (log_file){
        log_file.read( reinterpret_cast<char*>(&temp) , sizeof(work_record) ) ;
        if (temp.finance){
            temp.print_work_record() ;
        }
    }
    log_file.close() ;
}

work_record::work_record() {}

work_record::work_record(user u, std::string m,bool f)
{
    working_user = u ;
    for ( int i = 0 ; i < m.length() ; i++ ){
        manipulation[i] = m[i] ;
    }
    finance = f ;
}

void work_record::add_work_record()
{
    fstream log_file(LOG_FILE,ios::in|ios::out|ios::binary) ;
    if ( !log_file ) cerr << "add_work_record: fail to open file" << endl ;
    log_file.seekp(0,ios::end) ;
    log_file.write( reinterpret_cast<char*>(this) , sizeof(work_record) ) ;
    log_file.close() ;
}

void work_record::print_work_record()
{
    cout << working_user.user_id << ":\t" << manipulation << endl ;
}


