#ifndef SEMANTIC_H
#define SEMANTIC_H

#include<iostream>
#include<string>
#include<hash_map>
using namespace std;
//·ûºÅ±í
class SymTab
{
	struct string_hash{
		size_t operator()(const string &str) const{
			return __stl_hash_string(str.c_str());
		}
	};
	hash_map<string,vector<string>*,string_hash> varTab;

};



#endif // !SEMANTIC_H
