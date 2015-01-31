#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include<string>
#include <fstream>
#include<iomanip>
#include <sstream>
using namespace std;

#define rep(i, x, y) for(int i = x; i < y; i++)
#define Rep(i, x, y) for(int i = x; i <= y; i++)
#define vi vector<int>
#define vvi vector<vector<int> >
#define pp push_back
#define long long ll
#define unsigned long long ull
#define eps 1e-9

map< string, string > reg_io;
map< string, pair< char,char > > mem;
vector< vector< string > > Program;
vector< pair< string, string> > add_sym;
set< string > S;
string address;
bool fail;

string inc_add( string s )
{
	int l = s.length();
	bool done = false;
	for( int i=l-1 ; i>=0 ; i-- )
	{
		if( s[ i ] < '9' )
		{
			s[ i ] += 1;
			done = true;
		}
		else if ( s[ i ] == '9' )
		{
			s[ i ] = 'A' ;
			done = true;
		}
		else if ( s[ i ] < 'F' )
		{
			s[ i ] += 1;
			done = true;
		}
		else
			s[ i ] = '0';
		if( done )
			break;
	}
	if ( !done )
		s.insert( 0,"1");
	return s;
}

bool format_string( string st )
{
	vector< string > v;
	string s;
	stringstream sin;
	sin<<st;
	while( !sin.eof() )
	{
		sin>>s;
		while( s.find( ',' ) != st.npos )
		{
			if ( s[ 0 ] == ',' )
			{
				s.erase( 0, 1 );
				v.push_back( "," );
				continue;
			}
			string ss = s.substr( 0, s.find( ',' ) );
			s.erase( 0, s.find( ',' ) );
			for( int i=0 ; i<ss.length() ; i++ )
				ss[ i ] = tolower( ss[ i ] );
			v.push_back( ss );
		}		
		if ( s.size() != 0 )
		{
			for( int i=0 ; i<s.length() ; i++ )
				s[ i ] = tolower( s[ i ] );
			v.push_back( s );
		}
	}
	Program.push_back( v );
	if( v.size() == 1 && v[ 0 ] == "end" )
		return true;
	return false;
}

string get_address( string op )
{
	for( int i=0 ; i<add_sym.size() ; i++ )
		if( add_sym[ i ].first == op )
			return add_sym[ i ].second;
	return "notFound";
}

void begin()
{
	string word;
	char a, b;
	string c;
	ifstream fin ;
	fin.open("input.in");
	for( int i=0 ; i<18 ; i++ )
	{
		fin>>word>>c;
		reg_io[ word ] = c;
	}
	for( int i=0 ; i<7 ; i++ )
	{
		fin>>word>>a>>b;
		mem[ word ] = make_pair(a,b);
	}
	for( int i=0 ; i<4 ; i++ )
	{
		fin>>word;
		S.insert( word );
	}
	fin.close();
}

void hexaToDec( string s )
{
	string line = "", word;
	bool neg = false;
	if ( s[ 0 ] == '-' )
	{
		neg = true;
		s.erase( 0,1 );
	}
	for( int i =0 ; i<s.size() ; i++ )
	{
		word = "";
		if ( i )
			line += " ";
		char c ;
		int k,l;
		if ( s[ i ] <= '9' )
			k = s[ i ] - '0';
		else
			k = tolower( s[ i ] ) - 'a' + 10;
		while( k != 0 )
		{
			c = '0';
			l = k%2;
			k/=2;
			c += l;
			word = c + word;
		}
		while( word.size() < 4 )
			word = "0" + word;
		line += word;
	}
	int i = s.size() ;
	while( i != 4 )
	{
		line = "0000 " + line;
		i++;
	}
	if ( neg )
	{
		rep( i,0,line.size() )
		{
			if ( line[ i ] == '0' )
				line[ i ] = '1';
			else if ( line[ i ] == '1' )
				line[ i ] = '0';
		}
		for( int i= line.size() -1 ; i>=0 ; i-- )
		{
			if ( line[ i ] == '0' )
			{
				line[ i ] = '1';
				break;
			}
			else if ( line[ i ] == '1' )
				line[ i ] = '0';
		}
	}
	cout<<setw(25)<<line<<endl;
}

