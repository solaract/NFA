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
	struct EdgeNode *next;		//下一个邻接点
};

//NFA状态
struct VertexNode {
	int start = 0;					//是否为起始状态
	int end = 0;					//是否为接收状态
	EdgeNode *first;				//链接状态转移表
};

//NFA状态表
struct NFAlist
{
	VertexNode nodelist[10];		//NFA状态表
	int nodeNum = 0;				//状态数~
	int vertexNum = 0;				//转移数
};

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
	for (int i = 0; i < 10; i++)
	{
		NFA.nodelist[i].first = NULL;						//边表置空
	}

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
}

//E闭包，结果包含原来的状态
void ECLOSE(NFAlist &NFA,vector<int> &nodes) {
	//vector<int> nodes;										//E闭包状态序列
	int i = 0;													//当前状态位置
	EdgeNode *temp;
	//cout << "ECLOSE:" << (0 == NULL) << ';' << endl;
	while (i < (int)nodes.size()) {								//动态增长
		cout << "ECLOSE:node:" << nodes[i] << ';' << endl;
		temp = NFA.nodelist[nodes[i]].first;
		i++;
		while (temp != NULL)
		{
			cout << "ECLOSE:size:" << temp->inchar.size() << ';' << endl;
			int len = (int)temp->inchar.size();
			for (int i = 0;i < len;i++)
			{
				if (temp->inchar[i] == ' ')
				{
					nodes.push_back(temp->NFAnode);
					cout << "ECLOSE:innode:" << nodes.back() << ';' << endl;
					break;
				}
			}
			temp = temp->next;
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
void subset(NFAlist &NFA, vector<int> &nodes,char word) {
	EdgeNode *temp;
	int len = (int)nodes.size();
	int i = 0;
	vector<int> newNodes;

	while (i < len)												//定长，结果不包含原来的状态
	{
		temp = NFA.nodelist[nodes[i]].first;
		i++;
		while (temp != NULL)
		{
			cout << "subset:size:" << temp->inchar.size() << ';' << endl;
			int len = (int)temp->inchar.size();
			for (int i = 0; i < len; i++)
			{
				cout << "subset:word:" << temp->inchar[i] << ';' << endl;
				if (temp->inchar[i] == word)
				{
					newNodes.push_back(temp->NFAnode);			//转移后的状态
					cout << "subset:innode:" << newNodes.back() << ';' << endl;
					break;
				}
			}
			temp = temp->next;
		}
	}
	ECLOSE(NFA, newNodes);										//对子集求E闭包
	nodes.swap(newNodes);										//将结果传给mian变量
}

int main()
{
	NFAlist NFAdig;
	vector<int> nodes;
	nodes.push_back(0);
	initial(NFAdig);
	ECLOSE(NFAdig,nodes);
	cout << "nodes:";
	for (auto i : nodes) {
		cout << i << ';';
	}
	cout << endl;
	cout << (NFAdig.nodelist[3].first->next->NFAnode) << ';' << endl;
	subset(NFAdig, nodes, '1');
	cout << "nodes:";
	for (auto i : nodes) {
		cout << i << ';';
	}
	cout << endl;
	//for (int i = 0; i < 20; i++)
	//{
	//	std::cout << nodes[i] << ',';
	//}
	//std::cout << std::endl;
	return 0;
}

