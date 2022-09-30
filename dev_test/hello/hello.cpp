// hello.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include "config.h"
//#include "base64.h"
#include <vector>
#include <algorithm>
#include <set>


char format[] = "%s %s\n";
char hello[] = "Hello";
char world[] = "world";
using namespace std;

//vector<int>: sort and unique
void sort_unique_vec(const vector<int> &vecSrc, vector<int> &vecDes) {
	if (vecSrc.empty()) {
		return;
	}
	vecDes.clear();
	vecDes.assign(vecSrc.begin(), vecSrc.end());
	sort(vecDes.begin(), vecDes.end());
	auto iter_end = unique(vecDes.begin(), vecDes.end());
	vecDes.erase(iter_end, vecDes.end());
}

/*
* two vector<int> : set union
* if v_src and v_union are sorted vector, sort_flg = true (default),
* else, sort_flg = false.
* if v_res == v_union, return true,
* else, return false.
* [warning] : if v_src or v_union are not sorted vector, and set sort_flg = false,
* the process maybe coredump.
*/
bool vectors_set_union(const vector<int> &v_src, const vector<int> &v_union, vector<int> &v_res, bool sort_flg = true) {
	if ((v_src.empty()) || (v_union.empty())) {
		return false;
	}
	v_res.clear();
	if (sort_flg) {
		vector<int> v1(v_src);
		vector<int> v2(v_union);
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v_res));
	}
	else {
		set_union(v_src.begin(), v_src.end(), v_union.begin(), v_union.end(), back_inserter(v_res));
	}
	if (v_res != v_union) {
		return false;
	}
	return true;
}

void print_vector(const vector<int> &v){
	if (!v.empty()) {
		cout << "vector<int> : [";
		auto it = v.begin();
		for (; it != v.end(); ++it) {
			cout << *it << " ";
		}
		cout << "]" << endl;
	}
}

void get_string(char **p) {
	char *ret = new char[20];
	strcpy(ret, "hello word.");
	*p = ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	int i = 500000;
	while (i--) {
	__asm {
	mov     eax, offset world
	push    eax
	mov     eax, offset hello
	push    eax
	mov     eax, offset format
	push    eax
	call    printf
	pop     eax
	pop     eax
	pop     eax
	}
	//printf("%s %s\n", hello, world);
	//std::cout << hello << world << std::endl;
	}
	*/

	/*
	int n_sample;
	std::string s_sample;

	n_sample = GET_GOL_CONF.n_smaple;
	s_sample = GET_GOL_CONF.s_smaple;
	std::cout << "n_sample:" << n_sample << ", s_sample:" << s_sample << "." << std::endl;
	*/

	/*
	char *src = "ENgn7oEijwCAGp+AgBqsgIAisACACr+AIsAAgDrLAQ3PKeoa94CACgGngBoB34CAOgHhAQ3PGgHsgIAaAsyAgBoCzYCACgOngBoDqoCAIgPAAIAiBIsAgDoEnQENzwoF+oAKBryACgeXgBoH/ICAGgiBgIAKTq6ACk6vgApOuIAaTsCAgBpO3ICAGk7egIAaT7WAgDpP0gENzxpQroCAClDNgEJQzg89Z44aUayAgDpT9gENzzpUpgENzwpV3IAiVqoAgCIBHKAAgCIBHKEAgAoBHKKAGgEco4CAGgEcpICACgEcqIAiARypAIAiARyqAIAaARyxgIBCARy6Dz1njhoBHLuAgCIBHMMAgCkBHMkiARzLAIAaARzMgIAKARzNgCkBHNAiARzTAIApARzVIgEc1gCACgEc2YBCARzaDz1njhoBHNuAgAoBHNyAIgEc3QCAIgEc3gCAGgEc34CAGgEc4ICAGgEc4YCAQgEc4g89Z44aARzjgIAKARzkgAoBHOWACgEc5oAKARzngEIBHZoPPWeOGgEdm4CACgEdoYAaAR2igIBCAR2jDz1njhoBHaeAgCIBHagAgCIBHakAgCIBHaoAgCIBHcIAgCIBHcsAgCkBHcwiAR3NAIApAR3OKQEd0CIBHdEAgCkBHdIpAR3TIgEd1ACAKQEd1SIBHdYAgCIBHdcAgCkBHdgiAR3aAIAaAR3cgIAaAR3dgIAaAR3egIAaAR3fgIAaAR3ggIAaAR3hgIAaAR3igIAaAR3jgIAaAR3kgIAaAR3lgIApAR3mKQEd5ykBHegpAR3pKQEd6ikBHesKAR6zgA==";
	uint8_t *charIn = reinterpret_cast<uint8_t *>(src);
	uint32_t uLen = 0;
	uint8_t *pResult = sfa::utils::base64_decode(charIn, uLen);
	cout << *pResult << endl;
	*/

	/*
	vector<int> v1{ 1, 5, 3, 3, 7, 6, 9, 2, 2, 6, 9, 8, 6, 3, 4 };
	cout << "v1="; print_vector(v1);
	vector<int> v2{ 1, 5, 5, 8, 7, 6, 7, 8, 9, 2, 3, 4, 4, 5 };
	cout << "v2="; print_vector(v2);
	vector<int> v_ret;
	vector<int> v1_t;
	vector<int> v2_t;
	sort_unique_vec(v1, v1_t);
	cout << "v1_t="; print_vector(v1_t);
	sort_unique_vec(v2, v2_t);
	cout << "v2_t="; print_vector(v2_t);
	bool ret = vectors_set_union(v1_t, v2_t, v_ret, false);
	cout << "v_ret="; print_vector(v_ret);
	if (ret) {
		cout << "true" << endl;
	}
	else {
		cout << "false" << endl;
	}

	set<int> s_v;
	for (auto &it : v1) {
		s_v.insert(it);
	}
	cout << s_v.size() << endl;;

	string st1 = "1234,21313,23123,ahsn";
	cout << st1 << endl;
	st1.replace(st1.length() - 1, st1.length(), "_");
	cout << st1 << endl;
	*/

	/*
	char *pstr = nullptr;
	get_string(&pstr);
	if (pstr) {
		cout << pstr << endl;
		delete [] pstr;
		pstr = nullptr;
	}
	*/

	string s1 = "[5103.SSEC.20190830.112647l]";
	string::size_type s1_len = s1.length();
	string::size_type pos = s1.find_last_of(".");
	string s1_tmp = s1.substr(0, pos);
	pos = s1_tmp.find_last_of(".");
	string s2 = s1.substr(pos+1, s1_len-(pos+1)-1);
	
    system("pause");
	return 0;
}

