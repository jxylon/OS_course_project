#define MAX 100
typedef struct pcb
{
	int runtime;//运行时间
	char * name;//进程名
	int arrivetime;//到达时间
	int priority;//优先级
	int turnaroundtime;//周转时间
	double pturn;//带权周转时间
	int start;//开始时间
	int end;//结束时间
	char state;//状态
	int restime;//响应时间
	int remain;//剩余时间
	int slice;//时间片
	int remain_slice;//剩余时间片
	struct pcb * next;//下一个结点
}pcb;
//队列结构体
typedef struct
{
	pcb * front;
	pcb * rear;
}lqueuetp;
void init_pcb(pcb * p);//初始化pcb函数
void run();//最终运行函数
void manually_entered(int pro_num,pcb * head);//手动输入函数
void read_file(int pro_num,pcb * head);//读取文件
void free_pcb(pcb * head);//释放申请的空间
int menu(pcb * head);//处理机调度算法菜单函数
void FCFS(pcb * head);//先来先服务
void change_pcb(pcb * p1,pcb * p2);//交换pcb的值
void sort_by_arrivetime(pcb * head);//根据到达时间从小大到排序
void prin_head(pcb * head,int pro_num);//打印进程运行过程
void simple_prin(pcb * head);//简单的打印观察排序结果
void SJF(pcb * head,int pro_num);//短作业优先
void copy_pcb(pcb * node,pcb * x);//pcb复制，node复制到x
void set_all_f(pcb * head);//设置head都为f
void HRRN(pcb * head,int pro_num);//基于高响应比优先的动态优先级调度
void PSA(pcb * head,int pro_num);//基于静态优先级的调度(非抢占式)
void PSA_grab(pcb * head,int pro_num);//基于静态优先级的调度(抢占式)
void SRT(pcb * head,int pro_num);//最短剩余时间优先
int name_to_int(pcb * head,pcb * x);//名字转数字
pcb * min_pcb(pcb * x1,pcb * x2);//返回到达时间较小的进程
pcb * max_restime_pcb(pcb * head,int time);//找到链表中等待态的最大响应比的进程
pcb * min_remain_pcb(pcb * head);//找到链表中等待态的最短剩余时间的进程
pcb * min_priority_pcb(pcb * head);//找到链表中等待态的最小作业时间进程
pcb * min_runtime_pcb(pcb * head);//找到链表中等待态的最小作业时间进程
pcb * min_slice_arrivetime(pcb * head);//找到链表中等待态的最小时间片中最小到达时间的进程
int min_slice(pcb * head);//最小时间片
int all_t(pcb * head);//判断链表状态是否都是t
void init_matrix(int matrix[MAX][MAX],int pro_num);//初始化矩阵，记录进程在各秒是否运行的情况
void prin_matrix(int matrix[][MAX],int pro_num,int endtime,pcb * head);//打印矩阵
void TIMEROUND(pcb * head,int pro_num);//时间片轮转
void FEEDBACK(pcb * head,int pro_num);//多级反馈队列

