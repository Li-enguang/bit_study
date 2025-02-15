#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
using namespace std;

int n, m; //系统中进程总数n和资源种类总数m
int Available[100]; //资源可用总量
int Max[100][100];  //最大需求矩阵
int Allocation[100][100] = { 0 }; //当前给分配给每个进程的各种资源数量
int Need[100][100];//当前每个进程还需分配的各种资源数量
int Work[100]; //当前可分配的资源
bool Finish[100]; //进程是否结束

//显示功能
void show_function()
{
	cout << "\n\n*        功能选择        *" << endl;
	cout << "*1.显示当前资源情况      *" << endl;
	cout << "*2.当前状态安全检查      *" << endl;
	cout << "*3.请求资源分配          *" << endl;
	cout << "*4.退出程序              *" << endl;
	return;
}

//初始数据输入
void input()
{
	cout << "请输入进程总数：" << endl;
	cin >> n;
	cout << "请输入资源种类总数：" << endl;
	cin >> m;
	cout << "请依次输入各种资源数量：" << endl;
	for (int i = 0; i < m; i++)
	{
		cin >> Available[i];
	}
	for (int i = 0; i < n; i++)
	{
		cout << "***   输入进程P" << i << "的初始信息   ***" << endl;
		cout << "请输入该进程所需各资源的最大量：" << endl;
		for (int j = 0; j < m; j++)
		{
			cin >> Max[i][j];
		}
		cout << "请输入该进程已分配各资源的数量：" << endl;
		for (int j = 0; j < m; j++)
		{
			cin >> Allocation[i][j];
		}
	}
	return;
}

//显示当前资源情况
void show()
{
	//输出分配矩阵Allocation
	cout << "\n***   当前资源分配情况   ***" << endl;
	cout << "进程";
	for (int i = 0; i < m; i++)
	{
		cout << "\t资源" << i;
	}
	cout << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i;
		for (int j = 0; j < m; j++)
		{
			cout << "\t" << Allocation[i][j];
		}
		cout << endl;
	}
	//输出需求矩阵Need
	cout << "\n***   当前资源需求情况   ***" << endl;
	cout << "进程";
	for (int i = 0; i < m; i++)
	{
		cout << "\t资源" << i;
	}
	cout << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i;
		for (int j = 0; j < m; j++)
		{
			cout << "\t" << Need[i][j];
		}
		cout << endl;
	}
	//输出可利用资源量Available
	cout << "\n***   当前资源剩余情况   ***" << endl;
	for (int i = 0; i < m; i++)
	{
		cout << "资源" << i << "\t";
	}
	cout << endl;
	for (int j = 0; j < m; j++)
	{
		cout << Available[j] << "\t";
	}
	cout << endl;
	//输出进程执行情况
	cout << "\n***   当前进程执行情况   ***" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "进程" << i << "\t";
	}
	cout << endl;
	for (int j = 0; j < n; j++)
	{
		if (Finish[j])
		{
			cout << "true" << "\t";
		}
		else
		{
			cout << "false" << "\t";
		}
	}
	cout << endl;
}

