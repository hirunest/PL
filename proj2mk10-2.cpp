# include <iostream>
# include <string>
# include <stdio.h>
# include <vector>
# include <sstream>
# include <stack>
# include <map>
# include <iomanip>
# include <cstdlib>
# include <cmath>
# include <sstream>

using namespace std ;
string gtestnum = "" ;
bool ge = false;
int gspa = 0;
bool gp = true;

enum TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  INT,
  STRING,
  DOT,
  FLOAT,
  NIL,
  T,
  QUOTE,
  SYMBOL,
  SEXP,
  TREE,
  HOLLOW,
  ERROR,
  RES,
  LIST,
  EXIT
}; // enum TokenType

# define Resnum 38

map<string,bool> gte;

string gRes[Resnum] = { "cons", "car", "cdr", "list", "define",
  "+", "-", "*", "/", "quote", "pair?", "clean-environment", "null?",
  "integer?", "real?", "number?", "string?", "boolean?", "symbol?",
  "not", ">", "<", "string-append", "string>?", "string<?",
  "string=?", "eqv?", "equal?", "if", "cond", "begin",
  "and", "or", "exit", "=", "<=", ">=", "atom?" }; 

struct TreeNode {
  string name ;
  int type ;
  TreeNode * left ;
  TreeNode * right ;
  bool con ;
  bool e;
}; // struct TreeNode

map<string,TreeNode*> gResmap ;

void CrRmap() {
  for ( int i = 0 ; i < Resnum ; i++ ) {
    TreeNode *t = new TreeNode;
    t->left = NULL;
    t->right = NULL;
    t->con = false;
    t->name = gRes[i];
    t->type = RES;
    t->e = false;
    gResmap[gRes[i]] = t ; 
  } // for

} // CrRmap()

struct InstructionNode {
  string tokenname ;
  int tokentype ;
  int column ;
  int line ;
  int resetcolumn ;
  InstructionNode* next ;
}; // struct InstructionNode

struct IntNode {
  int value ;
  IntNode* next ;
} ; // IntNode

class Vectorint {
  public:
  IntNode* mhead ;
  IntNode* mtail ;
  int mindex ;
  Vectorint( ) { 
    mindex = 0 ;
    mtail = NULL ;
    mhead = NULL ;
  }  // Vectorint()

  ~Vectorint() { }

  void Push_back( int newnode ) {
    if ( mhead == NULL ) {
      mhead = new IntNode ;
      mhead -> next = NULL ;
      mhead -> value = newnode ;
      mtail = mhead ;
    } // if
    else {  
      mtail -> next = new IntNode ;
      mtail = mtail -> next ;
      mtail -> next = NULL ;
      mtail -> value = newnode ;
    } // else

    mindex++ ;
  } // Push_back()

  int Size() {
    return mindex ;
  } // Size()

  int Begin() {
    return 0 ;
  } // Begin()

  IntNode* Access( int index ) {
    int count = 0 ;
    IntNode* temp = mhead ;
    while ( count < index && temp != NULL ) {
      temp = temp -> next ;
      count ++ ;
    } // while

    return temp ;
  } // Access()

  bool Insert( int insertindex, int insertnode ) {
    IntNode* newnode = new IntNode ;
    newnode -> next = NULL ;
    newnode -> value = insertnode ;
    if ( mhead == NULL ) {
      mhead = newnode ;
      mtail = newnode ;
      mindex++ ;
      return true ;
    } // if
    else {
      if ( insertindex == 0 ) { // insert head
        newnode -> next = mhead ;
        mhead = newnode ;
      } // if
      else if ( insertindex == mindex ) {
        mtail -> next = newnode ;
        mtail = mtail -> next ;
      } // else if
      else { //  ( insertindex <= mindex ) 
        if ( insertindex < mindex ) {
          int count = 0 ;
          IntNode* nodebeinsertindex = mhead ;
          while ( count < insertindex - 1 ) {
            nodebeinsertindex = nodebeinsertindex -> next ;
            count++ ;
          } // while
  
          newnode -> next = nodebeinsertindex -> next ;
          nodebeinsertindex -> next = newnode ;
        } // if 
        else {
          cout << "error insert out of index " << endl ;
          return false ;
        } // else  
      } // else
  
      mindex++ ;
      return true ;
    } // else
  } // Insert()

  bool Erase( int deleteindex ) {
    if ( deleteindex >=  Size() ) {
      return false ;
    } // if
    else if ( deleteindex == 0 && Size() >= 2 ) {
      IntNode* thedeletenode = mhead ;
      mhead = mhead -> next ;
      delete thedeletenode ;
    } // else if
    else if ( deleteindex == 0 && Size() == 1 ) {
      delete mhead ;
      mhead = NULL ;
      mtail = NULL ;
    } // else if
    else { //  ( Size() >= 2 ) 
      int count = 0 ;
      IntNode* thenodebedel = mhead ;
      while ( count < deleteindex - 1 ) {
        thenodebedel = thenodebedel -> next ;
        count ++ ;
      } // while

      IntNode* thedeletenode = thenodebedel -> next ;
      thenodebedel -> next = thenodebedel -> next -> next ;
      if ( deleteindex == Size() - 1 ) {

        mtail = thenodebedel ;
      } // if

      delete thedeletenode ;
    } // else 
    

    mindex -- ;
    return true ;
  } // Erase()

  bool Clear() {
    int i = 0 ;
    IntNode* temp = mhead ;
    IntNode* temp2 = mhead ;
    while ( i < mindex ) {
      temp2 = temp -> next ;
      temp -> next = NULL ;
      delete temp ;
      temp = NULL ;
      temp = temp2 ;
      i++ ;
    } // while

    mhead = NULL ;
    mtail = NULL ;
    mindex = 0 ;
    return true ;
  } // Clear()

} ; // Vectorint

class Vector {
  public:
  InstructionNode* mhead ;
  InstructionNode* mtail ;
  int mindex ;
  Vector( ) { 
    mindex = 0 ;
    mtail = NULL ;
    mhead = NULL ;
  }  // Vector()

  ~Vector() { }

  void Push_back( InstructionNode newnode ) {
    if ( mhead == NULL ) {
      mhead = new InstructionNode ;
      mhead -> next = NULL ;
      mhead -> tokenname = newnode.tokenname ;
      mhead -> tokentype = newnode.tokentype ;
      mhead -> line = newnode.line ;
      mhead -> column = newnode.column ;
      mhead -> resetcolumn = newnode.resetcolumn ;
      mtail = mhead ;
    } // if
    else {  
      mtail -> next = new InstructionNode ;
      mtail = mtail -> next ;
      mtail -> next = NULL ;
      mtail -> tokenname = newnode.tokenname ;
      mtail -> tokentype = newnode.tokentype ;
      mtail -> line = newnode.line ;
      mtail -> column = newnode.column ;
      mtail -> resetcolumn = newnode.resetcolumn ;
    } // else

    mindex++ ;
  } // Push_back()

  int Size() {
    return mindex ;
  } // Size()

  int Begin() {
    return 0 ;
  } // Begin()

  InstructionNode* Access( int index ) {
    int count = 0 ;
    InstructionNode* temp = mhead ;
    while ( count < index && temp != NULL ) {
      temp = temp -> next ;
      count ++ ;
    } // while

    return temp ;
  } // Access()

  bool Insert( int insertindex, InstructionNode insertnode ) {
    InstructionNode* newnode = new InstructionNode ;
    newnode -> next = NULL ;
    newnode -> tokenname = insertnode.tokenname ;
    newnode -> tokentype = insertnode.tokentype ;
    newnode -> column = insertnode.column ;
    newnode -> line = insertnode.line ;
    newnode -> resetcolumn = insertnode.resetcolumn ;
    if ( mhead == NULL ) {
      mhead = newnode ;
      mtail = newnode ;
      mindex++ ;
      return true ;
    } // if
    else {
      if ( insertindex == 0 ) { // insert head
        newnode -> next = mhead ;
        mhead = newnode ;
      } // if
      else if ( insertindex == mindex ) {
        mtail -> next = newnode ;
        mtail = mtail -> next ;
      } // else if
      else { //  ( insertindex <= mindex ) 
        if ( insertindex < mindex ) {
          int count = 0 ;
          InstructionNode* nodebeinsertindex = mhead ;
          while ( count < insertindex - 1 ) {
            nodebeinsertindex = nodebeinsertindex -> next ;
            count++ ;
          } // while
  
          newnode -> next = nodebeinsertindex -> next ;
          nodebeinsertindex -> next = newnode ;
        } // if 
        else {
          cout << "error insert out of index " << endl ;
          return false ;
        } // else  
      } // else
  
      mindex++ ;
      return true ;
    } // else
  } // Insert()

  bool Erase( int deleteindex ) {
    if ( deleteindex >=  Size() ) {
      return false ;
    } // if
    else if ( deleteindex == 0 && Size() >= 2 ) {
      InstructionNode* thedeletenode = mhead ;
      mhead = mhead -> next ;
      delete thedeletenode ;
    } // else if
    else if ( deleteindex == 0 && Size() == 1 ) {
      delete mhead ;
      mhead = NULL ;
      mtail = NULL ;
    } // else if
    else { //  ( Size() >= 2 ) 
      int count = 0 ;
      InstructionNode* thenodebedel = mhead ;
      while ( count < deleteindex - 1 ) {
        thenodebedel = thenodebedel -> next ;
        count ++ ;
      } // while

      InstructionNode* thedeletenode = thenodebedel -> next ;
      thenodebedel -> next = thenodebedel -> next -> next ;
      if ( deleteindex == Size() - 1 ) {

        mtail = thenodebedel ;
      } // if

      delete thedeletenode ;
    } // else 
    

    mindex -- ;
    return true ;
  } // Erase()

  bool Clear() {
    int i = 0 ;
    InstructionNode* temp = mhead ;
    InstructionNode* temp2 = mhead ;
    while ( i < mindex ) {
      temp2 = temp -> next ;
      temp -> next = NULL ;
      delete temp ;
      temp = NULL ;
      temp = temp2 ;
      i++ ;
    } // while

    mhead = NULL ;
    mtail = NULL ;
    mindex = 0 ;
    return true ;
  } // Clear()

} ; // Vector


class Transfer{
  public : 
  Transfer() { } 

  ~Transfer() { }

  bool Isdigit( char ch ) {
    if ( ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || 
         ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '0' )
      return true ;
    else 
      return false ;
  } // Isdigit()

  float Strtof( string str ) {
    float num = 0 ;
    int index = 0 ;
    int dotindex = 0 ;
    while ( index < str.length() ) {
      if ( str[ index ] == '.' )
        dotindex = index ;
      index ++ ;
    } // while

    index = dotindex + 1 ;
    int count = 1, countnum = 0 ;
    while ( index < str.length() ) {
      if ( Isdigit( str[ index ] ) ) {
        float temp = ( str[ index ] - '0' ) ;
        countnum = 0 ;
        while ( countnum < count ) {
          temp = temp * 0.1 ;
          countnum ++ ;
        } // while

        num += temp ;
      } // if

      count++ ;
      index++ ;
    } // while
    
    index = 0 ;
    int intnum = 0 ;
    while ( index < dotindex ) {
      if ( Isdigit( str[ index ] ) ) {
        intnum = intnum * 10 + ( str[ index ] - '0' ) ;
      } // if

      index ++ ;
    } // while

    num = intnum + num ;
    if ( str[ 0 ] == '-' ) 
      num = num * -1 ;
    return num ;
  } // Strtof()

  int Strtoint( string str ) {
    int index = 0 ;
    int intnum = 0 ;
    while ( index < str.length() ) {
      if ( Isdigit(  str[ index ] ) ) {
        intnum = intnum *10 + ( str[ index ]- '0' ) ;
      } // if

      index ++ ;
    } // while

    if ( str [0] == '-' ) 
      intnum = intnum * -1 ;
    return intnum ;
  } // Strtoint()

} ;// Transfer

class TokenTable {
  public :
  vector < string > mwhitespace ;
  Transfer mtransfer ;
  TokenTable() {
    Createtable() ;
  } // TokenTable()

  ~TokenTable() {
  } // ~TokenTable()

  void Createtable() {
    string str = " " ;
    mwhitespace.push_back( str ) ;
    str = "\t" ;
    mwhitespace.push_back( str ) ;
    str = "\n" ;
    mwhitespace.push_back( str ) ;
    str = "\r" ;
    mwhitespace.push_back( str ) ;
  } // Createtable()

