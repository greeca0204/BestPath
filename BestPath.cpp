/////////////////////////////////////////////////////////////
//
// FileName : Path.h
// Creator : 罗家辉
// Date : 2013-10-20 14:42:54
// Comment :寻径问题
//
/////////////////////////////////////////////////////////////
#include<iostream>
#include<time.h>
#include <stdlib.h>

using namespace std;

/*设置村庄个数和村庄间距离的最大值（本来程序可以无限扩充的，但为了避免调试时出现的数据量太大，故使用了它们的极限值）*/
#define DISTANCE_MAX 800      //设定村庄之间距离的最大值为800。
#define LIMDIS 65535          /*设定一个极限距离（设定原因在于：当随机数生成的数大于800而又小于1000时，即两个村庄无法直达的情况,
                              当最短距离为一个800-1000的数，这个800-1000的数小于用弗洛伊德算法选路时生成的那个最短路径数的时候，
							  程序就会选用那一个800-1000的数，而非用弗洛伊德算法生成的那个最短路径数，会引起程序错误。因此，设
							  定一个极限距离，即int类型的极限，能给程序一个提示，说明这一条路是死路，是不能走的，若两个村庄有去
							  无返，或图为非连通图的时候，此值可以作为终止路径计算的标志。）*/

/*设定全局变量*/
int countryNum;       //村庄的个数。
int *final;           //距离最远的村庄的最好情况。
int **InitDis;        //村庄之间的初始距离。
int **ShortestDis;    //村庄之间的最短距离。

/*函数声明部分*/ 
void WelcomeInfo();   //欢迎信息。
void InputData();     //输入村庄个数。
int DisRand();        //用于随机生成村庄间的距离。
void ProdGraph();     //随机生成一个图。
void Floyd();         //弗络伊德算法,其时间复杂度为O(n^3)，用于计算村庄之间的最短距离。
void ShortestPath();  //计算离医院最远的村庄到医院的距离最短的函数,是对Floyd的后续处理。
void OutputGraph();   //用于输出最终结果的函数。

/*程序入口*/
void main()
{		
	system("color 1F");//设置全局界面的颜色。
	WelcomeInfo();
	InputData();
	ProdGraph();  
	Floyd();
	ShortestPath();
	OutputGraph();
}

/*欢迎信息*/
void WelcomeInfo()
{
	cout << "====================================" <<endl;
	cout << "课程名：数据结构课程设计" <<endl;
	cout << "题目：寻径问题" <<endl;
	cout << "制作人：罗家辉" <<endl;
	cout << "班级：计科11-7班" <<endl;
	cout << "学号：1106102010" <<endl;
	cout << "指导老师：张艳玲、邹宇" <<endl;
	cout << "====================================" <<endl;
	cout << "欢迎您使用本程序！" <<endl;	
}

/*输入村庄个数*/
void InputData()
{
	int i;
	cout << "请输入村庄个数:";	

	//非法字符判断
	while(1)
	{
		cin >>countryNum;

		if(!cin)	//默认参数为0,即无错误。
		{ 
			cout<<"输入有错!请重新输入!"<<endl;
			cout<<"请输入村庄个数:";
			cin.clear();  //让错误标识改回为0。
			cin.sync();   //清空流。
		}
		else
		{
			break;
		}
	}

	InitDis = new int*[countryNum];  //初始化村庄间的初始距离。注意，int*[countryNum]表示一个有countryNum个元素的指针数组。
	for (i = 0; i < countryNum; i++)
	{
		InitDis[i] = new int[countryNum];
	}

	ShortestDis = new int*[countryNum];//初始化村庄间的最短距离。注意，int*[countryNum]表示一个有countryNum个元素的指针数组
	for (i = 0; i < countryNum; i++)
	{
		ShortestDis[i] = new int[countryNum];
	}
	
	final = (int *)malloc(countryNum*sizeof(int));  //初始化距离最远的村庄的最好情况。	
}