//判断是不是所有进程均已执行完
bool judge_end(bool *finish)
{
	bool flag = 1;
	for (int i = 0; i < m; i++)
	{
		if (finish[i] != true)
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

//状态安全检查
void safety(int *result)
{
	int ji = 1;
	//初始化Work
	for (int i = 0; i < m; i++)
	{
		Work[i] = Available[i];
	}
	//初始化finish，finish是在本次测试中标记是不是所有进程都执行完
	bool finish[100];
	for (int i = 0; i < n; i++)
	{
		finish[i] = Finish[i];
	}
	int flag1 = 0;
	bool flag2 = 1;
	for (int i = 0; i < n; i++)
	{
		//当循环新开始时，标记这次循环当前还没有结束任何进程
		if (i == 0)
			flag1 = 0;
		//跳过已经完成的进程
		if (finish[i] == true)
			continue;
		//Work与Need比较
		int flag3 = 1;
		for (int j = 0; j < m; j++)
		{
			if (Work[j] < Need[i][j])
			{
				flag3 = 0;
				break;
			}
		}
		//若资源不够则跳过这一进程
		if (flag3 != 1)
		{
			//是不是本次循环进程一个都没能结束
			if (i == n - 1 && flag1 != 1)
			{
				//分析一个都没能结束的原因
				//是不是全部进程已经都执行完了
				if (judge_end(finish))
					break;
				else
				{
					//存在没结束的进程但无法结束
					flag2 = 0;
					break;
				}
			}
			continue;
		}
		else
		{
			//若资源够，则执行完该进程，释放该进程占用的资源
			result[ji] = i;
			ji++;
			for (int j = 0; j < m; j++)
			{
				Work[j] = Work[j] + Allocation[i][j];
			}
			finish[i] = true;
			flag1 = 1;//标记这一轮找到了可以执行完的进程
			i = -1;//从头再开始遍历进程集合
		}
	}
	result[0] = flag2;
	return;
}

//资源分配
bool allocate(int number, int *  request)
{
	int flag = 1;
	for (int i = 0; i < m; i++)
	{
		if (request[i] > Need[number][i])
		{
			flag = 0;
			break;
		}
	}
	if (flag == 0)
	{
		cout << "请求大于该进程还需要的资源量，请求不合法" << endl;
		return 0;
	}
	flag = 1;
	for (int i = 0; i < m; i++)
	{
		if (request[i] > Available[i])
		{
			flag = 0;
			break;
		}
	}
	if (flag == 0)
	{
		cout << "请求大于当前系统剩余的资源量，请求不合法" << endl;
		return 0;
	}

	// 尝试分配
	for (int i = 0; i < m; i++)
	{
		Need[number][i] = Need[number][i] - request[i];
		Allocation[number][i] = Allocation[number][i] + request[i];
		Available[i] = Available[i] - request[i];
	}

	//安全性判断
	int *result = (int*)malloc(sizeof(int) * 101);
	memset(result, -1, sizeof(int) * 101);
	safety(result);
	if (result[0])
	{
		cout << "存在此安全序列：";
		cout << "P" << result[1];
		for (int i = 2;; i++)
		{
			if (result[i] == -1)
			{
				break;
			}
			cout << " --> P" << result[i];
		}
		cout << endl;
		return 1;
	}
	else
	{
		cout << "根据安全性检查，本次分配不安全" << endl;
		//撤销分配
		for (int i = 0; i < m; i++)
		{
			Need[number][i] = Need[number][i] + request[i];
			Allocation[number][i] = Allocation[number][i] - request[i];
			Available[i] = Available[i] + request[i];
		}
		return 0;
	}
}

void end(int number)
{
	int flag = 1;
	for (int i = 0; i < m; i++)
	{
		if (Need[number][i] != 0)
		{
			flag = 0;
			break;
		}
	}
	//若果这个进程已经不需要再分配资源了
	//执行完该进程，释放已分配资源
	if (flag == 1)
	{
		cout << "***     进程P" << number << "执行完成！    ***" << endl;
		Finish[number] = true;
		for (int i = 0; i < m; i++)
		{
			Available[i] = Available[i] + Allocation[number][i];
			Allocation[number][i] = 0;
		}
	}
}

int main()
{
	cout << "******************************" << endl;
	cout << "***   欢迎来到银行家算法   ***" << endl;
	cout << "***1.显示当前资源情况      ***" << endl;
	cout << "***2.当前状态安全检查      ***" << endl;
	cout << "***3.请求资源分配          ***" << endl;
	cout << "***4.退出程序              ***" << endl;
	cout << "******************************\n" << endl;

	memset(Finish, false, sizeof(Finish));
	//初始数据输入
	input();
	//计算Need矩阵和剩余的可以利用资源Available
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
			Available[j] = Available[j] - Allocation[i][j];
		}
	}


	int flag;
	while (true)
	{
		//功能选择
		show_function();
		cout << "请选择功能：";
		cin >> flag;
		if (flag == 1)
		{
			//显示当前资源情况
			//包括当前资源分配情况和资源剩余情况
			show();
		}
		else if (flag == 2)
		{
			//当前状态安全检查
			int *result = (int*)malloc(sizeof(int) * 101);
			memset(result, -1, sizeof(int) * 101);
			safety(result);

			if (result[0] == 1)
			{
				cout << "存在此安全序列：";
				cout << "P" << result[1];
				for (int i = 2;; i++)
				{
					if (result[i] == -1)
					{
						break;
					}
					cout << " --> P" << result[i];
				}
				cout << endl;
				cout << "***       此刻状态安全       ***" << endl;

			}
			else
			{
				cout << "***      此刻状态不安全！    ***" << endl;
			}
		}
		else if (flag == 3)
		{
			//请求资源分配
			int *request = (int*)malloc(sizeof(int) * 100);
			int number;
			cout << "请输入发出请求的进程：P";
			cin >> number;
			cout << "请输入请求各资源的数量：" << endl;
			for (int i = 0; i < m; i++)
			{
				cin >> request[i];
			}
			if (allocate(number, request))
			{
				cout << "***  该分配安全，分配成功！ ***" << endl;
				//成功分配的进程，能完成的就完成，释放占用的资源
				end(number);
			}
			else
			{
				cout << "***        分配失败！       ***" << endl;
			}

		}
		else if (flag == 4)
		{
			//退出程序
			cout << "成功退出！" << endl;
			break;

		}
	}
	system("pause");
	return 0;
}