  bool IsWhitespace( string str ) {
    string temp = "\0" ;
    bool returnvalue = false ;
    for ( int it = 0 ; it < mwhitespace.size() ; it++ ) {
      temp = mwhitespace[ it ] ;
      if ( temp == str ) {
        returnvalue = true ;
      } // if
    } // for

    return returnvalue ;
  } // IsWhitespace()

  bool IsWhitespace( char ch ) {
    string str = "" ;
    str += ch ;
    return IsWhitespace( str ) ;
  } // IsWhitespace()

  bool IfSeparatorisAToken( string str ) {
    if ( str == "(" || str == ")" || str == "\'" || str == "\"" || str == ";" ) {
      return true ;
    } // if
    else
      return false ;
  } // IfSeparatorisAToken()

  bool IfSeparatorisAToken( char ch ) {
    string str = "" ;
    str += ch ;
    return IfSeparatorisAToken( str ) ;
  } // IfSeparatorisAToken()

  bool IsSeparators( char ch ) {
    if ( IsWhitespace( ch ) ) {
      return true ;
    } // if
    else if ( IfSeparatorisAToken( ch ) ) {
      return true ;
    } // else if
    else
      return false ;
  } // IsSeparators()

  bool IsINT( string peek ) {
    int pos = 0 ;
    if ( pos < peek.length() && ( peek[ pos ] == '+' || peek[ pos ] == '-' ) )
      pos++ ;
    bool num = false ;
    while ( pos < peek.length() ) {
      char ch = peek[ pos ] ;
      if ( ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' ||
           ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '0' ) {
        pos++ ;
        num = true ;
      } // if
      else
        return false ;
    } // while

    if ( num )
      return true ;
    else 
      return false ;
  } // IsINT()

  bool IsFLOAT( string peek ) {
    int pos = 0;
    bool dot = false;
    bool interger = false;
    bool decimal = false;
    if ( pos < peek.length() && ( peek[ pos ] == '+' || peek[ pos ] == '-' ) )
      pos++ ;

    while ( pos < peek.length() ) {
      if ( !dot && mtransfer.Isdigit( peek[ pos ] ) ) {
        interger = true ;
        pos++ ;
      } // if
      else if ( peek[ pos ] == '.' ) {
        if ( dot )
          return false ;
        dot = true ;
        pos++ ;
      } // else if
      else if ( dot && mtransfer.Isdigit( peek[ pos ] ) ) {
        decimal = true ;
        pos++ ;
      } // else if
      else 
        return false;
    } // while

    if ( interger && dot )
      return true ;
    else if ( decimal && dot )
      return true ;
    else 
      return false ;
  } // IsFLOAT()


  int FindTokenType( string str ) {
    // regex regINT( "[+-]?[0-9]+" ) ;
    // regex regFLOATone( "[+-]?[0-9]*.[0-9]+" ) ;
    // regex regFLOATtwo( "[+-]?[0-9]+.[0-9]*" ) ;
    string falsestmt1 = "nil" ;
    string falsestmt2 = "#f" ;
    string truestmt1 = "t" ;
    string truestmt2 = "#t" ;
    if ( IsINT( str ) )
      return INT ;
    else if ( IsFLOAT( str ) )
      return FLOAT ;
    else if ( str == "(" )
      return LEFT_PAREN ;
    else if ( str == ")" )
      return RIGHT_PAREN ;
    else if ( str == "\"" )
      return STRING ;
    else if ( str == "." )
      return DOT ;
    else if ( str == "\'" )
      return QUOTE ;
    else if ( str == falsestmt1 || str == falsestmt2 )
      return NIL ;
    else if ( str == truestmt1 || str == truestmt2 )
      return T ;
    else
      return SYMBOL ;
  } // FindTokenType()

  bool Isnextline( char ch ) {
    if ( ch == '\n' || ch == '\r' )
      return true ;
    else
      return false ;
  } // Isnextline()

  int Countlength( string str ) {
    int index = 0 ;
    int length = 0 ;
    while ( index < str.size() ) {
      if ( str[ index ] == '\n' || str[ index ] == '\t' ||
           str[ index ] == '\\' || str[ index ] == '\"' ) {
        length += 2 ;
      } // if
      else 
        length++ ;

      index ++ ;
    } // while
  
    return length ;
  } // Countlength()
}; // class TokenTable


enum Sexpstatus {
  COMPLETE,
  NOTCOMPLETE,
  ERRORSTMT
}; // enum Sexpstatus


class Syntax {

  public :
  Vector mtokengroup ;
  int mindex ;
  TokenTable mtable ;
  Syntax() { mindex = 0 ; }

  ~Syntax() {
  } // ~Syntax()

  void Reset() {
    mindex = 0 ;
    mtokengroup.Clear() ;
  } // Reset()

  void Setsyntaxcheack( Vector input ) {
    // mtokengroup = input ;
    int index = 0 ;
    InstructionNode* memofindex = input.Access( index ) ;
    InstructionNode copy ;
    while ( index < input.Size() ) {
      copy.tokenname = memofindex -> tokenname ;
      copy.tokentype = memofindex -> tokentype ;
      copy.column = memofindex -> column ;
      copy.line = memofindex -> line ;
      copy.resetcolumn = memofindex -> resetcolumn ;
      mtokengroup.Push_back( copy ) ;
      index++ ;
      memofindex = memofindex -> next ;
    } // while
    
  } // Setsyntaxcheack()

  InstructionNode Getnexttoken() {
    if ( mindex < mtokengroup.Size() ) {
      InstructionNode* memloc = mtokengroup.Access( mindex ) ;
      if ( memloc -> tokentype != STRING ) {
        InstructionNode node ;
        node.tokenname = memloc -> tokenname ;
        node.tokentype = memloc -> tokentype ;
        node.line = memloc -> line ;
        node.column = memloc -> column ;
        node.next = NULL ;
        node.resetcolumn = memloc -> resetcolumn ;
        mindex += 1 ;
        return node ;
      } // if
      else { // STRING
        InstructionNode node ;
        if ( mindex + 2 < mtokengroup.Size() ) { // successful string
          node.tokenname = memloc -> tokenname ;
          node.tokentype = memloc -> tokentype ;
          node.line = memloc -> line ;
          node.column = memloc -> column ; // not sure 
          node.next = NULL ;
          node.resetcolumn = memloc -> resetcolumn ;
          mindex += 3 ;
        } // if
        else if ( mindex + 1 < mtokengroup.Size() ) {
          memloc = mtokengroup.Access( mindex + 1 ) ;
          node.tokenname = memloc -> tokenname ;
          node.tokentype = memloc -> tokentype ;
          node.line = memloc -> line ;
          node.column = memloc -> column + node.tokenname.length() ; // not sure 
          node.next = NULL ;
          node.resetcolumn = memloc -> resetcolumn ;
          mindex += 2 ;
        } // else if
        else {
          node.tokenname = memloc -> tokenname ;
          node.tokentype = memloc -> tokentype ;
          node.line = memloc -> line ;
          node.column = memloc -> column + 1 ; // not sure 
          node.next = NULL ;
          node.resetcolumn = memloc -> resetcolumn ;
          mindex += 1 ;
        } // else

        return node ;        
      } // else
    } // if
    else {
      InstructionNode node ;
      node.tokenname = "error" ;
      node.tokentype = ERROR ;
      return node ;
    } // else
  } // Getnexttoken()

  InstructionNode Peeknexttoken() {
    if ( mindex < mtokengroup.Size() ) {
      InstructionNode* memloc = mtokengroup.Access( mindex ) ;
      if ( memloc -> tokentype != STRING ) {
        InstructionNode node ;
        node.tokenname = memloc -> tokenname ;
        node.tokentype = memloc -> tokentype ;
        node.line = memloc -> line ;
        node.column = memloc -> column ;
        node.next = NULL ;
        node.resetcolumn = memloc -> resetcolumn ;
        return node ;
      } // if
      else { // string
        InstructionNode node ;
        if ( mindex + 2 < mtokengroup.Size() ) { // successful string
          node.tokenname = memloc -> tokenname ;
          node.tokentype = memloc -> tokentype ;
          node.line = memloc -> line ;
          node.column = memloc -> column ; // not sure 
          node.next = NULL ;
          node.resetcolumn = memloc -> resetcolumn ;
        } // if
        else if ( mindex + 1 < mtokengroup.Size() ) {
          memloc = mtokengroup.Access( mindex + 1 ) ;
          node.tokenname = memloc -> tokenname ;
          node.tokentype = memloc -> tokentype ;
          node.line = memloc -> line ;
          // node.column = memloc -> column + node.tokenname.length() ; length() can not deal \n \t
          node.column = ( memloc -> column ) + mtable.Countlength( node.tokenname ) ;
          node.next = NULL ;
          node.resetcolumn = memloc -> resetcolumn ;
        } // else if
        else {
          node.tokenname = memloc -> tokenname ;
          node.tokentype = memloc -> tokentype ;
          node.line = memloc -> line ;
          node.column = memloc -> column + 1 ; // not sure 
          node.next = NULL ;
          node.resetcolumn = memloc -> resetcolumn ;
        } // else

        return node ;
      } // else
    } // if
    else {
      InstructionNode node ;
      node.tokenname = "null" ;
      node.tokentype = HOLLOW ;
      return node ;
    } // else
  } // Peeknexttoken()

  bool Allisstring() {
    InstructionNode* stringbody = mtokengroup.Access( mindex + 1 ) ;
    InstructionNode* stringmark = mtokengroup.Access( mindex + 2 ) ;
    if ( stringbody -> tokentype == STRING && stringmark -> tokentype == STRING ) {
      return true ;
    } // if
    else
      return false ;
  } // Allisstring()

  int Syntaxforsexp() { // need to deal " string "
    InstructionNode node = Peeknexttoken() ;
    int type = node.tokentype ;
    if ( type == SYMBOL || type == INT || type == FLOAT || type == STRING || type == NIL || type == T ) {
      if ( type == STRING ) {
        if ( mindex + 2 < mtokengroup.Size() && Allisstring() ) {
          node = Getnexttoken() ;
          return COMPLETE ;
        } // if
        else {
          // cout << "string error :"  ;
          cout << "> ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
          cout << node.line << " Column " << node.column << endl << endl ;
          return ERRORSTMT ;
        } // else
      } // if
      else {
        node = Getnexttoken() ;
        return COMPLETE ;
      } // else
    } // if
    else if ( type == LEFT_PAREN ) {
      node = Getnexttoken() ;
      int status = Syntaxforsexp() ;
      if ( status == ERRORSTMT ) {
        return ERRORSTMT ;
      } // if
      else if ( status == NOTCOMPLETE ) {
        return NOTCOMPLETE ;
      } // else if

      bool stop = false ;
      while ( stop != true ) {
        node = Peeknexttoken() ;
        type = node.tokentype ;
        if ( type == LEFT_PAREN || type == QUOTE ||
             type == SYMBOL || type == INT || type == FLOAT || type == STRING || type == NIL || type == T ) {
          status = Syntaxforsexp() ;
          if ( status == ERRORSTMT ) {
            stop = true ;
            return ERRORSTMT ;
          } // if
        } // if
        else if ( node.tokentype == DOT || node.tokentype == RIGHT_PAREN ) {
          stop = true ;
        } // else if
        else if ( node.tokentype == HOLLOW ) {
          return NOTCOMPLETE ;
        } // else if
        else {
          cout << "> ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
          cout << node.line << " Column " << node.column << endl << endl ;
          return ERRORSTMT ;
        } // else
      } // while

      node = Peeknexttoken() ;
      // cout << "810:" << node.tokenname << " " << node.tokentype << endl ;
      if ( node.tokentype == DOT ) { // . sexp ) 
        node = Getnexttoken() ;
        status = Syntaxforsexp() ;
        if ( status == ERRORSTMT ) {
          return ERRORSTMT ;
        } // if
        else if ( status == NOTCOMPLETE ) {
          return NOTCOMPLETE ;
        } // else if

        
        node = Peeknexttoken() ;
        // cout << "819:" << node.tokenname << " " << node.tokentype << endl ;
        if ( node.tokentype == RIGHT_PAREN ) {
          node = Getnexttoken() ;
          return COMPLETE ;
        } // if
        else if ( node.tokentype == HOLLOW ) {
          node = Getnexttoken() ;
          return NOTCOMPLETE ;
        } // else if
        else {
          cout << "> ERROR (unexpected token) : ')' expected when token at Line " ;
          cout << node.line << " Column " << node.column << " is >>" << node.tokenname << "<<\n" << endl ;
          return ERRORSTMT ;
        } // else

      } // if
      else if ( node.tokentype == RIGHT_PAREN ) {
        node = Getnexttoken() ;
        return COMPLETE ;
      } // else if
      else if ( node.tokentype == HOLLOW ) {
        return NOTCOMPLETE ;
      } // else if
      else {
        cout << "Line" << node.line << "column " << node.column ;
        cout << "expect a dot but a " << node.tokenname << endl ;
        return ERRORSTMT ;
      } // else

    } // else if
    else if ( type == QUOTE ) {
      node = Getnexttoken() ;
      return Syntaxforsexp() ;
    } // else if
    else if ( type == HOLLOW ) {
      return NOTCOMPLETE ;
    } // else if
    else {
      cout << "> ERROR (unexpected token) : atom or '(' expected when token at Line " ;
      cout << node.line << " Column " << node.column << " is >>" << node.tokenname << "<<\n" << endl ;
      return ERRORSTMT ;
    } // else
  } // Syntaxforsexp()
}; // Syntax()