void first_pass()
{
	int i = 0, li = 1;
	if ( Program[ 0 ][ 0 ] == "org" )
	{
		address = Program[ 0 ][ 1 ];
		i++; li++;
	}
	else
		address = "0";
	for( ; i<Program.size() ; i++ )
	{
		vector< string > v;
		v = Program[ i ];
		if ( v.size() > 2 && v[ 1 ] == "," )
		{
			if( reg_io.find( v[ 0 ] ) != reg_io.end() || mem.find( v[ 0 ] ) != mem.end() || S.find( v[ 0 ] ) != S.end() )
			{
				fail = true;
				cout<<"Error at line "<< li <<endl;
				puts("Variable can't be a reserved word.\n");
				return;
			}
			else
				add_sym.push_back( make_pair( v[ 0 ], address ) );
		}
		address = inc_add( address );
		li++;
	}

	cout<<"==============================\n==== Address Symbol Table ====\n==============================\n";
	for ( i = 0 ; i<add_sym.size() ; i++ )
		cout<< setw(10 )<<add_sym[ i ].first<<setw( 10 )<<add_sym[ i ].second<<endl;
	cout<<"==============================\n";
}

void second_pass()
{
	int i = 0, li = 1;
	cout<<"========================================================"<<endl;
	cout<<setw(10)<<"address"<<setw(10)<<"inst."<<setw(10)<<"code"<<setw(25)<<"Binary code"<<endl;
	cout<<"========================================================"<<endl;
	bool label ;
	string line;
	if ( Program[ 0 ][ 0 ] == "org" )
	{
		address = Program[ 0 ][ 1 ];
		line = Program[ 0 ][ 0 ] + " " + Program[ 0 ][ 1 ];
		cout<<setw(20)<<line<<endl;
		i++; li++;
	}
	for ( ; i<Program.size() ; i++ )
	{
		vector< string > v;
		v = Program[ i ];
		int j = 0;
		if ( v.size() >= 2 && v[ 1 ] == "," )
			j+=2;
		if ( reg_io.find( v[ j ] ) != reg_io.end() )
		{
			if( v.size() > j+1 )
			{
				cout<<"Error at line "<<li<<endl;
				puts("This instruction needn't any memory reference.");
				return;
			}
			cout<<setw(10)<<address<<setw(10)<<v[ j ]<<setw(10)<<reg_io[ v[ j ] ];
			hexaToDec( reg_io[ v[j] ] );
		}
		else if ( mem.find( v[ j ] ) != mem.end() )
		{
			if ( v.size() == j+1 )
			{
				cout<<"Error at line "<<li<<endl;
				puts("variable is missing.");
				return;
			}
			line = v[ j ] + " " + v[ j+1 ];
			string add="", ww = get_address( v[ j+1 ] );
			if ( ww == "notFound" )
			{
				cout<<"Error at line "<<li<<".\n";
				puts( "Undefined variable." );
				return;
			}
			if ( v.size() == j+3  )
			{
				if ( v[ j+2 ] == "i" )
				{
					line += " " + v[ j+2 ];
					add.push_back( mem[ v[ j ] ].second );
				}
				else
				{
					cout<<"Error at line "<<li<<".\n";
					puts( "Expected I for indirect address." );
					return;
				}
			}
			else if ( v.size() == j+2 )
			{
				add.push_back( mem[ v[ j ] ].first );
			}
			else
			{
				cout<<"Error at line "<<li<<".\n";
				puts( "Excess labels." );
				return;
			}
			add += ww;
			cout<<setw(10)<<address<<setw(10)<<line<<setw(10)<<add;
			hexaToDec( add );
		}
		else if ( S.find( v[ j ] ) != S.end() )
		{
			if( v[ j ] == "org" || v[ j ] == "end" )
				cout<<setw(10)<<address<<setw(10)<<v[ j ]<<endl;
			else
			{
				line = v[ j ] + " " + v[ j+1 ];
				cout<<setw(10)<<address<<setw(10)<<line<<setw(10)<<v[j+1];
				hexaToDec( v[ j+1 ] );
			}
		}
		else
		{
			cout<<"Error at line "<<li<<".\n";
			puts( "Instruction not recognized.");
			return;
		}
		address = inc_add( address );
		li++;
	}
}


int main()
{
	//freopen( "input.in", "r", stdin );
	freopen( "output.out", "r", stdin );
	begin();
	while( true )
	{
		cout<<"Press 1 to enter a new program 2 for exit.\n";
		int n;
		cin>>n;
		cin.ignore();
		if ( n!=1 && n!=2 )
			continue;
		else if( n==2 )
			break;
		else
		{
			cout<<"Enter your assembly program : \n";
			string word;
			Program.clear();
			add_sym.clear();
			while( getline( cin, word ) )
				if ( format_string( word ) )
					break;
			fail = false;
			first_pass();
			if ( !fail )
				second_pass();
		}
	}
}