/*村庄间距离图的生成*/
void ProdGraph()
{
	srand((int)time(0));  //定义随机种子，供自动生成村庄间的距离使用。
	int i,j;
	//根据随机数的生成情况，分别对没有村庄、只有一个村庄、多个村庄的情况进行输出。
	if(0==countryNum) //没有村庄。
	{
		cout<< "没有村庄!\n";
		exit(1);
	}
	else if(1==countryNum)  //只有一个村庄。
	{
		cout << "只有一个村庄,无需选路!";
		cout << "\n建设医院的最佳位置是村庄1\n";
		exit(1);
	}
	else  //多个村庄。
	{
		cout<< "随机生成一个节点为" << countryNum << "的图。\n\n";
    }
	//两次循环，将村庄间的距离放入矩阵中。
	for(i=0; i<countryNum; i++)
	{
        for(j=0; j<countryNum; j++)
		{
			//如果i=j,则代表源和目的都是自己，故村庄间的初始距离和最短距离都为0；否则，需要随机生成村庄间的距离。
            if(i==j)
			{ 
				ShortestDis[i][j] = 0;
				InitDis[i][j] = 0;
			}   
            else
			{
				InitDis[i][j]=DisRand();//随机生成村庄间的距离。
				//输出村庄间的距离，如果随机生成的数字比村庄之间距离的最大值要大的话，则输出∞,否则,按生成的数字输出。
				if(InitDis[i][j]>=DISTANCE_MAX)
				{
					cout << "村庄" << i+1 <<"到村庄" << j+1 << "的距离是：∞\n";
					InitDis[i][j]=LIMDIS;//将其指向一个无限大的值，使其永远不能到达。
				}
				else			
				{
					cout << "村庄" << i+1 <<"到村庄" << j+1 << "的距离是：" << InitDis[i][j] <<"\n";
				}
                ShortestDis[i][j] = InitDis[i][j];//同时将输入的值存到村庄之间的最短距离的矩阵中，用于下面Floyd函数的处理。
			}   
        }
    }
    
	//输出村庄间的距离的初始化矩阵
	cout << "\n村庄之间路径的距离（矩阵）:\n";

	//输出矩阵列标识
	cout <<"\t";
	for(j=0;j<countryNum;j++)
	{
		cout << "村庄" << j+1 <<"\t";
	}
	cout << "\n";
	//两次循环输出整个矩阵
    for(i=0;i<countryNum;i++)
	{
		cout << "村庄" << i+1 <<"\t";//输出矩阵行标识
		for(j=0;j<countryNum;j++)
		{			
			//输出村庄间的距离，如果随机生成的数字比村庄之间距离的最大值要大的话，则输出∞,否则,按生成的数字输出。
			if(InitDis[i][j]>=DISTANCE_MAX)
			{
				cout << "∞\t";
			}
			else
			{
				cout << InitDis[i][j] <<"\t";
			}
		}
		cout << "\n";  //每输出一行，需要用换行来输出整个矩阵。
	}
	delete[] InitDis; //销毁InitDis指针数组。
}

/*弗络伊德算法，求多源点的最短路径*/
void Floyd()
{   
	int i,j,k;
	//弗洛伊德算法，参照课本书写三次循环。
	for(k=0;k<countryNum;k++)
	{
		for(i=0;i<countryNum;i++)
		{
			for(j=0;j<countryNum;j++)
			{   
				//如果通过中间路径，比原来存于数组里的值小，则代换。
				if(ShortestDis[i][j]>ShortestDis[i][k]+ShortestDis[k][j])
				{   
					ShortestDis[i][j]=ShortestDis[i][k]+ShortestDis[k][j];
				}
			}
		}
	}
	cout << "\n村庄之间路径的最短距离（矩阵）：\n";

	//输出矩阵列标识
	cout <<"\t";
	for(j=0;j<countryNum;j++)
	{
		cout << "村庄" << j+1 <<"\t";
	}
	cout << "\n";
	//两次循环输出村庄之间的最短路径,同样以矩阵的形式显示。
	for(i=0;i<countryNum;i++)
	{
		cout << "村庄" << i+1 <<"\t";//输出矩阵行标识。
		for(j=0;j<countryNum;j++)
		{	
			//若此图为非连通图或有些村庄会能去不能返,将其标志为∞，即不可达；否则，正常选路。
			if(LIMDIS==ShortestDis[i][j])
			{
				cout<< "∞\t";
			}
			else
			{
				cout << ShortestDis[i][j] <<"\t";
			}
		}
		cout << "\n";  //每输出一行，需要用换行来输出整个矩阵。
	}
}

/*计算离医院最远的村庄到医院的最短距离*/
void ShortestPath()
{   
	int i,j;

	//遍历所有最短路径的最大值，找出各村庄最短路径的最长距离。
	for(i=0;i<countryNum;i++)
	{   
		final[i] = 0;
        //遍历同一行的不同列，找出最短路径的最大值。
		for(j=0;j<countryNum;j++) 
		{
			if(final[i]<ShortestDis[j][i])
			{ 
				final[i]=ShortestDis[j][i];
			}
			//cout<<ShortestDis[j][i]<<ends;
		}
    }
	delete[] ShortestDis; //销毁ShortestDis指针数组。

    cout << "\n村庄\t最长距离\n";
    for(i=0;i<countryNum;i++)
	{
        //如果最长距离为极限距离65535的时候,就将其值置为∞输出；否则，输出各村庄到其他村庄最短路径的最长距离。
		if(LIMDIS==final[i])
		{
			cout << i+1 << "\t" << "∞\n";  //置为∞输出。
		}
		else
		{
			cout << i+1 << "\t" << final[i] << "\n";   //输出各村庄到其他村庄最短路径的最长距离。
		}
	}
}

/*输出医院建立的最佳位置*/
void OutputGraph()
{
	int i,tmp=0;
	//将各村庄到其他村庄的最短路径的最长距离进行遍历，选出最长距离的最小值，确定医院的位置。
	for(i=1;i<countryNum;i++)
	{
		if(final[tmp]>final[i])
		{
			tmp=i;
		}
		//此图为非连通图或有些村庄能去不能返,就不能确定该医院建在哪里；否则，正常选路。
		//if(LIMDIS==final[tmp]){
			//cout <<"此图为非连通图或有些村庄能去不能返,故不能确定该医院建在哪里!\n";
			//exit(1);
		//}
	}

	free(final); //销毁final动态数组。
	cout << "\n建设医院的最佳位置是村庄" << tmp+1 << "\n";   //输出医院建设的最佳位置。
}

/*随机生成村庄间的距离*/
int DisRand()
{
	int pathDis =0;
	pathDis = (rand()%1000); //生成村庄间的距离
	return pathDis;
}