class Scaner {
  public :
  Scaner() { 
    mtreeroot = NULL ; 
    msysstop = false;
  } // Scaner()

  ~Scaner() {
  } // ~Scaner()

  TreeNode * mtreeroot ;
  TokenTable mtable ;
  Syntax msyntax ;
  map<string,TreeNode*> mdefmap;
  int mspnum;
  bool msysstop ;
  void Deletetree( TreeNode * tree ) {
    if ( tree == NULL ) {
      return ;
    } // if

    Deletetree( tree -> left ) ;
    Deletetree( tree -> right ) ;
    delete tree ;
    tree = NULL ;
    return ;
  } // Deletetree()

  string DealSYMNOLstring( string str ) {
    int index = 0 ;
    string returnstr = "" ;
    while ( index < str.length() ) {
      if ( str[ index ] == '\\' && index + 1 < str.length() ) {
        index ++ ;
        char nextch = str [ index ] ;
        if ( nextch == 't' || nextch == 'n' || nextch == '\"' || nextch == '\\' ) {
          returnstr += nextch ;
        } // if
        else {
          returnstr += '\\' ;
          returnstr += nextch ;
        } // else
      } // if
      else {
        returnstr += str[ index ] ;
      } // else

      index++ ;
    } // while

    return returnstr ;
  } // DealSYMNOLstring()

  int Lexicalpraser( Vector tokengroup, Vector  &tokenlinklist ) {
    int index = 0 ;
    InstructionNode node ;
    while ( index < tokengroup.Size() ) {
      InstructionNode* memlocofindex = tokengroup.Access( index ) ;
      InstructionNode* memlocofindexplus = tokengroup.Access( index + 1 ) ;
      InstructionNode* memlocofindexplusplus = tokengroup.Access( index + 2 ) ;
      node.column = memlocofindex -> column ;
      if ( index < tokengroup.Size() - 1 && 
           mtable.FindTokenType( memlocofindex -> tokenname ) == LEFT_PAREN 
           && mtable.FindTokenType( memlocofindexplus -> tokenname ) == RIGHT_PAREN ) {
        node.line = memlocofindex -> line ;
        node.tokenname = "nil" ;
        node.tokentype = NIL ;
        node.resetcolumn = memlocofindexplus -> column ;
        index += 2 ;
        tokenlinklist.Push_back( node ) ;
      } // if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == LEFT_PAREN ) {
        node.line = memlocofindex -> line ;
        node.tokenname = memlocofindex -> tokenname ;
        node.tokentype = LEFT_PAREN ;
        node.resetcolumn = memlocofindex -> column ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == RIGHT_PAREN ) {
        node.line = memlocofindex -> line ;
        node.tokenname = memlocofindex -> tokenname ;
        node.resetcolumn = memlocofindex -> column ;
        node.tokentype = RIGHT_PAREN ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == INT ) {
        node.line = memlocofindex -> line ;
        node.tokenname = memlocofindex -> tokenname ;
        node.tokentype = INT ;
        node.resetcolumn = memlocofindex -> column + memlocofindex -> tokenname.length() - 1 ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == STRING ) {
        if ( index + 2 < tokengroup.Size() && memlocofindex -> tokenname == "\"" 
             && memlocofindexplusplus -> tokenname == "\"" ) {
          node.tokenname = "\"" ;
          node.tokentype = STRING ;
          node.column = memlocofindex -> column ;
          node.line = memlocofindex -> line ;
          node.resetcolumn = memlocofindex -> column ;
          tokenlinklist.Push_back( node ) ; // "

          node.line = memlocofindexplus -> line ;
          node.column = memlocofindexplus -> column ;
          node.tokenname = memlocofindexplus -> tokenname ;
          node.tokentype = STRING ;
          node.resetcolumn = memlocofindexplusplus -> column ; // " string " <- re
          tokenlinklist.Push_back( node ) ; // string body

          node.tokenname = "\"" ;
          node.tokentype = STRING ;
          node.column = memlocofindexplusplus -> column ;
          node.line = memlocofindexplusplus -> line ;
          node.resetcolumn = memlocofindexplusplus -> column ;
          tokenlinklist.Push_back( node ) ; // "
          index = index + 3 ;
        } // if
        else {
          if ( index + 1 < tokengroup.Size() ) {
            node.tokenname = "\"" ;
            node.tokentype = STRING ;
            node.column = memlocofindex -> column ;
            node.line = memlocofindex -> line ;
            node.resetcolumn = memlocofindex -> column ;
            tokenlinklist.Push_back( node ) ; // "

            node.tokenname = memlocofindexplus -> tokenname ;
            node.line = memlocofindexplus -> line ;
            node.column = memlocofindexplus -> column ;
            node.tokentype = STRING ;
            node.resetcolumn = memlocofindexplus -> column ;
            tokenlinklist.Push_back( node ) ; // string body
            index = index + 2 ;
          } // if
          else {
            node.tokenname = memlocofindex -> tokenname ;
            node.line = memlocofindex -> line ;
            node.column = memlocofindex -> column ;
            node.tokentype = STRING ;
            node.resetcolumn = memlocofindex -> column ;
            tokenlinklist.Push_back( node ) ; // "
            index ++ ;
          } // else

          // cout << "> ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
          // cout << node.line << " Column " << node.column + node.tokenname.length() << endl << endl ;
          // return ERRORSTMT ;
        } // else
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == DOT ) {
        node.line = memlocofindex -> line ;
        node.tokenname = memlocofindex -> tokenname ;
        node.resetcolumn = memlocofindex -> column ;
        node.tokentype = DOT ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == FLOAT ) {
        node.line = memlocofindex -> line ;
        node.tokenname = memlocofindex -> tokenname ;
        node.resetcolumn = memlocofindex -> column + memlocofindex -> tokenname.length() - 1 ;
        node.tokentype = FLOAT ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == NIL ) {
        node.line = memlocofindex -> line ;
        node.tokenname = "nil" ;
        node.resetcolumn = memlocofindex -> column + memlocofindex -> tokenname.length() - 1 ;
        node.tokentype = NIL ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == T ) {
        node.line = memlocofindex -> line ;
        node.tokenname = "#t" ;
        node.resetcolumn = memlocofindex -> column + memlocofindex -> tokenname.length() - 1 ;
        node.tokentype = T ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType( memlocofindex -> tokenname ) == QUOTE ) {
        node.line = memlocofindex -> line ;
        node.tokenname = "quote" ;
        node.resetcolumn = memlocofindex -> column ;
        node.tokentype = QUOTE ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if
      else if ( mtable.FindTokenType(  memlocofindex -> tokenname ) == SYMBOL ) {
        node.line = memlocofindex -> line ;
        // node.tokenname = tokengroup[ index ].tokenname ;
        node.tokenname = DealSYMNOLstring( memlocofindex -> tokenname ) ;
        node.resetcolumn = memlocofindex -> column + memlocofindex -> tokenname.length() - 1 ;
        node.tokentype = SYMBOL ;
        index++ ;
        tokenlinklist.Push_back( node ) ;
      } // else if

    } // while

    return NOTCOMPLETE ;
  } // Lexicalpraser()

  void GetTokenGroup( Vector &tokengroup, string projinput, 
                      int &currentline ) {
    string buffer = "" ;
    InstructionNode node ;
    char curch = ' ' ;
    int index = 0 ;
    bool stop = false ;
    int column = 0 ;
    while ( index < projinput.size() ) {
      while ( index < projinput.size() && mtable.IsWhitespace( curch ) ) {
        curch = projinput[ index ] ;
        column++ ;
        index++ ;
      } // while

      node.column = column ;
      node.line = currentline ;
      while ( index < projinput.size() && mtable.IsSeparators( curch ) != true ) {
        buffer += curch ;
        if ( curch == '\\' && index < projinput.size() ) {
          char nextch = projinput[ index ] ;
          if ( nextch == 't' || nextch == 'n' || nextch == '\"' || nextch == '\\' ) {
            buffer += nextch ;
          } // if
          else {
            buffer += '\\' ;
            buffer += nextch ;
          } // else

          index++ ;
          column++ ;
        } // if

        curch = projinput[ index ] ;
        column++ ;
        index++ ;
      } // while

      if ( !buffer.empty() ) {
        node.tokenname = buffer ;
        tokengroup.Push_back( node ) ;
      } // if

      if ( mtable.IfSeparatorisAToken( curch ) == true ) {
        if ( curch == ';' ) {
          while ( index < projinput.size() ) {
            curch = projinput[ index ] ;
            index++ ;
            column++ ;
          } // while
        } // if
        else if ( curch == '\"' ) {
          Dealstring( tokengroup, projinput, index, curch, currentline, column ) ;
          // cout << "aa835" ;
        } // else if
        else {
          buffer = "" ;
          buffer += curch ;
          node.tokenname = buffer ;
          node.column = column ;
          node.line = currentline ;
          tokengroup.Push_back( node ) ;
          curch = projinput[ index ] ;
          index++ ;
          column++ ;
        } // else
      } // if

      buffer.clear() ;
    } // while
  } // GetTokenGroup()

  InstructionNode Catchfirstnode( Vector &current, Vector &target ) {
    InstructionNode* firstoftarget = target.Access( 0 ) ;
    InstructionNode node ;
    if ( firstoftarget -> tokentype != STRING ) {
      node.next = NULL ;
      node.tokenname = firstoftarget -> tokenname ;
      node.tokentype = firstoftarget -> tokentype ;
      node.line = firstoftarget-> line ;
      node.column = firstoftarget-> column ;
      node.resetcolumn = firstoftarget-> resetcolumn ;
      target.Erase( target.Begin() ) ;
      current.Push_back( node ) ;
      return node ;
    } // if
    else {
      if ( 2 < target.Size() ) { // complete string
        node.next = NULL ;
        node.tokenname = firstoftarget -> tokenname ; // "
        node.tokentype = firstoftarget -> tokentype ;
        node.line = firstoftarget-> line ;
        node.column = firstoftarget-> column ;
        node.resetcolumn = firstoftarget-> resetcolumn ;
        firstoftarget = firstoftarget -> next ;
        target.Erase( target.Begin() ) ;
        current.Push_back( node ) ;
        
        node.next = NULL ;
        node.tokenname = firstoftarget -> tokenname ; // string body 
        node.tokentype = firstoftarget -> tokentype ;
        node.line = firstoftarget-> line ;
        node.column = firstoftarget-> column ;
        node.resetcolumn = firstoftarget-> resetcolumn ;
        firstoftarget = firstoftarget -> next ;
        target.Erase( target.Begin() ) ;
        current.Push_back( node ) ;

        node.next = NULL ;
        node.tokenname = firstoftarget -> tokenname ; // "
        node.tokentype = firstoftarget -> tokentype ;
        node.line = firstoftarget-> line ;
        node.column = firstoftarget-> column ;
        node.resetcolumn = firstoftarget-> resetcolumn ;
        target.Erase( target.Begin() ) ;
        current.Push_back( node ) ;
        return node ;
      }  // if
      else if ( 1 < target.Size() ) { // " string
        node.next = NULL ;
        node.tokenname = firstoftarget -> tokenname ; // "
        node.tokentype = firstoftarget -> tokentype ;
        node.line = firstoftarget-> line ;
        node.column = firstoftarget-> column ;
        node.resetcolumn = firstoftarget-> resetcolumn ;
        firstoftarget = firstoftarget -> next ;
        target.Erase( target.Begin() ) ;
        current.Push_back( node ) ;
        
        node.next = NULL ;
        node.tokenname = firstoftarget -> tokenname ; // string body 
        node.tokentype = firstoftarget -> tokentype ;
        node.line = firstoftarget-> line ;
        node.column = firstoftarget-> column ;
        node.resetcolumn = firstoftarget-> resetcolumn ;
        target.Erase( target.Begin() ) ;
        current.Push_back( node ) ;
      } // else if
      else { // "
        node.next = NULL ;
        node.tokenname = firstoftarget -> tokenname ; // "
        node.tokentype = firstoftarget -> tokentype ;
        node.line = firstoftarget-> line ;
        node.column = firstoftarget-> column ;
        node.resetcolumn = firstoftarget-> resetcolumn ;
        target.Erase( target.Begin() ) ;
        current.Push_back( node ) ;
      } // else

      return node ;
    } // else
  } // Catchfirstnode()

