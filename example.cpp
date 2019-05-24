// ConsoleApplication6.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "serialize.h"
using namespace std;

struct test
{
	int a;
	double b;

	void pack(Serialize& ar)
	{
		ar << a;
		ar << b;
	}

	void unpack(Serialize& ar)
	{
		ar >> a;
		ar >> b;
	}
};

struct Person
{
	char c;
	int age;
	double score;
	char name[32];
	std::string address;
	std::vector<string> v;
	std::map<int, std::vector<string>> m;
	std::list<int> l;
	test t;
	Person()
	{
		memset(name, 0, sizeof(name));
		age = 0;
		score = 0;
	}

	void pack(Serialize& ar)
	{
		ar << c;
		ar << age;
		ar << score;
		ar << name;
		ar << address;
		ar << v;
		ar << m;
		ar << l;
		ar << t;
	}

	void unpack(Serialize& ar)
	{
		ar >> c;
		ar >> age;
		ar >> score;
		ar >> name;
		ar >> address;
		ar >> v;
		ar >> m;
		ar >> l;
		ar >> t;
	}
};

int main()
{
	Person p1, p2;
	p1.c = 'a';
	p1.age = 20;
	p1.score = 80;
	strcpy(p1.name, "zhangsan");
	p1.address = "china";
	p1.v = { "ffff1","ffff2","ffff3" };
	p1.m[1] = p1.v;
	p1.l = { 11,12,13 };
	p1.t = { 200,300 };
	
	Serialize s;
	p1.pack(s);
	p2.unpack(s);

	cout << p2.c << endl;
	cout << p2.age << endl;
	cout << p2.score << endl;
	cout << p2.name << endl;
	cout << p2.address.c_str() << endl;
	for (auto& d : p2.v)
	{
		cout << d << endl;
	}

	for (auto& d : p2.m)
	{
		cout << d.first << endl;
		for (auto& t : d.second)
		{
			cout << t.c_str() << endl;
		}
		
	}

	for (auto& d : p2.l)
	{
		cout << d << endl;
	}

	cout << p2.t.a << endl;
	cout << p2.t.b << endl;
}