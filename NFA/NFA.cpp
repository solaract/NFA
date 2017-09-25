// NFA.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

//边表节点（状态转移
struct EdgeNode {
	int NFAnode;				//对应状态下标
	vector<char> inchar;		//允许的输入字符
	struct EdgeNode *next = NULL;		//下一个邻接点
};

//NFA状态
struct VertexNode {
	int start = 0;					//是否为起始状态
	int end = 0;					//是否为接收状态
	EdgeNode *first = NULL;				//链接状态转移表
};

//NFA状态表
struct NFAlist
{
	VertexNode nodelist[10];		//NFA状态表
	int nodeNum = 0;				//状态数~
	int vertexNum = 0;				//转移数
	vector<int> acc;				//接受状态集
};




/*		NFA模拟			*/

//初始化状态转移
void createVertex(VertexNode &VertexNode, int NFAnode, vector<char> inchar) {
	//重新分配空间
	EdgeNode *temp = new EdgeNode;
	temp->NFAnode = NFAnode;
	temp->inchar = inchar;
	temp->next = VertexNode.first;
	VertexNode.first = temp;
}

//初始化NFA
void initial(NFAlist &NFA) {
	vector<char> inchar;

	NFA.nodeNum = 6;
	NFA.vertexNum = 8;

	NFA.nodelist[0].start = 1;								//起始状态
	inchar.push_back(' '), inchar.push_back('+'), inchar.push_back('-');
	createVertex(NFA.nodelist[0], 1, inchar);				//0->1

	inchar.clear();
	inchar.push_back('0'), inchar.push_back('1'), 
		inchar.push_back('2'), inchar.push_back('3'),
		inchar.push_back('4'), inchar.push_back('5'),
		inchar.push_back('6'), inchar.push_back('7'), 
		inchar.push_back('8'), inchar.push_back('9');

	createVertex(NFA.nodelist[1], 1, inchar);				//1->1
	createVertex(NFA.nodelist[1], 4, inchar);				//1->4
	createVertex(NFA.nodelist[2], 3, inchar);				//2->3
	createVertex(NFA.nodelist[3], 3, inchar);				//3->3

	inchar.clear();
	inchar.push_back('.');

	createVertex(NFA.nodelist[1], 2, inchar);				//1->2
	createVertex(NFA.nodelist[4], 3, inchar);				//4->3

	inchar[0] = ' ';

	NFA.nodelist[5].end = 1;								//接受状态
	createVertex(NFA.nodelist[3], 5, inchar);				//3->5

	for (int i = 0; i < 10; i++)
	{
		if (NFA.nodelist[i].end == 1)NFA.acc.push_back(i);	//接受状态集
	}
}

//E闭包，结果包含原来的状态
void ECLOSE(NFAlist NFA,vector<int> &nodes) {
	int i = 0;													//当前状态位置
	EdgeNode *temp;
	//cout << "ECLOSE:" << (0 == NULL) << ';' << endl;
	//对当前状态集中的每个状态，动态栈
	while (i < (int)nodes.size()) {								//动态增长
		//cout << "ECLOSE:node:" << nodes[i] << ';' << endl;
		temp = NFA.nodelist[nodes[i]].first;					//进入下一状态
		i++;
		//对状态的每个转移
		while (temp != NULL)
		{
			//cout << "ECLOSE:size:" << temp->inchar.size() << ';' << endl;
			int len = (int)temp->inchar.size();
			//对转移的每个接受字符
			for (int i = 0;i < len;i++)
			{
				if (temp->inchar[i] == ' ')
				{
					//状态集自检有无重复
					int equal = 0;
					for (auto i : nodes) {
						if (temp->NFAnode == i) {			//该状态已入集（栈）
							equal = 1;
							break;
						}
					}
					if (equal)break;
					nodes.push_back(temp->NFAnode);				//转移后的状态入集（栈）
					//cout << "ECLOSE:innode:" << nodes.back() << ';' << endl;
					break;
				}
			}
			temp = temp->next;									//进入下一转移
		}
	}
	/*cout << "ECLOSE:nodes:";
	for (auto i : nodes) {
		cout << i << ';';
	}
	cout << endl;*/
	//return nodes;
}

//子集构造，结果不包含原来的状态
void subset(NFAlist NFA, vector<int> &nodes,char word) {
	EdgeNode *temp;												
	int len = (int)nodes.size();								//当前状态集长度
	int i = 0;													//当前状态索引
	vector<int> newNodes;										//新状态集

	//对当前状态集中的每个状态，静态栈
	while (i < len)												//定长，结果不包含原来的状态
	{
		temp = NFA.nodelist[nodes[i]].first;					//进入下一状态
		i++;
		//对状态的每个转移
		while (temp != NULL)
		{
			//cout << "subset:size:" << temp->inchar.size() << ';' << endl;
			int len = (int)temp->inchar.size();
			//对转移的每个接受字符
			for (int i = 0; i < len; i++)
			{
				//cout << "subset:word:" << temp->inchar[i] << ';' << endl;
				//接受转移
				if (temp->inchar[i] == word)
				{
					//新状态集自检有无重复
					int equal = 0;
					for (auto i : newNodes) {
						if (temp->NFAnode == i) {			//该状态已入集（栈）
							equal = 1;
							break;
						}
					}
					if (equal)break;
					newNodes.push_back(temp->NFAnode);			//转移后的状态入新集
					//cout << "subset:innode:" << newNodes.back() << ';' << endl;
					break;
				}
			}
			temp = temp->next;									//进入下一转移
		}
	}
	ECLOSE(NFA, newNodes);										//对子集求E闭包
	nodes.swap(newNodes);										//将结果传给mian变量
}