  bool Stopsys( Vector tokengroup ) {
    if ( tokengroup.Size() == 5 ) {
      InstructionNode* memloc = tokengroup.Access( 0 ) ; // 0
      InstructionNode* memlocplus = memloc -> next ; // 1
      InstructionNode* memlocplusplus = memlocplus -> next ; // 2
      InstructionNode* memlocof3 = memlocplusplus -> next ; // 3
      InstructionNode* last = memlocof3 -> next ; // 4
      if ( memloc -> tokenname == "(" && memlocplus -> tokenname == "exit" && 
           memlocplusplus -> tokenname == "." && memlocof3 -> tokenname == "nil" &&  
           last -> tokenname == ")" ) {
        if ( memlocplus -> tokentype == SYMBOL )
          return true ;
      } // if

    } // if

    return false ;
  } // Stopsys()

  void Resetcolumnforsameline( Vector &tokenlinklist, InstructionNode lastnode ) { 
    int index = 0 ;
    InstructionNode* temp = NULL ;
    while ( index < tokenlinklist.Size() ) {
      temp = tokenlinklist.Access( index ) ;
      if ( lastnode.line == temp -> line ) {
        temp -> line = 1 ;
        temp -> column -= lastnode.resetcolumn ;
        temp -> resetcolumn -= lastnode.resetcolumn;
      } // if

      index ++  ;
    } // while

  } // Resetcolumnforsameline()

  void Dealstring( Vector &tokengroup, string projinput, int &index, 
                    char &curch, int &line, int &column ) {
    // cout << "1266 : "<<  index << " " << column << endl ;
    string buffer = "" ;
    InstructionNode node ;
    buffer += curch ;
    node.tokenname = buffer ;
    node.line = line ;
    node.column = column ;
    tokengroup.Push_back( node ) ; // "
    column++ ;
    int strbodystartcolumn = column ;
    curch = projinput[ index ] ;


    // cout << "1278 : "<<  index << " " << column << endl ;
    bool stop = false ;
    node.column = index ;
    node.tokenname = "" ;
    string strinput = "" ;
    if ( index < projinput.size() - 1 ) {
      while ( stop != true && index < projinput.size() - 1  ) {
        if ( projinput[ index ] == '\\' && index + 1 < projinput.size() ) {
          index++ ;
          column ++ ;
          char nextchar = projinput[ index ] ;
          if ( nextchar == '\"' || nextchar == '\\' ) {
            strinput += nextchar ;
          } // if
          else if ( nextchar == 't' ) {
            strinput += '\t' ;
          } // else if
          else if ( nextchar == 'n' ) {
            strinput += '\n' ;
          } // else if
          else {
            strinput += '\\' ;
            strinput += nextchar ;
          } // else 

          curch = projinput[ index ] ;
          index ++ ;
          column++ ;
        } // if
        else if ( projinput[ index ] != '\"' ) {
          curch = projinput[ index ] ;
          strinput += projinput[ index ] ;
          index ++ ;
          column++ ;
        } // if
        else {
          stop = true ;
        } // else 
      } // while

      node.tokenname = strinput ;
      node.column = strbodystartcolumn ;
      node.line = line ;
      // cout << "1288strbody : " << index << endl ;
      tokengroup.Push_back( node ) ; // stringbody
      curch = projinput[ index ] ;
      if ( curch == '\"' ) {
        node.tokenname = curch ; // " or \0
        node.line = line ;
        node.column = column ;
        tokengroup.Push_back( node ) ;
        index++ ;
        column++ ;
        curch = projinput[ index ] ;
        index++ ;
        column++ ;
      } // if
      else {
        index++ ;
        column++ ;
        curch = projinput[ index ] ;
        index++ ;
        column++ ;
      } // else
    } // if

    column-- ;
    // cout << "1343 : "<<  index << " " << column << endl ;
  }  // Dealstring()


  void SpiltleftrightParan( Vector &tokenlinklist ) {
    int index = 0 ;
    InstructionNode* temp = tokenlinklist.Access( index ) ;
    while ( index < tokenlinklist.Size() ) {
      if ( index + 1 < tokenlinklist.Size() ) {
        InstructionNode* temp2 = temp -> next ;
        if ( temp -> tokentype == LEFT_PAREN
             && temp2 -> tokentype == RIGHT_PAREN ) {
          temp -> tokenname = "nil" ;
          temp -> tokentype = NIL ;
          tokenlinklist.Erase( tokenlinklist.Begin() + index + 1 ) ;
        } // if
      } // if

      index++ ;
      temp = temp -> next ;
    }  // while
  } // SpiltleftrightParan()

  void Mergestringstmt( Vector &tokenlinklist ) {
    int index = 0 ;
    InstructionNode* cur = tokenlinklist.Access( 0 ) ;
    while ( index < tokenlinklist.Size() ) {
      if ( cur -> tokentype == STRING && index + 2 < tokenlinklist.Size() ) {
        InstructionNode* curnext = cur -> next ;
        InstructionNode* curnextnext = curnext -> next ;    
        if ( curnext -> tokentype == STRING && curnextnext -> tokentype == STRING ) {
          tokenlinklist.Erase( index ) ;
          tokenlinklist.Erase( index + 1 ) ;
        } // if
        
        cur = tokenlinklist.Access( index ) ;
        cur = cur -> next ;
      } // if
      else {
        cur = cur -> next ;
      } // else 

      index++ ;
    } // while
  } // Mergestringstmt()

  void Interpreter() {
    // vector < InstructionNode > tokengroupinput ;
    // vector < InstructionNode > tokenlinklist ;
    // vector < InstructionNode > tokenlinklist2 ;
    Vector tokengroupinput ;
    Vector tokenlinklist ;
    Vector tokenlinklist2 ;
    InstructionNode lastnode ;
    Transfer transfer ;
    int syntaxstatus = - 1 ;
    int currentline = 1, recurrentline = 1 ;
    string projinput = "" ;
    char* str = new char[ 1000 ] ;
    cin.getline( str, 1000 ) ;
    gtestnum = str ;
    bool nextstop = false ;
    

    while ( msysstop != true ) {
      if ( syntaxstatus == NOTCOMPLETE && tokenlinklist2.Size() != 0 ) {
        lastnode =  Catchfirstnode( tokenlinklist, tokenlinklist2 ) ;
        // cout << "tokenlist :" << endl ;
        SpiltleftrightParan( tokenlinklist ) ;
        // Printtokenlinklist( tokenlinklist ) ;
        // cout << "tokenlinklist2 : " << endl ;
        // Printtokenlinklist( tokenlinklist2 ) ;
        msyntax.Reset() ;
        msyntax.Setsyntaxcheack( tokenlinklist ) ;
        syntaxstatus = msyntax.Syntaxforsexp() ;
        if ( syntaxstatus == COMPLETE ) {
          Mergestringstmt( tokenlinklist ) ;
        } // if
      } // if
      else if ( tokenlinklist.Size() == 0 || tokenlinklist2.Size() == 0 ) {
        if ( nextstop == false ) {
          projinput = "" ;
          tokengroupinput.Clear() ;
          tokenlinklist2.Clear() ;
          // cout << "cin command line " << endl ;
          if ( !cin.getline( str, 1000 ) ) {
            nextstop = true ;
          } // if

          projinput = str ;
          // projinput = "(12(. 3))" ;
          projinput += " " ;
          // cout << "proj :" << projinput << endl ;
          GetTokenGroup( tokengroupinput, projinput, currentline ) ;
          currentline++ ;
          syntaxstatus = Lexicalpraser( tokengroupinput, tokenlinklist2 ) ;
        } // if
        else {
          cout << "> ERROR (no more input) : END-OF-FILE encountered\n" ;
          msysstop = true ;
          syntaxstatus = NOTCOMPLETE ;
        } // else 
      } // else if


      if ( syntaxstatus == COMPLETE ) {
        recurrentline = currentline ;
        currentline = 1 ;
        if ( tokenlinklist.Size() == 1 ) {
          // cout << "> " ;
          InstructionNode* temp = tokenlinklist.Access( 0 ) ;
          

          if ( Defcheck( temp->tokenname ) ) {
            mtreeroot = mdefmap[temp->tokenname];
          } // if
          else if ( gResmap.find( temp->tokenname ) != gResmap.end() && temp->tokentype == SYMBOL ) {
            mtreeroot = gResmap[temp->tokenname] ;
          } // else if
          else {
            mtreeroot = new TreeNode;
            mtreeroot->name = temp->tokenname;
            mtreeroot->type = temp->tokentype;
            mtreeroot->left = NULL;
            mtreeroot->right = NULL;
            mtreeroot->con = false;
            mtreeroot->e = false;
          } // else
          
        } // if
        else {
          // cout << " syntax correct " << endl ;
          // Printtokenlinklist( tokenlinklist ) ;
          TransferToTreestmt( tokenlinklist ) ;
          if ( Stopsys( tokenlinklist ) ) {
            cout << "> \n" ;
            msysstop = true ;
            return ;
          } // if

          // cout << " sucees tranfer : " ;
          // Printtokenlinklist( tokenlinklist ) ;
          mtreeroot = CreateTree( tokenlinklist, 0, tokenlinklist.Size() - 1 ) ;
          // cout << " print tree : " << endl ;
          // Inorder( mtreeroot ) ;
          // cout << endl ;
        } // else 

        
        Qucheck( mtreeroot ) ;
        Num( mtreeroot ) ;
        Aliases( mtreeroot, true ) ;
        
        try {
          if ( Isfunc( mtreeroot ) ) {
            mspnum = 0;
            mtreeroot = Eval( mtreeroot );
          } // if
        } // try
        catch ( TreeNode *e ) {
          mtreeroot = e;
        } // catch

        cout << "> ";
    
        if ( !Ato( mtreeroot ) && !msysstop ) { 
          // int spacenum = 2 ;
          // bool temp = true ;
          // cout << "( " ;
          // Prettyprint( NULL, mtreeroot, spacenum, temp ) ;
          // spacenum -= 2 ;
          // Printspace( spacenum ) ;
          // cout << ")\n" ;
          gspa = 0;
          Myp( mtreeroot, true, false );
          tokenlinklist.Clear() ;
        } // if
        else if ( !msysstop ) {
          
          Atomp( mtreeroot );
          tokenlinklist.Clear() ;
        } // else if
        //   Deletetree( mtreeroot ) ;

        if ( tokenlinklist2.Size() != 0 ) {
          // cout << "reset node line column " << endl ;        
          Resetcolumnforsameline( tokenlinklist2, lastnode ) ;
          syntaxstatus = NOTCOMPLETE ;
          currentline = 2 ;
        } // if

        cout << "\n" ;
      } // if
      else if ( syntaxstatus == ERRORSTMT ) {
        currentline = 1 ;
        tokengroupinput.Clear() ;
        tokenlinklist.Clear() ;
        tokenlinklist2.Clear() ;
      } // else if

      // cout << " ======= next turn ======  " << endl ;    
    } // while
  } // Interpreter()

  void Printtokenlinklist( Vector tokengroup ) {
    cout << "tokenlinklist.size: " << tokengroup.Size() << endl ;
    InstructionNode *cur = tokengroup.mhead ;
    int i = 0 ;

    while ( cur != NULL ) {
      cout << cur->tokenname << "  " ;
      cout << cur->tokentype << "  " ;
      cout << cur->line << "  " ;
      cout << cur->column << " " ; 
      cout << cur->resetcolumn << endl ;
      cur = cur -> next ;
    } // while
  } // Printtokenlinklist()

