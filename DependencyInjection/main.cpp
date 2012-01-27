//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <inject.hpp>
#include <injectable.hpp>
#include <helpers.hpp>
#include <iocfactory.hpp>

struct D {
	virtual std::string getName() =0;
	virtual void print(){std::cout << "I am " << getName() << std::endl;}; 
};

struct D1:public D{virtual std::string getName(){return "D1";}}; 
struct D2:public D{virtual std::string getName(){return "D2";}}; 

struct D3:public D{
	D3(int k) : p(k){};
	virtual std::string getName(){
		return "D3";
	}
	virtual void print(){
		std::cout << "I am " << getName() << " number:" << p << std::endl;
	} 
	int p;
};

class TestClass : public Injectable<D1,D2,D3> {
public:
	Inject<D1,TestClass> some_var;
	Inject<D2,TestClass> some_var2;
	Inject<D3,TestClass> some_var3;

	void print() {
		some_var->print();
		some_var2->print();
		some_var3->print();
	}
};

class TestClass2 : public Injectable<D3,D3,D3> {
public:
	Inject<D3,TestClass2> some_var;
	Inject<D3,TestClass2> some_var2;
	Inject<D3,TestClass2> some_var3;

	void print() {
		some_var->print();
		some_var2->print();
		some_var3->print();
	}
};

class TestClass3 : public Injectable<D3,D3> {
public:
	Inject<D3,TestClass3> some_var;
	Inject<D3,TestClass3> some_var2;

	void print() {
		some_var->print();
		some_var2->print();
	}
};

int main() {
	D1 d1; D2 d2; D3 d3(1);
	IocFactory<TestClass> factory;
	factory.use(d1).use(d2).use(d3);
	TestClass* test = factory.create();
	test->print();

	D3 d3_2(2);
	factory.replace(0,d3_2);
	test = factory.create();
	test->print();

	D3 p1(10),p2(20),p3(30);
	IocFactory<TestClass2> factory2;
	TestClass2* test2 = factory2.use(p1).use(p2).use(p3).create();
	test2->print();

	IocFactory<TestClass3> factory3;
	TestClass3* test3 = factory3.use(p1).use(p2).create();
	test3->print();

	std::cin.get();
	return 0;
}