//判断集合中是否包含接受状态
int accept(vector<int> acc, vector<int> nodes) {
	for (auto i : nodes) {
		for (auto j : acc) {
			//cout << "i;j:" << i << ';' << j << endl;
			if (i == j)return 1;
		}
	}
	return 0;
}



/*		DFA模拟			*/

//对NFA状态子集排序（冒泡）
int sort(vector<int> &nodes) {
	int len = (int)nodes.size();
	int temp;
	int num;
	if (len == 0) {
		cout << "sort:nodes is empty" << endl;
		return 0;
	}
	for (int i = 0; i < len; i++)
	{
		num = 0;
		for (int j = 0; j < len - 1 - i; j++)
		{
			if (nodes[j] > nodes[j + 1]) {
				temp = nodes[j];
				nodes[j] = nodes[j + 1];
				nodes[j + 1] = temp;
				num++;
			}
		}
		if (num == 0)break;
	}
	return 1;
}

//DFA初始化
void initialDFA(NFAlist NFA,NFAlist &DFA, vector<int> nodes) {
	vector<vector<int>> map;									//NFA->DFA映射集
	vector<char> words = { '+','-','.','0','1','2','3','4','5','6','7','8','9' };//可能的输入字符
	vector<char> inchar;
	int len = (int)words.size();

	DFA.nodelist[0].start = 1;									//起始状态
	if (!sort(nodes))
	{
		cout << "start set is empty!";
		return;
	}
	map.push_back(nodes);
	//对动态栈中的每个NFA状态集合
	for (int n = 0; n < (int)map.size(); n++)
	{

		cout << "initialDFA:{" << endl;
		cout << "DFA:" << n << endl;
		cout << "NFAlist:";
		for (auto m : map[n]) {
			cout << m << ';';
		}
		cout << endl;

		//对每个可能的输入字符
		for (int i = 0; i < len; i++) {
			int empty;
			nodes = map[n];
			subset(NFA, nodes, words[i]);
			empty = !sort(nodes);
			if (empty)continue;
			int same = 0;
			int index;
			int len = (int)map.size();
			//该集合是否已存在
			for (int j = 0; j < len; j++)
			{
				if (map[j] == nodes) {
					same++;
					index = j;
					break;
				}
			}
			if (same)
			{
				inchar.push_back(words[i]); 
				createVertex(DFA.nodelist[n], index, inchar);
				cout << "trans:" << "source:" << n << ";target:" << index << ";char:" << words[i];
				cout << "nodes:{";
				for (auto m : nodes) {
					cout << m << ';';
				}
				cout << '}' << endl;

			}else{
				map.push_back(nodes);
				inchar.push_back(words[i]);
				createVertex(DFA.nodelist[n], map.size()-1, inchar);
				cout << "--trans:" << "source:" << n << ";target:" << map.size() - 1 << ";char:" << words[i];
				cout << "nodes:{";
				for (auto m : nodes) {
					cout << m << ';';
				}
				cout << '}' << endl;
			}
			inchar.clear();
		}
		if (accept(NFA.acc, map[n])) {
			DFA.nodelist[n].end = 1;
			DFA.acc.push_back(n);
			cout << "--acc" << endl;
		}
		cout << '}' << endl;
	}
	
}

int main()
{
	NFAlist NFAdig;												//NFA图
	vector<int> nodes;											//当前状态集
	char inWord[100];											//输入字符
	int mode = 0;												//解析模式
	NFAlist *analyse;

	NFAlist DFAdig;												//DFA图

	//初始化NFA图
	nodes.push_back(0);
	initial(NFAdig);
	ECLOSE(NFAdig,nodes);

	cout << "input a number 0:NFA or 1:DFA to choose the analyse mode:" << endl;
	cin >> mode;

	cout <<  "input some words:" << endl;
	cin >> inWord;

	if (mode == 1) {
		//初始化DFA图
		initialDFA(NFAdig, DFAdig, nodes);
		nodes.clear();
		nodes.push_back(0);
		analyse = &DFAdig;
	}
	else
	{
		analyse = &NFAdig;
	}

	for (int i = 0; inWord[i] != '\0'&&i < 100; i++)
	{
		subset(*analyse, nodes, inWord[i]);
		cout << "subNodes:";
		for (auto i : nodes) {
			cout << i << ';';
		}
		cout << endl;
	}

	cout << "finalNodes:";
	for (auto i : nodes) {
		cout << i << ';';
	}
	cout << endl;

	if (accept(analyse->acc, nodes)) {
		cout << "accept" << endl;
	}
	else
	{
		cout << "reject" << endl;
	}
	return 0;
}