  void DealQUOTEstmt( Vector &tokengroup, int index ) {
    InstructionNode* memlocofindexplus = tokengroup.Access( index + 1 ) ;
    if ( memlocofindexplus -> tokentype == QUOTE ) {
      DealQUOTEstmt( tokengroup, index + 1 ) ;
      memlocofindexplus = tokengroup.Access( index + 1 ) ;
    } // if

    int type = memlocofindexplus -> tokentype ;
    if ( type == LEFT_PAREN ) {
      InstructionNode node ;
      node.resetcolumn = -1 ;
      node.line = - 1 ;
      node.column = - 1 ;
      node.next = NULL ;
      InstructionNode* memofindex = tokengroup.Access( index ) ;
      memofindex -> tokentype = INT ; // origin INT
      node.tokenname = "(" ;
      node.tokentype = LEFT_PAREN ;
      tokengroup.Insert( tokengroup.Begin() + index, node ) ;
      node.tokenname = "." ;
      node.tokentype = DOT ;
      tokengroup.Insert( tokengroup.Begin() + index + 2, node ) ;
      bool stop = false ;
      // stack < int > st ;
      Vectorint st ;
      int pairindex = - 1 ;
      index++ ;
      memofindex = tokengroup.Access( index ) ;
      while ( stop != true && index < tokengroup.Size() ) {
        if ( memofindex -> tokentype == LEFT_PAREN ) {
          st.Push_back( index ) ;
        } // if
        else if ( memofindex -> tokentype == RIGHT_PAREN ) {
          pairindex = index ;
          st.Erase( st.Size() - 1 ) ; // st.pop() ;
          if ( st.Size() == 0 ) {
            stop = true ;
          } // if

        } // else if
        
        memofindex = memofindex -> next ;
        index++ ;
      } // while

      node.tokenname = ")" ;
      node.tokentype = RIGHT_PAREN ;
      tokengroup.Insert( tokengroup.Begin() + pairindex, node ) ;
    } // if
    else if ( type == SYMBOL || type == INT || type == STRING || 
              type == FLOAT || type == NIL || type == T ) {
      InstructionNode* memofindex = tokengroup.Access( index ) ;
      memofindex -> tokentype = INT ;
      InstructionNode node ;
      node.tokenname = "(" ;
      node.tokentype = LEFT_PAREN ;
      node.line = - 1 ;
      node.column = - 1 ;
      tokengroup.Insert( tokengroup.Begin() + index, node ) ;
      node.tokenname = "." ;
      node.tokentype = DOT ;
      tokengroup.Insert( tokengroup.Begin() + index + 2, node ) ;
      node.tokenname = ")" ;
      node.tokentype = RIGHT_PAREN ;
      tokengroup.Insert( tokengroup.Begin() + index + 4, node ) ;
    } // else if

  } // DealQUOTEstmt()

  void TransferToTreestmt( Vector &tokengroup ) {
    int index = 0, innerindex = 0, leftparenindex = - 1 ;
    // stack < int > st ;
    Vectorint st ;
    InstructionNode node ;
    node.column = - 1 ;
    node.line = - 1 ;
    node.resetcolumn = -1 ;
    while ( index < tokengroup.Size() ) {
      
      // cout << "index: " << index << endl ;
      // cout << "tokeli : " << endl ;
      // Printtokenlinklist( tokengroup ) ;
      // cout << endl ;

      InstructionNode* memloc = tokengroup.Access( index ) ;
      if ( memloc -> tokentype == LEFT_PAREN ) {
        st.Push_back( index ) ;
      } // if
      else if ( memloc -> tokentype == RIGHT_PAREN ) {
        // cout << "rightpa" << endl ;
        IntNode* top =  st.Access( st.Size() - 1 ) ;
        // innerindex = st.top() ;
        innerindex = top -> value ;
        leftparenindex = innerindex ;
        // st.pop() ;
        st.Erase( st.Size() - 1 ) ;
        innerindex++ ;
        FitDOTPair( tokengroup, innerindex, index ) ;
        // cout << "token : " << innerindex << " " << index  << endl ;
        while ( innerindex < index ) {
          InstructionNode* memofinnerindex = tokengroup.Access( innerindex ) ;
          int tokentype = memofinnerindex -> tokentype ;
          if ( tokentype == SYMBOL || tokentype == INT || tokentype == FLOAT || tokentype == STRING ||
               tokentype == NIL || tokentype == T ) { // ( atom sexp .sexp ) 
            innerindex++ ;
            memofinnerindex = memofinnerindex -> next ;
            if ( memofinnerindex -> tokentype == DOT ) { // ( atom . sexp )
              innerindex = index ;
            } // if
            else {  // ( atom sexp . sexp ) -> ( atom . ( sexp . sexp ) )
              node.tokenname = "." ;
              node.tokentype = DOT ;
              tokengroup.Insert( tokengroup.Begin() + innerindex, node ) ;
              innerindex += 1 ;
              node.tokenname = "(" ;
              node.tokentype = SEXP ;
              tokengroup.Insert( tokengroup.Begin() + innerindex, node ) ;
              innerindex += 1 ;
              index += 2 ;
              node.tokenname = ")" ;
              node.tokentype = SEXP ;
              tokengroup.Insert( tokengroup.Begin() + index, node ) ;
              index += 1 ;
              innerindex -= 1 ;
            } // else
            
            // cout << "tokeli : " << leftparenindex << " " << index  << endl ;
            // Printtokenlinklist( tokengroup ) ;
          } // if
          else if ( tokentype == SEXP ) { // case for ( sexp . sexp ) ( sexp sexp1 . sexp2 )
            // stack < int > sexpleft ;
            Vectorint sexpleft ;
            bool stop = false ;
            while ( stop != true ) { // innerindex -> sexp tail -> '.' or sexp1
              InstructionNode* memofinnerindex = tokengroup.Access( innerindex ) ;
              if ( memofinnerindex -> tokenname == "(" ) {
                sexpleft.Push_back( innerindex ) ;
              } // if
              else if ( memofinnerindex -> tokenname == ")" ) {
                // sexpleft.pop() ;
                sexpleft.Erase( sexpleft.Size() -1 ) ;
              } // else if

              innerindex++ ;
              if ( sexpleft.Size() == 0 ) // sexpleft.empty()
                stop = true ;
            } // while

            memofinnerindex = tokengroup.Access( innerindex ) ;
            if ( memofinnerindex -> tokentype == DOT ) { // ( sexp . sexp )
              innerindex = index ;
            } // if
            else { // ( sexp sexp1 . sexp2 )
              node.tokenname = "." ;
              node.tokentype = DOT ;
              tokengroup.Insert( tokengroup.Begin() + innerindex, node ) ;
              innerindex += 1 ;
              node.tokenname = "(" ;
              node.tokentype = SEXP ;
              tokengroup.Insert( tokengroup.Begin() + innerindex, node ) ;
              innerindex += 1 ;
              index += 2 ;
              node.tokenname = ")" ;
              node.tokentype = SEXP ;
              tokengroup.Insert( tokengroup.Begin() + index, node ) ;
              index += 1 ;
              innerindex -= 1 ;

            } // else 
          } // else if

          innerindex++ ;
        } // while

        InstructionNode* temp = tokengroup.Access( leftparenindex ) ;
        // tokengroup[ leftparenindex ].tokentype = SEXP ;
        temp -> tokentype = SEXP ;
        // tokengroup[ index ].tokentype = SEXP ;
        temp = tokengroup.Access( index ) ;
        temp -> tokentype = SEXP ;
      } // else if
      else if ( memloc -> tokentype == QUOTE ) {
        // cout << "qqquote" << endl ;
        DealQUOTEstmt( tokengroup, index ) ;
        index -= 1 ;
      } // else if

      index++ ;
    } // while
  } // TransferToTreestmt()

  int FitDOTPair( Vector &tokengroup, int startindex, int &max ) {
    int index = 0 ;
    int indexofDot = - 1 ;
    // stack < InstructionNode > st 
    Vector st ;
    InstructionNode node ;
    node.resetcolumn = -1 ;
    node.line = -1 ;
    node.next = NULL ;
    node.column = -1 ;
    InstructionNode* memofindex = tokengroup.Access( index ) ;
    while ( index < max ) {
      if ( memofindex -> tokenname == "(" && memofindex -> tokentype != STRING ) {
        node.tokenname = "(" ;
        st.Push_back( node ) ;
      } // if
      else if ( memofindex -> tokenname == ")" && memofindex -> tokentype != STRING  ) {
        InstructionNode* temp = st.Access( st.Size() - 1 ) ;
        while ( temp -> tokenname != "(" ) { // st.top().tokenname != "("
          // st.pop() ;
          st.Erase( st.Size() - 1 ) ;
          temp = st.Access( st.Size() - 1 ) ;
        } // while

        // st.pop() ;
        st.Erase( st.Size() - 1 ) ;
      } // else if
      else if ( memofindex -> tokenname == "." && memofindex -> tokentype != STRING  ) {
        node.tokenname = "." ;
        node.line = index ;
        st.Push_back( node ) ;
      } // else if

      memofindex = memofindex -> next ;
      index++ ;
      if ( index == max ) {
        InstructionNode* temp = st.Access( st.Size() - 1 ) ;
        if ( temp -> tokenname == "." ) {
          return temp -> line ;
        } // if
      } // if
    } // while

    node.column = - 1 ;
    node.line = - 1 ;
    node.tokenname = "." ;
    node.tokentype = DOT ;
    tokengroup.Insert( tokengroup.Begin() + max, node ) ;
    max++ ;
    node.tokenname = "nil" ;
    node.tokentype = NIL ;
    tokengroup.Insert( tokengroup.Begin() + max, node ) ;
    max++ ;
    return max - 3 ;
  } // FitDOTPair()

  int FindDOtPairindex( Vector &tokengroup, int stindex, int endindex ) {
    int index = 0 ;
    int indexofDot = - 1 ;
    // stack < InstructionNode > st ;
    Vector st ;
    InstructionNode node ;
    InstructionNode* memofindex = tokengroup.Access( index ) ;
    while ( index < endindex ) {
      if ( memofindex -> tokenname == "(" && memofindex -> tokentype == SEXP ) {
        node.tokenname = "(" ;
        st.Push_back( node ) ;
      } // if
      else if ( memofindex -> tokenname == ")" && memofindex -> tokentype == SEXP ) {
        InstructionNode* temp = st.Access( st.Size() - 1 ) ;
        while ( temp -> tokenname != "(" ) {
          // st.pop() ;
          st.Erase( st.Size() - 1 ) ;
          temp = st.Access( st.Size() - 1 ) ;
        } // while

        // st.pop() ;
        st.Erase( st.Size() - 1 ) ;
      } // else if
      else if ( memofindex -> tokentype == DOT ) {
        node.tokenname = "." ;
        node.line = index ;
        st.Push_back( node ) ;
      } // else if

      memofindex = memofindex -> next ;
      index++ ;
      if ( index == endindex ) {
        InstructionNode* temp = st.Access( st.Size() - 1 ) ;
        if ( temp -> tokenname == "." ) {
          return temp -> line ;
        } // if
      } // if
    } // while

    return -1 ;
  } // FindDOtPairindex()

  TreeNode * CreateTree( Vector tokengroup, int stindex, int endindex ) {
    TreeNode * head = new TreeNode;
    head -> name = " " ;
    head -> type = TREE ;
    head -> left = NULL ;
    head -> right = NULL ;
    head->con = false;
    head->e = false;
    int index = 0 ;

    int dotindex = FindDOtPairindex( tokengroup, stindex, endindex ) ;
    if ( dotindex == -1 ) {
      return NULL ;
    } // if

    InstructionNode* memofbedotindex = tokengroup.Access( dotindex - 1 ) ;
    if ( memofbedotindex -> tokentype == SEXP ) { //  tokengroup[ dotindex - 1 ].tokentype == SEXP
      TreeNode * lefttree = CreateTree( tokengroup, stindex + 1, dotindex - 1 ) ;
      head -> left = lefttree ;
    } // if
    else {
      TreeNode * left = new TreeNode;
      left -> name = memofbedotindex -> tokenname ;
      left -> type = memofbedotindex -> tokentype ;
      left->con = false ;
      left->e = false;
      left -> left = NULL ;
      left -> right = NULL ;
      head -> left = left ;
      head->con = false;
      head->e = false;
    } // else

    InstructionNode* memofafdotindex = tokengroup.Access( dotindex + 1 ) ;
    if ( memofafdotindex -> tokentype == SEXP ) {
      TreeNode * righttree = CreateTree( tokengroup, dotindex + 1, endindex - 1 ) ;
      head -> right = righttree ;
    } // if
    else {
      TreeNode * right = new TreeNode ;
      right -> name = memofafdotindex -> tokenname ;
      right -> type = memofafdotindex -> tokentype ;
      right->con = false ;
      right->e = false;
      right -> left = NULL ;
      right -> right = NULL ;
      head -> right = right ;
      head->con = false;
      head->e = false;
    } // else

    return head ;
  } // CreateTree()

