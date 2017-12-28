/*2017.12.25：实现读取文件、初始化工作、表格界面
  2017.12.26：实现调度图、FCFS、PSA、PSA抢占式、HRRN、SJF、SRT
  2017.12.27：实现时间片、多级反馈
  2017.12.28：微调了用户界面、动态显示*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include"head.h"
//设置颜色
void setColor(unsigned short ForeColor=7,unsigned short BackGroundColor=0)
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle,ForeColor+BackGroundColor*0x10);//设置颜色
}
//设置XY的坐标
void setxy(int x, int y)
{
   COORD coord = {x, y};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//返回光标x
int getX()
{
	HANDLE gh_std_out;
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(gh_std_out,&bInfo);
	return bInfo.dwCursorPosition.X;
}
//返回光标y
int getY()
{
	HANDLE gh_std_out;
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(gh_std_out,&bInfo);
	return bInfo.dwCursorPosition.Y;
}
//pcb复制，node复制到x
void copy_pcb(pcb * node,pcb * x)
{
	x->arrivetime=node->arrivetime;
	x->end=node->end;
	x->name=node->name;
	x->priority=node->priority;
	x->pturn=node->pturn;
	x->runtime=node->runtime;
	x->start=node->start;
	x->state=node->state;
	x->turnaroundtime=node->turnaroundtime;
	x->restime=node->restime;
	x->remain=node->remain;
	x->slice=node->slice;
}
//入队
void push(lqueuetp * lq,pcb * node)
{
	pcb * x=(pcb *)malloc(sizeof(pcb));
	copy_pcb(node,x);
	x->next=NULL;
	lq->rear->next=x;
	lq->rear=x;
}
//初始化pcb函数
void init_pcb(pcb * p)
{
	p->state='f';//未执行完状态
	p->name=(char *)malloc(sizeof(char)*50);//为字符串申请空间
	memset(p->name,'\0',sizeof(char)*50);
}
//手动输入函数
void manually_entered(int pro_num,pcb * head)
{
	pcb * p=NULL,* tail;
	int i;
	tail=head;
	for(i=0;i<pro_num;i++)
	{
		p=(pcb *)malloc(sizeof(pcb));
		init_pcb(p);
		printf("%d:输入进程名:",i);
		scanf("%s",p->name);
		//到达时间
		printf("\t到达时间:");
		scanf("%d",&p->arrivetime);
		//运行时间
		printf("\t运行时间:");
		scanf("%d",&p->runtime);
		//优先级
		printf("\t优先级:");
		scanf("%d",&p->priority);
		p->remain=p->runtime;
		p->start=-1;
		tail->next=p;
		p->next=NULL;
		tail=p;
	}
}
//读取文件
void read_file(int pro_num,pcb * head)
{
	pcb * p=NULL,* tail;
	tail=head;
	int i;
	char ch;
	FILE * fp;
	if((fp=fopen("process.txt","r"))==NULL)
	{
		printf("OPEN FAILED!\n");
		return ;
	}
	for(i=0;i<pro_num;i++)
	{
		p=(pcb *)malloc(sizeof(pcb));
		init_pcb(p);
		//到达时间 运行时间 优先级
		fscanf(fp,"%s %d %d %d",p->name,&p->arrivetime,&p->runtime,&p->priority);
		//读入换行符
		fscanf(fp,"%c",&ch);
		p->remain=p->runtime;
		p->start=-1;
		tail->next=p;
		p->next=NULL;
		tail=p;
	}
}
//释放申请的空间
void free_pcb(pcb * head)
{
	pcb * tmp=NULL,* tmpnext=NULL;
	tmp=head->next;
	while(tmp)
	{
		tmpnext=tmp->next;
		free(tmp);
		tmp=tmpnext;
	}
	free(head);
}
//处理机调度算法菜单函数
int menu(pcb * head)
{
	int n;
	printf("\t\t\t\t\t\t\t****************处理机调度算法********************\n");
	printf("\t\t\t\t\t\t\t\t\t1->先来先服务\n");
	printf("\t\t\t\t\t\t\t\t\t2->短作业优先\n");
	printf("\t\t\t\t\t\t\t\t\t3->最短剩余时间优先\n");
	printf("\t\t\t\t\t\t\t\t\t4->时间片轮转\n");
	printf("\t\t\t\t\t\t\t\t\t5->基于静态优先级的调度(非抢占式)\n");
	printf("\t\t\t\t\t\t\t\t\t6->基于静态优先级的调度(抢占式)\n");
	printf("\t\t\t\t\t\t\t\t\t7->基于高响应比优先的动态优先级调度\n");
	printf("\t\t\t\t\t\t\t\t\t8->多级反馈队列调度等处理机调度算法的实现\n");
	printf("\t\t\t\t\t\t\t\t\t0->退出\n");
	printf("\t\t\t\t\t\t\t*************************************************\n");
	simple_prin(head);
	fflush(stdin);
	printf("选择算法:");
	scanf("%d",&n);
	return n;
}
//简单的打印观察排序结果
void simple_prin(pcb * head)
{
	pcb * tmp=NULL;
	tmp=head->next;
	printf("\t\t\t\t\t\t\t\t----------进程控制块-------------\n");
	printf("\t\t\t\t\t\t\t\t进程名\t|到达时间|运行时间|优先级\n");
	while(tmp!=NULL)
	{
		printf("\t\t\t\t\t\t\t\t---------------------------------\n");
		printf("\t\t\t\t\t\t\t\t%4s\t|%4d\t |%4d\t  |%4d\n",tmp->name,tmp->arrivetime,tmp->runtime,tmp->priority);
		tmp=tmp->next;
	}
	printf("\t\t\t\t\t\t\t\t---------------------------------\n");
}
//交换pcb的值
void change_pcb(pcb * p1,pcb * p2)
{
	double tmp_dou;
	int tmp_int;
	char * tmp_str,tmp_char;
	tmp_str=(char *)malloc(sizeof(char)*50);
	//交换到达时间
	tmp_int=p1->arrivetime;
	p1->arrivetime=p2->arrivetime;
	p2->arrivetime=tmp_int;
	//交换结束时间
	tmp_int=p1->end;
	p1->end=p2->end;
	p2->end=tmp_int;
	//交换优先级
	tmp_int=p1->priority;
	p1->priority=p2->priority;
	p2->priority=tmp_int;
	//交换运行时间
	tmp_int=p1->runtime;
	p1->runtime=p2->runtime;
	p2->runtime=tmp_int;
	//交换开始时间
	tmp_int=p1->start;
	p1->start=p2->start;
	p2->start=tmp_int;
	//交换周转时间
	tmp_int=p1->turnaroundtime;
	p1->turnaroundtime=p2->turnaroundtime;
	p2->turnaroundtime=tmp_int;
	//交换带权周转时间
	tmp_dou=p1->pturn;
	p1->pturn=p2->pturn;
	p2->pturn=tmp_dou;
	//交换状态
	tmp_char=p1->state;
	p1->state=p2->state;
	p2->state=tmp_char;
	//交换名字
	tmp_str=p1->name;
	p1->name=p2->name;
	p2->name=tmp_str;
	//交换响应时间
	tmp_int=p1->restime;
	p1->restime=p2->restime;
	p2->restime=tmp_int;
	//交换剩余时间
	tmp_int=p1->remain;
	p1->remain=p2->remain;
	p2->remain=tmp_int;
	//交换时间片
	tmp_int=p1->slice;
	p1->slice=p2->slice;
	p2->slice=tmp_int;
}
//根据到达时间从小到大排序
void sort_by_arrivetime(pcb * head)
{
	pcb * p,* q,* min;
    p = head->next;
    while(p!=NULL)
    {
        min=p;
        q=p->next;
        while(q!=NULL)
        {
            if(min->arrivetime>q->arrivetime)
                min=q;
            q=q->next;
        }
        // 交换值
        if(min!=p)
        {
            change_pcb(min,p);
        }
        p=p->next;
    }
}
//打印进程运行过程
void prin_head(pcb * head,int pro_num)
{
	double turnall=0,pturnall=0;
	pcb * tmp;
	printf("\t\t\t\t\t\t---------------------------进程控制块-----------------------------\n");
	printf("\t\t\t\t\t\t进程名\t|到达时间|开始时间|运行时间|结束时间|周转时间|带权周转时间\n");
	tmp=head->next;
	//队列不空
	while(tmp!=NULL)
	{
		printf("\t\t\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t\t\t%4s\t|%4d\t |%4d\t  |%4d\t   |%4d    |%4d    |%8.2lf\n",tmp->name,tmp->arrivetime,tmp->start,tmp->runtime,tmp->end,tmp->turnaroundtime,tmp->pturn);
		turnall+=tmp->turnaroundtime;
		pturnall+=tmp->pturn;
		tmp=tmp->next;
	}
	printf("\t\t\t\t\t\t------------------------------------------------------------------\n");
	//打印平均周转时间、平均带权周转时间
	printf("\t\t\t\t\t\t\t\t平均周转时间为%.2lfs\n",turnall/pro_num);
	printf("\t\t\t\t\t\t\t\t平均带权周转时间为%.2lfs\n",pturnall/pro_num);
	printf("\t\t\t\t\t\t------------------------------------------------------------------\n");
}
//初始化矩阵，记录进程在各秒是否运行的情况
void init_matrix(int matrix[MAX][MAX],int pro_num)
{
	int i,j;
	for(i=0;i<pro_num;i++)
	{
		for(j=0;j<MAX;j++)
		{
			matrix[i][j]=0;
		}
	}
}
void prin_move(int matrix[][MAX],int pro_num,int endtime,pcb * head)
{
	int i,j,p[MAX]={0},x,y,y0,flag;
	pcb * tmp;
	tmp=head->next;
	for(i=0;i<endtime;i++)
	{
		printf("--------");
	}
	printf("\n  ");
	for(i=0;i<endtime;i++)
		printf("| %3d\t",i+1);
	printf("\n");
	for(i=0;i<endtime;i++)
	{
		printf("--------");
	}
	printf("\n");
	while(tmp)
	{
		printf("%s: ",tmp->name);
		tmp=tmp->next;
		printf("\n\n");
	}
	x=getX();
	y=getY();
	y0=y;
	x=x+2;
	setxy(x,y);
	for(i=0;i<endtime;i++)
	{
		flag=0;
		for(j=0;j<pro_num;j++)
		{
			if(matrix[j][i]==1)
			{
				x=getX();
				y=y0-2*(pro_num-j);
				setxy(x,y);
				setColor(7,j+1);
				printf("\t");
				Sleep(100);
				setColor(7,0);
				flag=1;
				break;
			}
		}
		if(!flag)
			printf("\t");
	}
	printf("\n\n\n");
	for(i=0;i<endtime;i++)
	{
		printf("--------");
	}
	printf("\n");
}
//判断链表状态是否都是t
int all_t(pcb * head)
{
	pcb * tmp;
	tmp=head->next;
	while(tmp!=NULL)
	{
		if(tmp->state!='t')
			return 0;
		tmp=tmp->next;
	}
	return 1;
}
//找到链表中等待态的最小作业时间进程
pcb * min_runtime_pcb(pcb * head)
{
	int min=9999;
	pcb * tmp=NULL,* min_tmp=NULL;
	tmp=head->next;
	while(tmp!=NULL)
	{
		if(tmp->runtime<min&&tmp->state=='w')
		{
			min=tmp->runtime;
			min_tmp=tmp;
		}
		tmp=tmp->next;
	}
	return min_tmp;
}
//找到链表中等待态的最小作业时间进程
pcb * min_priority_pcb(pcb * head)
{
	int min=9999;
	pcb * tmp=NULL,* min_tmp=NULL;
	tmp=head->next;
	while(tmp!=NULL)
	{
		if(tmp->priority<min&&tmp->state=='w')
		{
			min=tmp->priority;
			min_tmp=tmp;
		}
		tmp=tmp->next;
	}
	return min_tmp;
}
//找到链表中等待态的最短剩余时间的进程
pcb * min_remain_pcb(pcb * head)
{
	int min=9999;
	pcb * tmp=NULL,* min_tmp=NULL;
	tmp=head->next;
	while(tmp!=NULL)
	{
		if(tmp->remain<min&&tmp->state=='w')
		{
			min=tmp->remain;
			min_tmp=tmp;
		}
		tmp=tmp->next;
	}
	return min_tmp;
}
//找到链表中等待态的最大响应比的进程
pcb * max_restime_pcb(pcb * head,int time)
{
	int max=0;
	pcb * tmp=NULL,* max_tmp=NULL;
	tmp=head->next;
	while(tmp!=NULL)
	{
		tmp->restime=(time-tmp->arrivetime+tmp->runtime)/tmp->runtime;
		if(tmp->restime>max&&tmp->state=='w')
		{
			max=tmp->restime;
			max_tmp=tmp;
		}
		tmp=tmp->next;
	}
	return max_tmp;
}
//最小时间片
int min_slice(pcb * head)
{
	int min=9999;
	pcb * tmp=NULL;
	tmp=head->next;
	while(tmp!=NULL)
	{
		if(tmp->slice<min&&tmp->state=='w')
		{
			min=tmp->slice;
		}
		tmp=tmp->next;
	}
	return min;
}
//找到链表中等待态的最小时间片中最小到达时间的进程
pcb * min_slice_arrivetime(pcb * head)
{
	int min=9999,minslice;
	pcb * tmp=NULL,* min_tmp=NULL;
	tmp=head->next;
	minslice=min_slice(head);
	while(tmp!=NULL)
	{
		if(tmp->arrivetime<min&&tmp->state=='w'&&tmp->slice==minslice)
		{
			min=tmp->arrivetime;
			min_tmp=tmp;
		}
		tmp=tmp->next;
	}
	return min_tmp;
}
//返回到达时间较小的进程
pcb * min_pcb(pcb * x1,pcb * x2)
{
	if(x1==NULL)
		return x2;
	else if(x2==NULL)
		return x1;
	else if(x1->arrivetime<x2->arrivetime)
		return x1;
	else
		return x2;
}
//名字转数字
int name_to_int(pcb * head,pcb * x)
{
	int i=0;
	pcb * tmp;
	tmp=head->next;
	while(tmp)
	{
		if(strcmp(tmp->name,x->name)==0)
			return i;
		tmp=tmp->next;
		i++;
	}
	return 0;
}
//设置head都为f
void set_all_f(pcb * head)
{
	pcb * tmp;
	tmp=head->next;
	while(tmp)
	{
		tmp->state='f';
		tmp->start=-1;
		tmp->remain=tmp->runtime;
		tmp=tmp->next;
	}
}
//先来先服务
void FCFS(pcb * head,int pro_num)
{
	int time=0,i,j=0;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp;
	sort_by_arrivetime(head);
	tmp=head->next;
	while(tmp!=NULL)
	{
		if(tmp->arrivetime>time)
		{
			time++;
			continue;
		}
		else
		{
			tmp->start=time;
			tmp->end=tmp->start+tmp->runtime;
			tmp->turnaroundtime=tmp->end-tmp->arrivetime;
			tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			tmp->state='t';
			for(i=time;i<time+tmp->runtime;i++)
				proMatrix[j][i]=1;
			j++;
			time+=tmp->runtime;
			tmp=tmp->next;
		}
	}
	printf("先来先服务\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//短作业优先
void SJF(pcb * head,int pro_num)
{
	int time=0,i;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp=NULL,* tmp2=NULL,* tmp3=NULL;
	tmp=head->next;
	tmp2=tmp;
	//按到达时间升序排序
	sort_by_arrivetime(head);
	//所有的进程没有都执行完
	while(!all_t(head))
	{
		//找到最小运行时间的进程
		tmp3=min_runtime_pcb(head);
		//如果当前时间未达到进程下一个进程的开始时间，则时间加一
		if(min_pcb(tmp,tmp3)->arrivetime>time)
		{
			time++;
			continue;
		}
		else if(tmp3!=NULL)
		{
			//运行等待态的进程
			while(tmp3!=NULL)
			{
				tmp3->start=time;
				tmp3->end=tmp3->start+tmp3->runtime;
				tmp3->state='t';
				tmp3->turnaroundtime=tmp3->end-tmp3->arrivetime;
				tmp3->pturn=(double)tmp3->turnaroundtime/tmp3->runtime;
				for(i=time;i<time+tmp3->runtime;i++)
					proMatrix[name_to_int(head,tmp3)][i]=1;
				time+=tmp3->runtime;
				tmp2=tmp;
				//运行过程中到达的进程入队
				while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
				{
					tmp2->state='w';//记为等待态
					tmp2=tmp2->next;
				}
				//找到最小运行时间的进程
				tmp3=min_runtime_pcb(head);
			}
			tmp=tmp2;
		}
		else
		{
			tmp->start=time;
			tmp->end=tmp->start+tmp->runtime;
			tmp->state='t';
			tmp->turnaroundtime=tmp->end-tmp->arrivetime;
			tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			for(i=time;i<time+tmp->runtime;i++)
					proMatrix[name_to_int(head,tmp)][i]=1;
			time+=tmp->runtime;
			tmp2=tmp->next;
			//运行过程中到达的进程入队
			while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
			{
				tmp2->state='w';//记为等待态
				tmp2=tmp2->next;
			}
			tmp=tmp2;
		}
	}
	printf("短作业优先\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//最短剩余时间优先
void SRT(pcb * head,int pro_num)
{
	int time=0;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp=NULL,* tmp2=NULL,* tmp3=NULL;
	tmp=head->next;
	tmp2=tmp;
	//按到达时间升序排序
	sort_by_arrivetime(head);
	//所有的进程没有都执行完
	while(!all_t(head))
	{
		//找到最短剩余时间的进程
		tmp3=min_remain_pcb(head);
		//如果当前时间未达到进程下一个进程的开始时间，则时间加一
		if(min_pcb(tmp,tmp3)->arrivetime>time)
		{
			time++;
			continue;
		}
		else if(tmp3!=NULL)
		{
			//运行等待态的进程
			while(tmp3!=NULL)
			{
				if(tmp3->state=='f' || tmp3->start==-1)
				{
					tmp3->start=time;
					tmp3->state='w';
				}
				tmp3->remain-=1;
				if(tmp3->remain==0)
				{
					tmp3->end=time+1;
					tmp3->state='t';
					tmp3->turnaroundtime=tmp3->end-tmp3->arrivetime;
					tmp3->pturn=(double)tmp3->turnaroundtime/tmp3->runtime;
				}
				proMatrix[name_to_int(head,tmp3)][time]=1;
				time++;
				tmp2=tmp;
				//运行过程中到达的进程入队
				while(tmp2!=NULL)
				{
					if(tmp2->arrivetime <=time && tmp2->state=='f')
					{
						tmp2->state='w';//记为等待态
					}
					tmp2=tmp2->next;
				}
				//找到最短剩余时间的进程
				tmp3=min_remain_pcb(head);
			}
			tmp=tmp2;
		}
		else
		{
			if(tmp->state=='f' || tmp->start==-1)
			{
				tmp->start=time;
				tmp->state='w';
			}
			tmp->remain-=1;
			if(tmp->remain==0)
			{
				tmp->end=time+1;
				tmp->state='t';
				tmp->turnaroundtime=tmp->end-tmp->arrivetime;
				tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			}
			proMatrix[name_to_int(head,tmp)][time]=1;
			time++;
			tmp2=tmp->next;
			//运行过程中到达的进程入队
			while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
			{
				tmp2->state='w';//记为等待态
				tmp2=tmp2->next;
			}
			tmp=tmp2;
		}
	}
	printf("最短剩余时间优先\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//基于静态优先级的调度(抢占式)
void PSA_grab(pcb * head,int pro_num)
{
	int time=0;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp=NULL,* tmp2=NULL,* tmp3=NULL;
	tmp=head->next;
	tmp2=tmp;
	//按到达时间升序排序
	sort_by_arrivetime(head);
	//所有的进程没有都执行完
	while(!all_t(head))
	{
		//找到最大优先级的进程
		tmp3=min_priority_pcb(head);
		//如果当前时间未达到进程下一个进程的开始时间，则时间加一
		if(min_pcb(tmp,tmp3)->arrivetime>time)
		{
			time++;
			continue;
		}
		else if(tmp3!=NULL)
		{
			//运行等待态的进程
			while(tmp3!=NULL)
			{
				if(tmp3->state=='f' || tmp3->start==-1)
				{
					tmp3->start=time;
					tmp3->state='w';
				}
				tmp3->remain-=1;
				if(tmp3->remain==0)
				{
					tmp3->end=time+1;
					tmp3->state='t';
					tmp3->turnaroundtime=tmp3->end-tmp3->arrivetime;
					tmp3->pturn=(double)tmp3->turnaroundtime/tmp3->runtime;
				}
				proMatrix[name_to_int(head,tmp3)][time]=1;
				time++;
				tmp2=tmp;
				//运行过程中到达的进程入队
				while(tmp2!=NULL)
				{
					if(tmp2->arrivetime <=time && tmp2->state=='f')
					{
						tmp2->state='w';//记为等待态
					}
					tmp2=tmp2->next;
				}
				//找到最小运行时间的进程
				tmp3=min_priority_pcb(head);
			}
			tmp=tmp2;
		}
		else
		{
			if(tmp->state=='f' || tmp->start==-1)
			{
				tmp->start=time;
				tmp->state='w';
			}
			tmp->remain-=1;
			if(tmp->remain==0)
			{
				tmp->end=time+1;
				tmp->state='t';
				tmp->turnaroundtime=tmp->end-tmp->arrivetime;
				tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			}
			proMatrix[name_to_int(head,tmp)][time]=1;
			time++;
			tmp2=tmp->next;
			//运行过程中到达的进程入队
			while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
			{
				tmp2->state='w';//记为等待态
				tmp2=tmp2->next;
			}
			tmp=tmp2;
		}
	}
	printf("基于静态优先级的调度(抢占式)\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//基于静态优先级的调度(非抢占式)
void PSA(pcb * head,int pro_num)
{
	int time=0,i;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp=NULL,* tmp2=NULL,* tmp3=NULL;
	tmp=head->next;
	tmp2=tmp;
	sort_by_arrivetime(head);
	while(!all_t(head))
	{
		//找到最大优先级的进程
		tmp3=min_priority_pcb(head);
		if(min_pcb(tmp,tmp3)->arrivetime>time)
		{
			time++;
			continue;
		}
		else if(tmp3!=NULL)
		{
			while(tmp3!=NULL)
			{
				tmp3->start=time;
				tmp3->end=tmp3->start+tmp3->runtime;
				tmp3->state='t';
				tmp3->turnaroundtime=tmp3->end-tmp3->arrivetime;
				tmp3->pturn=(double)tmp3->turnaroundtime/tmp3->runtime;
				tmp3->remain=0;
				for(i=time;i<time+tmp3->runtime;i++)
					proMatrix[name_to_int(head,tmp3)][i]=1;
				time+=tmp3->runtime;
				tmp2=tmp;
				while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
				{
					tmp2->state='w';//记为等待态
					tmp2=tmp2->next;
				}
				//找到最大优先级的进程
				tmp3=min_priority_pcb(head);
			}
			tmp=tmp2;
		}
		else
		{
			tmp->start=time;
			tmp->end=tmp->start+tmp->runtime;
			tmp->state='t';
			tmp->turnaroundtime=tmp->end-tmp->arrivetime;
			tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			tmp->remain=0;
			for(i=time;i<time+tmp->runtime;i++)
					proMatrix[name_to_int(head,tmp)][i]=1;
			time+=tmp->runtime;
			tmp2=tmp->next;
			while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
			{
				tmp2->state='w';//记为等待态
				tmp2=tmp2->next;
			}
			tmp=tmp2;
		}
	}
	printf("基于静态优先级的调度(非抢占式)\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//基于高响应比优先的动态优先级调度
void HRRN(pcb * head,int pro_num)
{
	int time=0,i;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp=NULL,* tmp2=NULL,* tmp3=NULL;
	tmp=head->next;
	tmp2=tmp;
	sort_by_arrivetime(head);
	while(!all_t(head))
	{
		//找到最高响应比的进程
		tmp3=max_restime_pcb(head,time);
		if(min_pcb(tmp,tmp3)->arrivetime>time)
		{
			time++;
			continue;
		}
		else if(tmp3!=NULL)
		{
			while(tmp3!=NULL)
			{
				tmp3->start=time;
				tmp3->end=tmp3->start+tmp3->runtime;
				tmp3->state='t';
				tmp3->turnaroundtime=tmp3->end-tmp3->arrivetime;
				tmp3->pturn=(double)tmp3->turnaroundtime/tmp3->runtime;
				tmp3->restime=1;
				tmp3->remain=0;
				for(i=time;i<time+tmp3->runtime;i++)
					proMatrix[name_to_int(head,tmp3)][i]=1;
				time+=tmp3->runtime;
				tmp2=tmp;
				while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
				{
					tmp2->state='w';//记为等待态
					tmp2=tmp2->next;
				}
				//找到最高响应比的进程
				tmp3=max_restime_pcb(head,time);
			}
			tmp=tmp2;
		}
		else
		{
			tmp->start=time;
			tmp->end=tmp->start+tmp->runtime;
			tmp->state='t';
			tmp->turnaroundtime=tmp->end-tmp->arrivetime;
			tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			tmp->restime=1;
			tmp->remain=0;
			for(i=time;i<time+tmp->runtime;i++)
					proMatrix[name_to_int(head,tmp)][i]=1;
			time+=tmp->runtime;
			tmp2=tmp->next;
			while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
			{
				tmp2->state='w';//记为等待态
				tmp2=tmp2->next;
			}
			tmp=tmp2;
		}
	}
	printf("基于高响应比优先的动态优先级调度\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//时间片轮转
void TIMEROUND(pcb * head,int pro_num)
{
	//定义变量
	pcb * tmp,* tail,* pre,*sort_head,* q,* r,*final_head,* copy;
	int time_slice,time,i;
	int proMatrix[MAX][MAX]={0};
	//初始化
	sort_head=(pcb *)malloc(sizeof(pcb));
	sort_head->next=head->next;
	final_head=(pcb *)malloc(sizeof(pcb));
	final_head->next=NULL;
	copy=(pcb *)malloc(sizeof(pcb));
	copy->next=NULL;
	tail=sort_head;
	//输入时间片长度
	printf("时间片长度为：");
	scanf("%d",&time_slice);
	sort_by_arrivetime(head);
	time=head->next->arrivetime;
	tmp=head->next;
	//备份head
	while(tmp)
	{
		q=(pcb *)malloc(sizeof(pcb));
		copy_pcb(tmp,q);
		q->next=copy->next;
		copy->next=q;
		tmp=tmp->next;
	}
	sort_by_arrivetime(copy);
	tmp=head->next;
	//sort_head、head均不为空
	while(sort_head->next!=NULL || head->next!=NULL)
	{
		tmp=head->next;
		pre=head;
		while(tmp!=NULL)
		{
			if(tmp->arrivetime>time)
			{
				tmp=tmp->next;
				pre=pre->next;
			}
			else
			{
				q=tmp;
				tmp=tmp->next;
				pre->next=tmp;
				tail->next=q;
				q->next=NULL;
				tail=q;
			}
		}
		//提取sort_head的第一个元素，出队
		r=sort_head->next;
		sort_head->next=r->next;
		r->next=NULL;
		if(sort_head->next==NULL)
			tail=sort_head;
		//运行该元素
		if(r->start==-1)
		{
			r->start=time;
			r->state='w';
		}
		if(r->remain>time_slice)
		{
			r->remain-=time_slice;
			for(i=time;i<time+time_slice;i++)
				proMatrix[name_to_int(copy,r)][i]=1;
			time+=time_slice;
		}
		else if(r->remain==time_slice)
		{
			r->remain=0;
			for(i=time;i<time+time_slice;i++)
				proMatrix[name_to_int(copy,r)][i]=1;
			time+=time_slice;
			r->end=time;
			r->turnaroundtime=r->end-r->arrivetime;
			r->pturn=(double)r->turnaroundtime/r->runtime;
			r->state='t';
		}
		else
		{
			for(i=time;i<time+r->remain;i++)
				proMatrix[name_to_int(copy,r)][i]=1;
			time+=r->remain;
			r->remain=0;
			r->end=time;
			r->turnaroundtime=r->end-r->arrivetime;
			r->pturn=(double)r->turnaroundtime/r->runtime;
			r->state='t';
		}
		//在运行时间内到达的元素入队
		tmp=head->next;
		pre=head;
		while(tmp!=NULL)
		{
			if(tmp->arrivetime>time)
			{
				tmp=tmp->next;
				pre=pre->next;
			}
			else
			{
				q=tmp;
				tmp=tmp->next;
				pre->next=tmp;
				tail->next=q;
				q->next=NULL;
				tail=q;
			}
		}
		//r继续入队
		if(r->remain==0)
		{
			r->next=final_head->next;
			final_head->next=r;
		}
		else
		{
			tail->next=r;
			r->next=NULL;
			tail=r;
		}
	}
	printf("时间片轮转\n");
	head=final_head;
	sort_by_arrivetime(head);
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//多级反馈队列
void FEEDBACK(pcb * head,int pro_num)
{
	int time=0,time_slice;
	int proMatrix[MAX][MAX]={0};
	pcb * tmp=NULL,* tmp2=NULL,* tmp3=NULL;
	tmp=head->next;
	tmp2=tmp;
	printf("输入第一个队列的时间片\n");
	fflush(stdin);
	scanf("%d",&time_slice);
	//按到达时间升序排序
	sort_by_arrivetime(head);
	//所有的进程没有都执行完
	while(!all_t(head))
	{
		//找到最大优先级的进程
		tmp3=min_slice_arrivetime(head);
		//如果当前时间未达到进程下一个进程的开始时间，则时间加一
		if(min_pcb(tmp,tmp3)->arrivetime>time)
		{
			time++;
			continue;
		}
		else if(tmp3!=NULL)
		{
			//运行等待态的进程
			while(tmp3!=NULL)
			{
				if(tmp3->state=='f' || tmp3->start==-1)
				{
					tmp3->start=time;
					tmp3->state='w';
					tmp3->slice=time_slice;
					tmp3->remain_slice=tmp3->slice;
				}
				tmp3->remain-=1;
				tmp3->remain_slice-=1;
				if(tmp3->remain==0)
				{
					tmp3->end=time+1;
					tmp3->state='t';
					tmp3->turnaroundtime=tmp3->end-tmp3->arrivetime;
					tmp3->pturn=(double)tmp3->turnaroundtime/tmp3->runtime;
				}
				if(tmp3->remain_slice==0)
				{
					tmp3->slice*=2;
					tmp3->remain_slice=tmp3->slice;
				}
				proMatrix[name_to_int(head,tmp3)][time]=1;
				time++;
				tmp2=tmp;
				//运行过程中到达的进程入队
				while(tmp2!=NULL)
				{
					if(tmp2->arrivetime <=time && tmp2->state=='f')
					{
						tmp2->state='w';//记为等待态
					}
					tmp2=tmp2->next;
				}
				//找到最小运行时间的进程
				tmp3=min_slice_arrivetime(head);
			}
			tmp=tmp2;
		}
		else
		{
			if(tmp->state=='f' || tmp->start==-1)
			{
				tmp->start=time;
				tmp->state='w';
				tmp->slice=time_slice;
				tmp->remain_slice=tmp->slice;
			}
			tmp->remain-=1;
			if(tmp->remain==0)
			{
				tmp->end=time+1;
				tmp->state='t';
				tmp->turnaroundtime=tmp->end-tmp->arrivetime;
				tmp->pturn=(double)tmp->turnaroundtime/tmp->runtime;
			}
			if(tmp->remain_slice==0)
			{
				tmp->slice*=2;
				tmp->remain_slice=tmp->slice;
			}
			proMatrix[name_to_int(head,tmp)][time]=1;
			time++;
			tmp2=tmp->next;
			//运行过程中到达的进程入队
			while(tmp2!=NULL && tmp2->arrivetime <=time && tmp2->state=='f')
			{
				tmp2->state='w';//记为等待态
				tmp2=tmp2->next;
			}
			tmp=tmp2;
		}
	}
	printf("多级反馈队列调度\n");
	prin_move(proMatrix,pro_num,time,head);
	prin_head(head,pro_num);
}
//最终运行函数
void run()
{
	pcb * head=NULL;
	int pro_num,i=1;
	head=(pcb *)malloc(sizeof(pcb));
	head->next=NULL;
	while(i)
	{
		printf("请输入进程的个数\n");
		fflush(stdin);
		scanf("%d",&pro_num);
		printf("\t\t\t\t\t\t\t********************数据源************************\n");
		printf("\t\t\t\t\t\t\t\t\t1->读取文件\n");
		printf("\t\t\t\t\t\t\t\t\t2->手动输入\n");
		printf("\t\t\t\t\t\t\t\t\t0->退出\n");
		printf("\t\t\t\t\t\t\t**************************************************\n");
		fflush(stdin);
		scanf("%d",&i);
		switch(i)
		{
		case 1:read_file(pro_num,head);break;//读取文件
		case 2:manually_entered(pro_num,head);break;//手动输入
		case 0:break;//退出
		}
		while(i)
		{
			i=menu(head);
			switch(i)
			{
			case 1:set_all_f(head);FCFS(head,pro_num);break;//先来先服务
			case 2:set_all_f(head);SJF(head,pro_num);break;//短作业优先
			case 3:set_all_f(head);SRT(head,pro_num);break;//最短剩余时间优先
			case 4:set_all_f(head);TIMEROUND(head,pro_num);read_file(pro_num,head);break;//时间片轮转
			case 5:set_all_f(head);PSA(head,pro_num);break;//基于静态优先级的调度(非抢占式)
			case 6:set_all_f(head);PSA_grab(head,pro_num);break;//基于静态优先级的调度(抢占式)
			case 7:set_all_f(head);HRRN(head,pro_num);break;//基于高响应比优先的动态优先级调度
			case 8:set_all_f(head);FEEDBACK(head,pro_num);break;//多级反馈队列
			case 0:break;//退出
			default:break;//退出
			}
		}
	}
	free_pcb(head);
}