  void Inorder( TreeNode * head ) {
    if ( head == NULL ) {
      cout << "null " ;
      return ;
    } // if

    Inorder( head -> left ) ;
    cout << head -> name << " " ;
    Inorder( head -> right ) ;
    return ;
  } // Inorder()

  void Printspace( int num ) {
    int index = 0 ;
    while ( index < num ) {
      cout << " " ;
      index++ ;
    } // while
  } // Printspace()

  bool NeedL( TreeNode *root ) {
    if ( root->left && root->right && root->left->type != NIL && root->right->type != NIL )
      return true;
    return false;
  } // NeedL()

  void Prettyprint( TreeNode * father, TreeNode * cur, int & spacenum, bool & lastisLEFTPAREN ) {
    Transfer transfer ;
    bool atom = false;
    if ( cur -> left != NULL && cur -> right != NULL 
         && cur -> left -> name == " " && cur -> right -> name == " " ) { // sub <- o -> sub 
      if ( lastisLEFTPAREN == false ) {
        Printspace( spacenum ) ;
      } // if

      cout << "( " ;
      spacenum += 2 ;
      lastisLEFTPAREN = true ;
      Prettyprint( cur, cur -> left, spacenum, lastisLEFTPAREN ) ;
      spacenum -= 2 ;
      Printspace( spacenum ) ;
      cout << ")\n" ;
      Prettyprint( cur, cur -> right, spacenum, lastisLEFTPAREN ) ;
    } // if
    else if ( cur -> left != NULL && cur -> right != NULL && 
              cur -> left -> name != " " && cur -> right -> name == " " ) { // atom <- o -> sub
      Prettyprint( cur, cur -> left, spacenum, lastisLEFTPAREN ) ;
      if ( cur -> left -> name == "quote" && cur -> left -> type != STRING ) {
        Printspace( spacenum ) ;
        atom = NeedL( cur->right );
        if ( atom ) { // Add for ''34
          spacenum += 2 ;
          cout << "( " ;
        } // if
        
        lastisLEFTPAREN = true ;
      } // if

      Prettyprint( cur, cur -> right, spacenum, lastisLEFTPAREN ) ;
      if ( cur -> left -> name == "quote" && cur -> left -> type != STRING ) {
        if ( atom ) {
          spacenum -= 2 ;
          Printspace( spacenum ) ;
          cout << ")\n" ;
        } // if
      } // if
    } // else if
    else if ( cur -> left != NULL && cur -> right != NULL 
              && cur -> left -> name == " " && cur -> right -> name != " " ) { // sub <- o -> atom
      if ( lastisLEFTPAREN == false ) {
        Printspace( spacenum ) ;
      } // if

      cout << "( " ;
      spacenum += 2 ;
      lastisLEFTPAREN = true ;
      Prettyprint( cur, cur -> left, spacenum, lastisLEFTPAREN ) ;
      lastisLEFTPAREN = false ;
      spacenum -= 2 ;
      Printspace( spacenum ) ;
      cout << ")\n" ;
      if ( cur -> right -> type != NIL ) {
        Printspace( spacenum ) ;
        cout << "." << endl ;
      } // if


      Prettyprint( cur, cur -> right, spacenum, lastisLEFTPAREN ) ;
    } // else if
    else if ( cur -> left != NULL && cur -> right != NULL 
              && cur -> left -> name != " " && cur -> right -> name != " " ) { // atom <- o -> atom
      Prettyprint( cur, cur -> left, spacenum, lastisLEFTPAREN ) ;
      Prettyprint( cur, cur -> right, spacenum, lastisLEFTPAREN ) ;
    } // else if
    else if ( cur -> left == NULL && cur -> right == NULL ) { // this is leaf
      if ( father -> right == cur && cur -> type == NIL ) { // origin father -> left -> type == INT
        if ( father -> left != NULL && father -> left -> type == SYMBOL 
             && father -> left -> name == "quote" ) {
          Printspace( spacenum ) ;
          cout << "nil\n" ;
        } // if

        return ;
      } // if
      else if ( father -> left == cur && father -> right != NULL &&
                ( father -> right -> type == STRING || father -> right -> type == INT || 
                  father -> right -> type == FLOAT || father -> right -> type == T || 
                  father -> right -> type == SYMBOL || father ->right->type == RES ) ) { // left <- o
        if ( lastisLEFTPAREN == false ) {
          Printspace( spacenum ) ;
        } // if

        if ( cur -> type == FLOAT ) {
          double floatnum = atof( cur -> name.c_str() ) ;
          floatnum = round( floatnum * 1000 ) / 1000;
          printf( "%.3f\n", floatnum ) ;
        } // if
        else if ( cur -> type == STRING ) {
          cout << "\"" << cur -> name << "\"" << endl ;
        } // else if
        else if ( cur -> type == INT ) {
          if ( cur -> name != "quote" && cur -> type != STRING )
            cout << transfer.Strtoint( cur -> name ) << endl ;
          else 
            cout << cur -> name << endl ;
        } // else if
        else if ( cur->type == RES ) {
          cout << "#<procedure " << cur->name << ">" << endl ;
        } // else if
        else {
          cout << cur -> name << endl ;
        } // else 

        if ( cur -> name == "quote" ) {
          if ( cur -> type == STRING ) {
            Printspace( spacenum ) ;
            cout << "." << endl ;
          }  // if
        } // if 
        else {
          Printspace( spacenum ) ;
          cout << "." << endl ;
        } // else

      } // else if
      else { // o -> right
        if ( lastisLEFTPAREN != true ) {
          Printspace( spacenum ) ;
        } // if
        
        if ( cur -> type == FLOAT ) {
          double floatnum = atof( cur -> name.c_str() ) ;
          floatnum = round( floatnum * 1000 ) / 1000 ;
          printf( "%.3f\n", floatnum ) ;
        } // if
        else if ( cur -> type == STRING ) {
          cout << "\"" << cur -> name << "\"" << endl ;
        } // else if
        else if ( cur -> type == INT ) {
          if ( cur -> name != "quote" && cur -> type != STRING )
            cout << transfer.Strtoint( cur -> name ) << endl ;
          else 
            cout << cur -> name << endl ;
        } // else if
        else if ( cur->type == RES ) {
          cout << "#<procedure " << cur->name << ">" << endl ;
        } // else if
        else
          cout << cur -> name << endl ;
      } // else

      lastisLEFTPAREN = false ;
    } // else if
  } // Prettyprint()

  void Num( TreeNode *root ) {
    if ( !root )
      return ;

    if ( root && root->type == INT ) {
      stringstream ss;
      int c = atoi( root->name.c_str() );
      ss << c ;
      root->name = ss.str();
    } // if
    else if ( root && root->type == FLOAT ) {
      stringstream ss;
      float c = atof( root->name.c_str() );
      c = round( c * 1000 ) / 1000 ;
      ss << fixed << setprecision( 3 ) << c ;
      root->name = ss.str();
    } // else if
    else if ( root && root->type == STRING ) {
      root->name = "\"" + root->name + "\"" ;
    } // else if 

    Num( root->left );
    Num( root->right ) ;
  } // Num()

  void Myp( TreeNode* root, bool l, bool p ) { 
    if ( !root ) {
      gspa -= 2;
      Printspace( gspa );
      cout << ")\n" ;
      gp = true;
    } // if

    if ( l ) {
      if ( gp ) 
        Printspace( gspa ) ;
      gspa += 2 ;
      cout << "( " ;
      gp = false;
    } // if

    if ( root->type == NIL ) {
      gspa -= 2;
      Printspace( gspa ) ;
      cout << ")\n" ;
      gp = true;
      return ;
    } // if
    else if ( Ato( root ) ) {
      Printspace( gspa );
      Atomp( root );
      gspa -= 2;
      Printspace( gspa );
      cout << ")\n" ;
      gp = true ;
    } // else if 


    if ( root->left && Ato( root->left ) ) {
      if ( gp )
        Printspace( gspa ) ;
      Atomp( root->left );
      gp = true;
    } // if

    if ( root->type == QUOTE ) {
      if ( gp )
        Printspace( gspa );
      cout << "( QUOTE\n";
      gp = true;
    } // if 

    if ( root->left && !Ato( root->left )  ) 
      Myp( root->left, true, p );

    if ( root->right && Ato( root->right ) && root->right->type != NIL ) {
      Printspace( gspa );
      cout << ".\n" ;
      gp = true;
    } // if
    
    if ( root->right )
      Myp( root->right, false, p ) ;


  } // Myp()


  void Atomp( TreeNode *root ) {
    if ( root && root->type == FLOAT ) {
      double f = atof( root->name.c_str() );
      f = round( f * 1000 ) / 1000 ;
      cout << fixed << setprecision( 3 ) << f << endl ;
    } // if 
    else if ( root && root->type == NIL ) {
      cout << "nil\n" ;
    } // else if 
    else if ( root && root->type == T ) {
      cout << "#t\n" ;
    } // else if
    else if ( root && root->type == STRING ) {
      cout << root->name << "\n" ;
    } // else if
    else if ( root && root->type == INT ) {
      stringstream ss;
      int c = atoi( root->name.c_str() );
      ss << c ;
      cout << ss.str() << endl ;
    } // else if
    else if ( root && root->type == RES ) {
      cout << "#<procedure " << root->name << ">" << endl ;
    } // else if
    else if ( root ) {
      cout << root->name << endl ;
    } // else if
  } // Atomp()

  void Aliases( TreeNode *root, bool lfirst ) {
    
    bool lnop = false;

    if ( !root )
      return ;

    // 在左子樹找到要替換的SYMBOL
    if ( root->left && !root->left->con && 
         root->left->type == SYMBOL &&
         mdefmap.find( root->left->name ) != mdefmap.end() ) { 

      if ( gte.find( root->left->name ) != gte.end() )
        ge = gte[root->left->name] ;
      else 
        ge = false;

      root->left = mdefmap[root->left->name] ;
      if ( root->left->name == "quote" )
        return ;

      // nop 表示換過就不用再檢查了
      lnop = true;
    } // if 
    else if ( root->left && !root->left->con && 
              root->left->type == SYMBOL &&
              gResmap.find( root->left->name ) != gResmap.end() ) {

      root->left = gResmap[root->left->name] ;
      if ( root->left->name == "quote" ) 
        return ;

      lnop = true ;
    } // else if

    if ( root->right && root->right->type == SYMBOL && Defcheck( root->right->name ) ) {
      root->right = mdefmap[root->right->name] ;
      
      // right 有值代表一定是最後一個
      return ;
    } // if

    if ( lfirst && root->left && root->left->name == "define" )
      root->right->left->con = true;


    if ( !lnop && root->left )
      Aliases( root->left, true );
    if ( root->right )
      Aliases( root->right, false );

  } // Aliases()

  bool Defcheck( TreeNode* node ) {
    return ( mdefmap.find( node->name ) != mdefmap.end() ) ;
  } // Defcheck()

  bool Defcheck( string name ) {
    return ( mdefmap.find( name ) != mdefmap.end() ) ;
  } // Defcheck()

  TreeNode *Def( TreeNode *root ) {
    string defsym = root->right->left->name;
    TreeNode *val = root->right->right->left;
    
    if ( val->name == "exit" )
      gte[defsym] = true;
    else
      gte[defsym] = false;

    val = Eval( val );
    mdefmap[defsym] = val ;

    TreeNode * result = new TreeNode;
    result->name = defsym + " defined" ;
    result->type = SYMBOL;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ; 

  } // Def()

  TreeNode * Li( TreeNode *root ) {
    TreeNode * result = root->right;
    TreeNode * cur = root->right;
    TreeNode *t = root->right;
    while ( t->type != NIL ) {
      cur = t->left;
      cur = Eval( cur );
      t->left = cur;
      t = t->right;
    } // while

    if ( !Ato( result ) )
      result->type = LIST;
    
    return result ;
  } // Li()

  TreeNode * Car( TreeNode * root ) {
    TreeNode * cur = root->right ;
    cur = Eval( cur->left ) ;
    root->right->left = cur;
    return cur->left ;
    // return Eval( cur->left )->left;
  } // Car()

  TreeNode* Cdr( TreeNode *root ) {
    TreeNode * cur = root->right;
    cur = Eval( cur->left );
    root->right->left = cur;
    return cur->right ;
    // return Eval( cur->left )->right; 
  } // Cdr()

  TreeNode * Cons( TreeNode *root ) {
    TreeNode *num = root->right;
    int args = 0;
    TreeNode * result = new TreeNode;
    while ( num->type != NIL ) {
      args++;
      num = num->right;
    } // while

    if ( args != 2 ) {
      result->name = "ERROR (incorrect number of arguments) : cons";
      result->type = SYMBOL ;
      result->left = NULL;
      result->right = NULL;
      result->con = false ;
      result->e = false;
      throw result;
    } // if


    TreeNode * first = root->right->left;
    TreeNode * second = root->right->right->left ;

    first = Eval( first );
    second = Eval( second );
    result->type = TREE;
    result->name = " " ;
    result->left = first ;
    result->right = second ;
    result->con = false;
    result->e = false;

    return result ;
  } // Cons()

  TreeNode * CEvi() {
    mdefmap.clear();
    TreeNode *result = new TreeNode;
    result->type = SYMBOL;
    result->name = "environment cleaned";
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false ;
    return result ; 
  } // CEvi()

  TreeNode *Ex() {
    TreeNode *result = new TreeNode;
    result->type = EXIT;
    result->name = "exit";
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    msysstop = true;
    return result ; 
  } // Ex()

  TreeNode *Qu( TreeNode *root ) {
    Setcon( root->right );
    return root->right->left ;
  } // Qu()

  void Setcon( TreeNode *root ) {
    if ( root )
      root->con = true;
    else 
      return ;

    Setcon( root->left );
    Setcon( root->right );
  } // Setcon()

  TreeNode *Pa( TreeNode *root ) {
    TreeNode *check = root->right->left;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;

    if ( check && ( check->type == TREE || check->type == LIST ) ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if 
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if 

    return result;
  } // Pa()

  TreeNode *Nu( TreeNode *root ) {
    TreeNode *check = root->right->left;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;

    if ( check && check->type == NIL ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if 
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if 

    return result;
  } // Nu()

  TreeNode *Int( TreeNode *root ) {
    TreeNode * check = root->right->left;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;
    if ( check && check->type == INT ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if 

    return result ;
  } // Int()

  TreeNode *Renum( TreeNode *root ) {
    TreeNode *check = root->right->left ;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;
    if ( check && ( check->type == INT || check->type == FLOAT ) ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if

    return result ;
  } // Renum()

  TreeNode *Str( TreeNode *root ) {
    TreeNode *check = root->right->left ;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;
    if ( check &&  check->type == STRING ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if

    return result ;
  } // Str()

  TreeNode *Boo( TreeNode *root ) {
    TreeNode *check = root->right->left ;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;
    if ( check && ( check->type == T || check->type == NIL ) ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if

    return result ;
  } // Boo()

  TreeNode *Sym( TreeNode *root ) {
    TreeNode *check = root->right->left ;
    TreeNode *result = new TreeNode;

    check = Eval( check ) ;
    if ( check && check->type == SYMBOL ) {
      result->name = "#t";
      result->type = T;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // if
    else if ( check ) {
      result->name = "nil";
      result->type = NIL;
      result->right = NULL;
      result->left = NULL;
      result->con = false;
      result->e = false;
    } // else if

    return result ;
  } // Sym()

  TreeNode *Add( TreeNode *root ) {
    TreeNode * result = new TreeNode;
    bool f = false;
    result->name = "0" ;
    result->type = INT;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    TreeNode *check = root->right ;
    TreeNode * t ;
    bool set = false ;
    bool cf = false;
    float tn, rn = 0;
    int tni, rni ;

    while ( check && check->type != NIL ) {
      t = check->left;
      t = Eval( t );
      if ( t->type == FLOAT )
        f = true;
      if ( !set ) {
        if ( f ) {
          rn = atof( t->name.c_str() );
          // rn = round( rn * 1000 ) / 1000.0 ;
          cf = true;
        } // if
        else 
          rni = atoi( t->name.c_str() );
        
        set = true;
      } // if
      else {
        if ( f ) {
          if ( !cf ) {
            rn = ( float ) rni ;
            cf = true ;
          } // if

          tn = atof( t->name.c_str() );
          // tn = round( tn * 1000 ) / 1000.0 ;
          rn += tn ;
        } // if
        else {
          tni = atoi( t->name.c_str() ) ;
          rni += tni ;
        } // else
      } // else

      check = check->right;
    } // while
    
    if ( f ) 
      rn = round( rn * 1000 ) / 1000.0 ;

    if ( f ) {
      stringstream ss;
      ss << fixed << setprecision( 3 ) << rn ;
      result->name = ss.str() ;
      result->type = FLOAT;
    } // if
    else {
      stringstream ss;
      ss << rni;
      result->name = ss.str() ;
    } // else 
   
    return result;
  } // Add()

  TreeNode *Su( TreeNode *root ) {
    TreeNode * result = new TreeNode;
    bool f = false;
    result->name = "0" ;
    result->type = INT;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    TreeNode *check = root->right ;
    TreeNode * t ;
    bool set = false ;
    bool cf = false;
    float tn, rn = 0;
    int tni, rni ;

    while ( check && check->type != NIL ) {
      t = check->left;
      t = Eval( t );
      if ( t->type == FLOAT )
        f = true;
      if ( !set ) {
        if ( f ) {
          rn = atof( t->name.c_str() );
          // rn = round( rn * 1000 ) / 1000.0 ;
          cf = true;
        } // if
        else 
          rni = atoi( t->name.c_str() );
        
        set = true;
      } // if
      else {
        if ( f ) {
          if ( !cf ) {
            rn = ( float ) rni ;
            cf = true ;
          } // if

          tn = atof( t->name.c_str() );
          // tn = round( tn * 1000 ) / 1000.0 ;
          rn -= tn ;
        } // if
        else {
          tni = atoi( t->name.c_str() ) ;
          rni -= tni ;
        } // else
      } // else

      check = check->right;
    } // while
    
    if ( f ) 
      rn = round( rn * 1000 ) / 1000.0 ;

    if ( f ) {
      stringstream ss;
      ss << fixed << setprecision( 3 ) << rn ;
      result->name = ss.str() ;
      result->type = FLOAT;
    } // if
    else {
      stringstream ss;
      ss << rni;
      result->name = ss.str() ;
    } // else 

    return result ;
  } // Su()

  TreeNode *Mu( TreeNode *root ) {
    TreeNode * result = new TreeNode;
    bool f = false;
    result->name = "0" ;
    result->type = INT;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    TreeNode *check = root->right ;
    TreeNode * t ;
    bool set = false ;
    bool cf = false;
    float tn, rn = 0;
    int tni, rni ;

    while ( check && check->type != NIL ) {
      t = check->left;
      t = Eval( t );
      if ( t->type == FLOAT )
        f = true;
      if ( !set ) {
        if ( f ) {
          rn = atof( t->name.c_str() );
          // rn = round( rn * 1000 ) / 1000.0 ;
          cf = true;
        } // if
        else 
          rni = atoi( t->name.c_str() );
        
        set = true;
      } // if
      else {
        if ( f ) {
          if ( !cf ) {
            rn = ( float ) rni ;
            cf = true ;
          } // if

          tn = atof( t->name.c_str() );
          // tn = round( tn * 1000 ) / 1000.0 ;
          rn *= tn ;
        } // if
        else {
          tni = atoi( t->name.c_str() ) ;
          rni *= tni ;
        } // else
      } // else

      check = check->right;
    } // while
    
    if ( f ) 
      rn = round( rn * 1000 ) / 1000.0 ;

    if ( f ) {
      stringstream ss;
      ss << fixed << setprecision( 3 ) << rn ;
      result->name = ss.str() ;
      result->type = FLOAT;
    } // if
    else {
      stringstream ss;
      ss << rni;
      result->name = ss.str() ;
    } // else 

    return result ;
  } // Mu()

  TreeNode *Di( TreeNode *root ) {
    TreeNode * result = new TreeNode;
    bool f = false;
    result->name = "0" ;
    result->type = INT;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    TreeNode *check = root->right ;
    TreeNode * t ;
    bool set = false ;
    bool cf = false;
    float tn, rn = 0;
    int tni, rni ;

    while ( check && check->type != NIL ) {
      t = check->left;
      t = Eval( t );
      if ( t->type == FLOAT )
        f = true;
      if ( !set ) {
        if ( f ) {
          rn = atof( t->name.c_str() );
          // rn = round( rn * 1000 ) / 1000.0 ;
          cf = true;
        } // if
        else 
          rni = atoi( t->name.c_str() );
        
        set = true;
      } // if
      else {
        if ( f ) {
          if ( !cf ) {
            rn = ( float ) rni ;
            cf = true ;
          } // if
          
          tn = atof( t->name.c_str() );
          // tn = round( tn * 1000 ) / 1000.0 ;
          rn /= tn ;
        } // if
        else {
          tni = atoi( t->name.c_str() ) ;
          rni /= tni ;
        } // else
      } // else

      check = check->right;
    } // while
    
    if ( f ) 
      rn = round( rn * 1000 ) / 1000.0 ;

    if ( f ) {
      stringstream ss;
      ss << fixed << setprecision( 3 ) << rn ;
      result->name = ss.str() ;
      result->type = FLOAT;
    } // if
    else {
      stringstream ss;
      ss << rni;
      result->name = ss.str() ;
    } // else 

    return result ;   
  } // Di()

  TreeNode *Not( TreeNode *root ) {
    TreeNode *first = root->right->left;
    TreeNode *result = new TreeNode;


    first = Eval( first );

    if ( first->type == NIL ) {
      result->name = "#t";
      result->type = T ;
      result->left = NULL;
      result->right = NULL;
      result->con = false;
      result->e = false;
    } // if 
    else {
      result->name = "nil";
      result->type = NIL ;
      result->left = NULL;
      result->right = NULL;
      result->con = false;
      result->e = false;
    } // else

    return result ;
  } // Not()

  TreeNode *Gr( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t ;
    double tn, rn;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        rn = atof( t->name.c_str() );
        rn = round( rn * 1000 ) / 1000;
        set = true;
      } // if
      else {
        tn = atof( t->name.c_str() ) ;
        tn = round( tn * 1000 ) / 1000 ;
        if ( rn <= tn )
          meet = false ;
        rn = tn;
      } // else 

      check = check->right;
    } // while

    if ( meet ) {
      result->name = "#t" ;
      result->type = T;
    } // if 
    else {
      result->name = "nil";
      result->type = NIL;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Gr()

  TreeNode *Gre( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t ;
    double tn, rn;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        rn = atof( t->name.c_str() );
        rn = round( rn * 1000 ) / 1000;
        set = true;
      } // if
      else {
        tn = atof( t->name.c_str() ) ;
        tn = round( tn * 1000 ) / 1000 ;
        if ( rn < tn )
          meet = false ;
        rn = tn;
      } // else 

      check = check->right;
    } // while

    if ( meet ) {
      result->name = "#t" ;
      result->type = T;
    } // if 
    else {
      result->name = "nil";
      result->type = NIL;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Gre()

  TreeNode *Le( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t ;
    double tn, rn;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        rn = atof( t->name.c_str() );
        rn = round( rn * 1000 ) / 1000;
        set = true;
      } // if
      else {
        tn = atof( t->name.c_str() ) ;
        tn = round( tn * 1000 ) / 1000 ;
        if ( rn >= tn )
          meet = false ;
        rn = tn;
      } // else 

      check = check->right;
    } // while

    if ( meet ) {
      result->name = "#t" ;
      result->type = T;
    } // if 
    else {
      result->name = "nil";
      result->type = NIL;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Le()

  TreeNode *Lee( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t ;
    double tn, rn;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        rn = atof( t->name.c_str() );
        rn = round( rn * 1000 ) / 1000;
        set = true;
      } // if
      else {
        tn = atof( t->name.c_str() ) ;
        tn = round( tn * 1000 ) / 1000 ;
        if ( rn > tn )
          meet = false ;
        rn = tn;
      } // else 

      check = check->right;
    } // while

    if ( meet ) {
      result->name = "#t" ;
      result->type = T;
    } // if 
    else {
      result->name = "nil";
      result->type = NIL;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Lee()

  TreeNode *Sa( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t ;
    double tn, rn;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        rn = atof( t->name.c_str() );
        rn = round( rn * 1000 ) / 1000;
        set = true;
      } // if
      else {
        tn = atof( t->name.c_str() ) ;
        tn = round( tn * 1000 ) / 1000 ;
        if ( rn != tn )
          meet = false ;
        rn = tn;
      } // else 

      check = check->right;
    } // while

    if ( meet ) {
      result->name = "#t" ;
      result->type = T;
    } // if 
    else {
      result->name = "nil";
      result->type = NIL;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Sa()

  TreeNode *Strapp( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t ;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      result->name += t->name ;
      check = check->right ;
    } // while 

    result->type = STRING;
    result->left = NULL ;
    result->right = NULL ;
    result->con = false;
    result->e = false;

    return result;
  } // Strapp()

  TreeNode *Strgr( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t;
    string ts;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        ts = t->name ;
        set = true;
      } // if
      else {
        if ( ts <= t->name )
          meet = false ;
        ts = t->name ;
      } // else 

      check = check->right ;
    } // while 

    if ( meet ) {
      result->name = "#t" ;
      result->type = T ;
    } // if
    else {
      result->name = "nil" ;
      result->type = NIL ;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Strgr()

  TreeNode *Strle( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t;
    string ts;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        ts = t->name ;
        set = true;
      } // if
      else {
        if ( ts >= t->name )
          meet = false ;
        ts = t->name ;
      } // else 

      check = check->right ;
    } // while 

    if ( meet ) {
      result->name = "#t" ;
      result->type = T ;
    } // if
    else {
      result->name = "nil" ;
      result->type = NIL ;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Strle()

  TreeNode *Strsa( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *check = root->right;
    TreeNode *t;
    string ts;
    bool set = false ;
    bool meet = true;

    while ( check && check->type != NIL ) {
      t = Eval( check->left );
      if ( !set ) {
        ts = t->name ;
        set = true;
      } // if
      else {
        if ( ts != t->name )
          meet = false ;
        ts = t->name ;
      } // else 

      check = check->right ;
    } // while 

    if ( meet ) {
      result->name = "#t" ;
      result->type = T ;
    } // if
    else {
      result->name = "nil" ;
      result->type = NIL ;
    } // else 

    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;

    return result ;
  } // Strsa()
  
  TreeNode *Eqv( TreeNode *root ) {
    TreeNode* result = new TreeNode;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    TreeNode *first = root->right->left;
    TreeNode *second = root->right->right->left;

    first = Eval( first );
    second = Eval( second );

    if ( first && second && first->type == TREE && second->type == TREE ) {
      if ( first == second ) {
        result->name = "#t";
        result->type = T;
      } // if
      else {
        result->name = "nil";
        result->type = NIL;
      } // else 
    } // if
    else if ( first && second && first->name == second->name && first->type == second->type ) {
      if ( first->type == STRING ) {
        result->name = "nil";
        result->type = NIL;
      } // if 
      else {
        result->name = "#t";
        result->type = T;
      } // else 
    } // else if
    else {
      result->name = "nil" ;
      result->type = NIL;
    } // else

    return result;

  } // Eqv()

  TreeNode *Equ( TreeNode *root ) { 
    TreeNode *result = new TreeNode;
    TreeNode *first = root->right->left ;
    TreeNode *second = root->right->right->left ;
    result->con = false;
    result->e = false;

    first = Eval( first ) ;
    second = Eval( second ) ;

    result = Equ( first, second ) ;

    return result;
  } // Equ()

  TreeNode *Equ( TreeNode *f, TreeNode* s ) {
    TreeNode *result = new TreeNode;
    result->name = "#t";
    result->type = T;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    result->e = false;
    if ( f && s && f->type == NIL && s->type == NIL )
      return result;
    
    if ( f && s && f->name != s->name ) {
      result->name = "nil";
      result->type = NIL;
      return result;
    }  // if 

    if ( f && s && f->type != s->type ) {
      result->name = "nil";
      result->type = NIL;
      return result;
    } // if

    if ( f && s && f->left && s->left )
      result = Equ( f->left, s->left );
    else if ( f && s && ( ( f->left && !s->left ) || ( !f->left && s->left ) ) ) {
      result->name = "nil";
      result->type = NIL;
    } // else if
      

    if ( result && result->type == NIL )
      return result ;
      
    if ( f && s && f->right && s->right )
      result = Equ( f->right, s->right );
    else if ( ( f && s && f->right && s->right ) 
              && ( ( f->right && !s->right ) || ( !f->right && s->right ) ) ) {
      result->name = "nil";
      result->type = NIL;
    } // else if

    return result;
      
  } // Equ()

  TreeNode *If( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    result->left = NULL;
    result->right = NULL;
    result->con = false ;
    result->e = false;
    TreeNode *exp = root->right;
    TreeNode *suc = exp->right;
    TreeNode *nag = exp->right->right;
    TreeNode *check = Eval( exp->left );

    if ( check->type != NIL ) 
      result = Eval( suc->left );
    else if ( nag )
      result = Eval( nag->left );

    return result ;
  } // If()

  TreeNode *Cond( TreeNode* root ) {
    TreeNode *result = new TreeNode ;
    result->left = NULL;
    result->right = NULL;
    result->con = false;
    TreeNode *check = root->right ;
    TreeNode *exp ;
    TreeNode *suc ;

    while ( check && check->type != NIL ) {
      exp = Eval( check->left->left );
      if ( ( exp && exp->type != NIL ) || ( exp->type == NIL && check && check->right->type == NIL ) ) {
        suc = check->left->right ;
        while ( suc && suc->type != NIL ) {
          result = Eval( suc->left );
          suc = suc->right;
        } // while

        return result;
      } // if

      check = check->right;
    } // while

    return result ;
  } // Cond()

  TreeNode *Be( TreeNode* root ) {
    TreeNode * result = new TreeNode;
    TreeNode *cur = root->right;

    while ( cur && cur->type != NIL ) {
      result = Eval( cur->left ) ;
      cur = cur->right;
    } // while


    return result ;
  } // Be()

  TreeNode *And( TreeNode* root ) {
    TreeNode *result = new TreeNode;
    TreeNode *cur = root->right ;

    while ( cur && cur->type != NIL ) {
      result = Eval( cur->left ) ;
      if ( result && result->type == NIL ) {
        return result ;
      } // if
      
      cur = cur->right ;
    } // while

    return result;

  } // And()

  TreeNode *Or( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    TreeNode *cur = root->right ;

    while ( cur && cur->type != NIL ) {
      result = Eval( cur->left ) ;
      if ( result && result->type != NIL ) {
        return result;
      } // if

      cur = cur->right ;
    } // while 

    return result ;
  } // Or()

  bool Ato( TreeNode *root ) {
    if ( root && !root->left && !root->right )
      return true;
    return false;
  } // Ato()

  TreeNode *Fleafunc( TreeNode *root ) {
    if ( root->left && root->left->type != RES ) {
      root->left = Fleafunc( root->left );
      return root ;
      // t = Eval( t );
    } // if
    else {
      return Eval( root ) ;
    } // else 

    return root ;
  } // Fleafunc()

  bool Isfunc( TreeNode *root ) {
    if ( Ato( root ) ) 
      return false;
    
    if ( root->left->type != RES )
      Fleafunc( root );
    string func = root->left->name;
    

    if ( func == "quote" || func == "cons" || func == "car" || func == "cdr" || func == "list"
         || func == "define" || func == "clean-environment" || func == "pair?" || func == "null?"
         || func == "integer?" ||  func == "real?" || func == "number?" || func == "string?" 
         || func == "boolean?" || func == "symbol?" || func == "+" || func == "-" || func == "*"
         || func == "/" || func == "not" || func == ">" || func == "<" || func == "=" 
         || func == ">=" || func == "<=" || func == "string-append" || func == "string>?" 
         || func == "string<?" || func == "string=?" || func == "eqv?" || func == "equal?"
         || func == "if" || func == "cond" || func == "begin" || func == "and" || func == "or" 
         || func == "exit" || func == "atom?" ) {

      return true;
    } // if

    return false ;
  } // Isfunc()

  TreeNode *Atom( TreeNode *root ) {
    TreeNode *result = new TreeNode;
    result->left = NULL;
    result->right = NULL;
    result->e = false;
    result->con = false;
    TreeNode *check = root->right->left;

    if ( check && check->left == NULL && check->right == NULL ) {
      result->name = "#t";
      result->type = T;
      return result ;
    } // if

    result->name = "nil";
    result->type = NIL;
    return result ;
  } // Atom()

  void Qucheck( TreeNode * root ) {
    // 將'(3 4 5)的樹轉換成 (quote (3 4 5) )
  
    if ( !root )
      return ;

    if ( root && root->left && root->left->name == "quote" && root->left->type == INT ) {
      TreeNode *t = root->right;
      TreeNode *n ;
      root->left->type = SYMBOL;
      root->right = new TreeNode;
      n = root->right;
      n->con = false;
      n->name = " ";
      n->type = TREE;
      n->left = t;
      n->right = new TreeNode;
      n->right->name = "nil" ;
      n->right->type = NIL ;
      n->right->left = NULL;
      n->right->right = NULL;
      n->right->con = false ;
    } // if

    if ( !root->left && !root->right )
      return ;
    
    if ( root->left )
      Qucheck( root->left ) ;

    if ( root->right )
      Qucheck( root->right );

  } // Qucheck()

  TreeNode *Eval( TreeNode *root ) {
    if ( root && root->type == TREE && !root->con && root->left &&  root->left->type != RES )
      Fleafunc( root );
    if ( root && root->type == TREE && !root->con && root->left &&  root->left->type == RES  ) {
      
      string func = root->left->name ;
      if ( func == "define" ) {
        return Def( root );
      } // if
      else if ( func == "cons" ) {
        return Cons( root );
      } // else if
      else if ( func == "car" ) {
        return Car( root );
      } // else if 
      else if ( func == "quote" ) {
        // root = Qucheck( root );
        // root->left->type = QUOTE;
        return Qu( root );
      } // else if
      else if ( func == "cdr" ) {
        return Cdr( root );
      } // else if
      else if ( func == "list" ) {
        return Li( root ); 
      } // else if
      else if ( func == "atom?" ) {
        return Atom( root );
      } // else if 
      else if ( func == "pair?" ) {
        return Pa( root );
      } // else if 
      else if ( func == "null?" ) {
        return Nu( root );
      } // else if 
      else if ( func == "integer?" ) {
        return Int( root );
      } // else if 
      else if ( func == "real?" || func == "number?" ) {
        return Renum( root ) ;
      } // else if
      else if ( func == "string?" ) {
        return Str( root ) ;
      } // else if
      else if ( func == "boolean?" ) {
        return Boo( root );
      } // else if 
      else if ( func == "symbol?" ) {
        return Sym( root ) ;
      } // else if
      else if ( func == "+" ) {
        return Add( root ) ;
      } // else if
      else if ( func == "-" ) {
        return Su( root ) ;
      } // else if 
      else if ( func == "*" ) {
        return Mu( root );
      } // else if
      else if ( func == "/" ) {
        return Di( root );
      } // else if
      else if ( func == "not" ) {
        return Not( root ) ;
      } // else if
      else if ( func == ">" ) {
        return Gr( root );
      } // else if 
      else if ( func == "<" ) {
        return Le( root );
      } // else if 
      else if ( func == "=" ) {
        return Sa( root );
      } // else if 
      else if ( func == ">=" ) {
        return Gre( root );
      } // else if 
      else if ( func == "<=" ) {
        return Lee( root );
      } // else if 
      else if ( func == "string-append" ) {
        return Strapp( root );
      } // else if 
      else if ( func == "string>?" ) {
        return Strgr( root ); 
      } // else if
      else if ( func == "string<?" ) {
        return Strle( root );
      } // else if 
      else if ( func == "string=?" ) {
        return Strsa( root );
      } // else if
      else if ( func == "eqv?" ) {
        return Eqv( root );
      } // else if
      else if ( func == "equal?" ) {
        return Equ( root );
      } // else if 
      else if ( func == "if" ) {
        return If( root );
      } // else if 
      else if ( func == "cond" ) {
        return Cond( root );
      } // else if
      else if ( func == "begin" ) {
        return Be( root ) ;
      } // else if
      else if ( func == "and" ) {
        return And( root );
      } // else if
      else if ( func == "or" ) {
        return Or( root );
      } // else if
      else if ( func == "exit" && root->right->type == NIL && ge ) {
        return Ex() ;
      } // else if
      else if ( func == "clean-environment" && root->right->type == NIL ) {
        return CEvi();
      } // else if 
    } // if

    return root ;
  } // Eval()
}; // Scaner()

int main() {
  Scaner scaner ;
  CrRmap();
  cout << "Welcome to OurScheme!\n" << endl ;
  scaner.Interpreter() ;
  cout << "Thanks for using OurScheme!" ;
} // main()
