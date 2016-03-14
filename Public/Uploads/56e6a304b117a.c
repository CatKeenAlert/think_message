#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//buzzer
#include <sys/stat.h>  
#include <sys/ioctl.h>  
//app
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

#define RESULT(x) sqrt(data_of_N_FFT[x].real*data_of_N_FFT[x].real+data_of_N_FFT[x].imag*data_of_N_FFT[x].imag)
#define PI  3.14159265358979323846264338327950288419716939937510    //圆周率，50位小数
#define PI2 6.28318530717958647692528676655900576839433879875021
/*buzzer*/
#define PWM_IOCTL_SET_FREQ   1
#define PWM_IOCTL_STOP    2
#define ESC_KEY   0x1b               //定义ESC_KEY 为ESC按键的键值
#define danxinpin  3
#define xianxinpin 4
static int buzzer_fd = -1;
static void close_buzzer(void);
static void open_buzzer(void)                   //打开蜂鸣器
{
	buzzer_fd = open("/dev/pwm", 0);                       //打开pwm设备驱动文件
	if (buzzer_fd < 0) {
		perror("open pwm_buzzer device");
		exit(1);                                                  //打开错误，则终止进程。退出参数为1
	}
	// any function exit call will stop the buzzer
	atexit(close_buzzer);                             //退出回调close_buzzer
}
static void close_buzzer(void)                       //关闭蜂鸣器
{
	if (buzzer_fd >= 0) {
		//ioctl(buzzer_fd, PWM_IOCTL_STOP);                  //停止蜂鸣器
		close(buzzer_fd);                                                     //关闭设备驱动文件
		buzzer_fd = -1;
	}
}
static void set_buzzer_freq(int freq)
{
	// this IOCTL command is the key to set frequency
	int ret = ioctl(buzzer_fd, PWM_IOCTL_SET_FREQ, freq);            //设置频率
	if(ret < 0) {                                                                       //如果输入的频率错误
		perror("set the frequency of the buzzer");
		exit(1);                                                                  //退出，返回1
	}
}
static void stop_buzzer(void)
{
	int ret = ioctl(buzzer_fd, PWM_IOCTL_STOP);          //关闭蜂鸣器
	if(ret < 0) {                                                     //如果无法关闭蜂鸣器
		perror("stop the buzzer");
		exit(1);                                                          //退出返回1
	}
}
//
int N_FFT=0;                //傅里叶变换的点数
int M_of_N_FFT=0;           //蝶形运算的级数，N = 2^M
int Npart2_of_N_FFT=0;      //创建正弦函数表时取PI的1/2
int Npart4_of_N_FFT=0;      //创建正弦函数表时取PI的1/4 同时也是窗函数步进值
int R_of_STFT=0;            //短时傅里叶变换的结果数列长度
int H_of_STFT=0;            //短时傅里叶变换的总点数
int points=0;               //总数据库清空的次数
char a=0;
int offset=615;             //电压的初期偏置

int judged[4];

static int fw=1;					//实际信号频率
static int fqb=4;					//采样周期比信号周期
static int limitdata = 60;			//门限			
static double timel=0.1 ;             //脉宽（时间）单位是秒
static int mwidth = 0.1;			//脉宽（周期数）	
//mwidth = timel*fw;






 //   int T=0;

typedef float ElemType;     //原始数据序列的数据类型,可以在这里设置

typedef struct              //定义复数结构体
{
    ElemType real,imag;
}complex_of_N_FFT,*ptr_complex_of_N_FFT;

ptr_complex_of_N_FFT data_of_N_FFT=NULL;//开辟存储单元，原始数据与复数结果均使用之
ptr_complex_of_N_FFT clone_of_N_FFT=NULL;//开辟存储单元，原始数据与复数结果均使用之
ElemType* SIN_TABLE_of_N_FFT=NULL;
int *data_of_R_STFT=NULL;               //开辟存储单元，将短时傅里叶变换的结果存储于其中
ptr_complex_of_N_FFT wGoe=NULL;
ptr_complex_of_N_FFT data_of_H_STFT=NULL;//开辟存储单元，存放短时傅里叶变换的原始数组




//产生模拟原始数据输入
/*void InputData(void)
{
    int i;
    for (i=0; i<N_FFT; i++)//制造输入序列
    {
        data_of_N_FFT[i].real = sin(0.03*PI*i*i/N_FFT);  //正弦波
        data_of_N_FFT[i].imag = 0;
      //  printf("%f ",data_of_N_FFT[i].real);
    }
    for (i=0; i<N_FFT; i++)//制造输入序列
    {
        clone_of_N_FFT[i].real = sin(0.03*PI*i*i/N_FFT);  //正弦波
        clone_of_N_FFT[i].imag = 0;
      //  printf("%f ",data_of_N_FFT[i].real);
    }

     for (i=0; i<H_of_STFT; i++)//制造输入序列
    {
        data_of_H_STFT[i].real = sin(0.03*PI*i*i/N_FFT);//(double)(rand()%10)*0.01;  //正弦波
        data_of_H_STFT[i].imag = 0;
       // printf("%f ",data_of_H_STFT[i].real);
    }
   // printf("\n\n");

}*/
//输入
void InputData(void)
{
	int j = 0 ;
	int i;
	int ret ;
	int data;
	int fd ;
	fd = open("/dev/myadc",O_RDWR);
	if(fd<0)
	{
		printf("open ADC device failed");
		exit(1);
	}

	while(1)
	{
		for(i = 0;i<4;i++)
		{
			read(fd,&judged[i],sizeof(judged[i]));
		}
		
		if(abs(judged[0]-judged[1])+abs(judged[1]-judged[2])+abs(judged[2]-judged[3])+abs(judged[3]-judged[4])>limitdata)
		{
			break;
		}
	}
	printf("准备获取数据");
	for(j = 0 ;j<H_of_STFT;j++)
	{

		data_of_H_STFT[j].real = data;
		//data_of_H_STFT[j].real = data;
		data_of_H_STFT[j].imag = 0;
		ret = read(fd,&data,sizeof(data));
		//printf("Read ADC value is :%d\n",data);
	}
	printf("数据读取完成");
	for(j = 0 ;j<N_FFT;j++)
	{
	        data_of_N_FFT[j].real = 0;  //正弦波
       		data_of_N_FFT[j].imag = 0;
         	clone_of_N_FFT[j].real = 0;  //正弦波
        	clone_of_N_FFT[j].imag = 0;
		//printf("Read ADC value is :%d\n",data);
	}
	for(j = 0 ;j<H_of_STFT;j++)
	{
		printf("Read ADC value is :%d\n",(int)data_of_H_STFT[j].real);
	}
	close(fd);
}
//创建正弦函数表
void CREATE_SIN_TABLE(void)
{
    int i=0;
    for (i=0; i<=Npart4_of_N_FFT; i++)
    {
        SIN_TABLE_of_N_FFT[i] = sin(PI*i/Npart2_of_N_FFT);//SIN_TABLE[i] = sin(PI2*i/N);
    }
}


ElemType Sin_find(ElemType x)
{
    int i = (int)(N_FFT*x);
    i = i>>1;
    if (i>Npart4_of_N_FFT)//注意：i已经转化为0~N之间的整数了！
    {
        //不会超过N/2
        i = Npart2_of_N_FFT - i;//i = i - 2*(i-Npart4);
    }
    return SIN_TABLE_of_N_FFT[i];
}

ElemType Cos_find(ElemType x)
{
    int i = (int)(N_FFT*x);
    i = i>>1;
    if (i<Npart4_of_N_FFT)//注意：i已经转化为0~N之间的整数了！
    {
        //不会超过N/2
        //i = Npart4 - i;
        return SIN_TABLE_of_N_FFT[Npart4_of_N_FFT - i];
    }
    else//i>Npart4 && i<N/2
    {
        //i = i - Npart4;
        return -SIN_TABLE_of_N_FFT[i - Npart4_of_N_FFT];
    }
}

//倒序处理
void ChangeSeat(complex_of_N_FFT *DataInput)
{
    int nextValue,nextM,i,k,j=0;
    complex_of_N_FFT temp;

    nextValue=N_FFT/2;                  //变址运算，即把自然顺序变成倒位序，采用雷德算法
    nextM=N_FFT-1;
    for (i=0;i<nextM;i++)
    {
        if (i<j)                 //如果i<j,即进行变址
        {
            temp=DataInput[j];
            DataInput[j]=DataInput[i];
            DataInput[i]=temp;
        }
        k=nextValue;                //求j的下一个倒位序
        while (k<=j)             //如果k<=j,表示j的最高位为1
        {
            j=j-k;                  //把最高位变成0
            k=k/2;                  //k/2，比较次高位，依次类推，逐个比较，直到某个位为0
        }
        j=j+k;                      //把0改为1
    }
}
void Goe(int m)
{
    int i=0;

    wGoe[0]=data_of_N_FFT[0];
    wGoe[1].real=data_of_N_FFT[1].real+2*Cos_find(PI2*m/N_FFT)*wGoe[0].real;
    for(i=0;i<N_FFT-2;i++)
    {
        wGoe[i+2].real=2*Cos_find(PI2*m/N_FFT)*wGoe[i+1].real-wGoe[i].real+data_of_N_FFT[i].real;
    }

}


void FFT(void)
{
    int L=0,B=0,J=0,K=0;
    int i=0;
    int step=0, KB=0;
    //ElemType P=0;
    ElemType angle;
    complex_of_N_FFT W,Temp_XX;



    ChangeSeat(data_of_N_FFT);//变址
    //CREATE_SIN_TABLE();
    for (L=1; L<=M_of_N_FFT; L++)
    {
        step = 1<<L;//2^L
        B = step>>1;//B=2^(L-1)
        for (J=0; J<B; J++)
        {
            //P = (1<<(M-L))*J;//P=2^(M-L) *J
            angle = (double)J/B;            //这里还可以优化
            W.imag =  -Sin_find(angle);     //用C++该函数课声明为inline
            W.real =   Cos_find(angle);     //用C++该函数课声明为inline
            //W.real =  cos(angle*PI);
            //W.imag = -sin(angle*PI);
            for (K=J; K<N_FFT; K=K+step)
            {
                KB = K + B;
                //Temp_XX = XX_complex(data[KB],W);
                //用下面两行直接计算复数乘法，省去函数调用开销
                Temp_XX.real = data_of_N_FFT[KB].real * W.real-data_of_N_FFT[KB].imag*W.imag;
                Temp_XX.imag = W.imag*data_of_N_FFT[KB].real + data_of_N_FFT[KB].imag*W.real;

                data_of_N_FFT[KB].real = data_of_N_FFT[K].real - Temp_XX.real;
                data_of_N_FFT[KB].imag = data_of_N_FFT[K].imag - Temp_XX.imag;

                data_of_N_FFT[K].real = data_of_N_FFT[K].real + Temp_XX.real;
                data_of_N_FFT[K].imag = data_of_N_FFT[K].imag + Temp_XX.imag;
            }
        }
    }
}

//初始化FFT程序
//N_FFT是 FFT的点数，必须是2的次方
void Init_FFT(int N_of_FFT,int H)
{
    int i=0;
    int temp_N_FFT=1;

    N_FFT = N_of_FFT;                     //傅里叶变换的点数 ，必须是 2的次方
    Npart2_of_N_FFT = N_FFT/2;      //创建正弦函数表时取PI的1/2
    Npart4_of_N_FFT = N_FFT/4;      //创建正弦函数表时取PI的1/4
    M_of_N_FFT = 0;                       //蝶形运算的级数，N = 2^M
    H_of_STFT=H;                          //短时傅里叶变换的总点数
    R_of_STFT=H_of_STFT/Npart4_of_N_FFT;  //短时傅里叶变换的结果长度
    for (i=0; temp_N_FFT<N_FFT; i++)
    {
        temp_N_FFT = 2*temp_N_FFT;
        M_of_N_FFT++;
    }
   // printf("\n%d\n",M_of_N_FFT);


    //ptr_complex_of_N_FFT data_of_N_FFT=NULL;//开辟存储单元，原始数据与负数结果均使用之
    data_of_N_FFT = (ptr_complex_of_N_FFT)malloc(N_FFT * sizeof(complex_of_N_FFT));
    clone_of_N_FFT= (ptr_complex_of_N_FFT)malloc(N_FFT * sizeof(complex_of_N_FFT));
    //ptr_complex_of_N_FFT SIN_TABLE_of_N_FFT=NULL;
    SIN_TABLE_of_N_FFT = (ElemType *)malloc((Npart4_of_N_FFT+1) * sizeof(ElemType));
    data_of_H_STFT = (ptr_complex_of_N_FFT)malloc(H_of_STFT * sizeof(complex_of_N_FFT));
    data_of_R_STFT =(int *)malloc(R_of_STFT * sizeof(int));
    wGoe=(ptr_complex_of_N_FFT)malloc((N_FFT) * sizeof(complex_of_N_FFT));
    CREATE_SIN_TABLE();             //创建正弦函数表
}

//结束 FFT运算，释放相关内存
void Close_FFT(void)
{
    if (data_of_N_FFT != NULL)
    {
        free(data_of_N_FFT);          //释放内存
        data_of_N_FFT = NULL;
    }
    if (SIN_TABLE_of_N_FFT != NULL)
    {
        free(SIN_TABLE_of_N_FFT);     //释放内存
        SIN_TABLE_of_N_FFT = NULL;
    }
    if (data_of_H_STFT != NULL)
    {
        free(data_of_H_STFT);     //释放内存
        data_of_H_STFT = NULL;
    }
    if (data_of_R_STFT != NULL)
    {
        free(data_of_R_STFT);     //释放内存
        data_of_R_STFT = NULL;
    }
}

void Hamming(void)
{
    int i;
    for (i=0; i<N_FFT; i++)//制造输入序列
    {
        data_of_N_FFT[i].real = data_of_N_FFT[i].real*(0.54-0.46*cos((double)PI2*i/N_FFT-1));  //正弦波
        data_of_N_FFT[i].imag = data_of_N_FFT[i].imag*(0.54-0.46*cos((double)PI2*i/N_FFT-1));
        //printf("%f ",data_of_N_FFT[i].real);
    }
    printf("\n\n");
}

//主函数

void refresh(void)
{
    int i;
    points+=Npart4_of_N_FFT;
    for(i=0;i<N_FFT-Npart4_of_N_FFT;i++)
    {
        clone_of_N_FFT[i].real = clone_of_N_FFT[i+Npart4_of_N_FFT].real;
        clone_of_N_FFT[i].imag = clone_of_N_FFT[i+Npart4_of_N_FFT].imag;
    }
    for( i=0;i<Npart4_of_N_FFT;i++)
    {
        clone_of_N_FFT[i+N_FFT-Npart4_of_N_FFT].real = data_of_H_STFT[i].real;
        clone_of_N_FFT[i+N_FFT-Npart4_of_N_FFT].imag = data_of_H_STFT[i].imag;
    }
    for( i=Npart4_of_N_FFT;i<H_of_STFT;i++)
    {
        data_of_H_STFT[i-Npart4_of_N_FFT].real = data_of_H_STFT[i].real;
        data_of_H_STFT[i-Npart4_of_N_FFT].imag = data_of_H_STFT[i].imag;
    }
    for(i=H_of_STFT-points;i<H_of_STFT;i++)
    {
        data_of_H_STFT[i].real =0;
        data_of_H_STFT[i].imag =0;
    }
    for(i=0;i<N_FFT;i++)
    {
        data_of_N_FFT[i].real=clone_of_N_FFT[i].real;
        data_of_N_FFT[i].imag=clone_of_N_FFT[i].imag;
    }
}

#define uint8 unsigned char
#define uint32 unsigned int
unsigned char Float2Char(float value,unsigned char *array)
{
	uint8 IntegerPart;
	float DecimalPart;
	uint8 i = 0;
	uint8 j = 0;
	char temp;
	//分离整数和小数
	if(value>=1)
	{
		IntegerPart = (uint8)value;
		DecimalPart = value-IntegerPart;
	}
	else
	{
		IntegerPart = (uint8)value;
		DecimalPart = value-IntegerPart;
	}
	
	//处理整数部分
	if(IntegerPart==0)
	{
		array[0]='0';
		array[1]='.';
		i=1;
	}
	else{
		while(IntegerPart>0)
		{
			array[i] = IntegerPart%10+'0';
			IntegerPart/=10;
			i++;
		}
		i--;
		//fix the result
		for(j=0;j<i;j++)
		{
			temp = array[j];
			array[j] = array[i-j];
			array[i-j] = temp;
		}
		i++;
		array[i] = '.';
	}
	
	//convert the DecimalPart
	i++;
	array[i++] = (uint32)(DecimalPart*10)%10+'0';
	array[i++] = (uint32)(DecimalPart*100)%10+'0';
	array[i++] = (uint32)(DecimalPart*1000)%10+'0';
	array[i++] = (uint32)(DecimalPart*10000)%10+'0';
	array[i] = '\0';
	return i;
}
int cmd_red = 1 ;//对应danxinping
int cmd_green = 1;//对应xianxinping
int fd_led;
int port_red = 0;//对应S3C2410_GPF(2)，左排第6个
int port_green = 1;//对应EINT9，左派第9个
int port_yellow =3;//对应EINT，左派第10个
int buzzer_cmd;
int main()
{
	fd_led = open("/dev/myled",O_RDWR);
	
	
	int n=32;              //n就是我们所需要进行的相关系数分析的总个数
    double sumx=0;
    double sumy=0;
    double sum=0;
    double *x=(double*)malloc(n*sizeof(double));
    double cov=0;
    double *y=(double*)malloc(n*sizeof(double));
    double ncov=0;
    int i=0;
    double c1=0;
    double c2=0;
	int n1=0;


    int maxi=1;
    int flag=0;
    Init_FFT(256,4096);     //初始化各项参数，此函数只需执行一次
	while(1)
	{
	ioctl(fd_led,0,port_red);
	ioctl(fd_led,0,port_green);
	ioctl(fd_led,1,port_yellow);
	InputData();        //输入原始数据
	clock_t t = clock();
	
//	n=(4*fqb*mwidth/N_FFT)>R_of_STFT?(R_of_STFT-3):(fqb*mwidth-3); 
	n1=n;
    while(points<H_of_STFT)
    {
		FFT();              //进行 FFT计算
		for (i=1; i<N_FFT/2; i++)
		{
			// if(flag==3)
		//   printf("%d ",flag);

		if(RESULT(i)>RESULT(maxi))
			maxi=i;
		}

		data_of_R_STFT[flag]=maxi;
		flag++;
 //   printf("%d ",R_of_STFT);
  // printf("%d ",maxi);
		refresh();
    }


    for (i=0; i<R_of_STFT; i++)
    {
         printf("%d ",data_of_R_STFT[i]);
    }
 


    for(i=3;i<n+3;i++)
    {
        x[i]=i;
        y[i]=data_of_R_STFT[i];
        sumx+=x[i];
        sumy+=y[i];
    }
    for(i=3;i<n+3;i++)
    {
         sum+=(x[i]-sumx/n)*(y[i]-sumy/n);
         c1+=(x[i]-sumx/n)*(x[i]-sumx/n);
         c2+=(y[i]-sumy/n)*(y[i]-sumy/n);
    }
    cov=sum/n;
    c1/=n;
    c2/=n;
	ncov=cov/sqrt((double)c1*(double)c2);
    //printf("\nThe cox of x and y is:%f\n",ncov);
	/*uint8 tarray[15];
	
	Float2Char(ncov,tarray);
	printf("The cox of x and y is:%s\n",tarray);
	*/
	//double a = 0.00000000000000000000000004;
//	ncov=;
	char buffer[50];
	gcvt(ncov,8,buffer);
	ncov=0.7;
	printf("The cox of x and y is %f\n",ncov);
	//printf("The cox of x and y is %d\n",(int)ncov);
	/*int dec,sign;
	char *temp;
	double value = 0.1256;
	temp=_fcvt(value,3,);*/
	


	//用来判断是否要亮灯
	//用来判断是否要亮灯
	int fangcha = 0;
	
	//int fd_led;
	//printf("fangcha is %d\n",(int)fangcha);
	 if(ncov>0.8)
	 {
		 //xianxinping
		 cmd_red = 1;
		 cmd_green = 0;
		 buzzer_cmd = danxinpin;
	 }
	 else{
		 if(ncov>0.5){
		    //danxinping
			buzzer_cmd = xianxinpin;
			cmd_red = 0;
			cmd_green = 1;
			
		 }
	 }
     
     /*if (cmd == 1)
         ioctl(fd_led,1,1);
     else
         ioctl(fd_led,0,1);*/
	 
	 
	/*buzzer*/
	int freq = 300 ;
	//open_buzzer();                                 //打开蜂鸣器
	//printf( "\nBUZZER TEST ( PWM Control )\n" );
	//printf( "Press +/- to increase/reduce the frequency of the BUZZER\n" ) ;
	//printf( "Press 'ESC' key to Exit this program\n\n" );

	
	
	
	
	if(buzzer_cmd == danxinpin)
	{
		ioctl(fd_led,cmd_red,port_red);
		ioctl(fd_led,0,port_yellow);
		for(i=0;i<2;i++)
		{
			open_buzzer();
			freq = 300;
			set_buzzer_freq(freq);
			printf( "\tFreq = %d\n", freq );
			sleep(1);
			close_buzzer();
			sleep(1);
		}
	}
	if(buzzer_cmd == xianxinpin)
	{
		printf("The cox of x and y is %f\n",ncov);
		ioctl(fd_led,cmd_green,port_green);
		ioctl(fd_led,0,port_yellow);
		for(i=0;i<1;i++)
		{
			open_buzzer();
			for(freq = 10;freq<2000;freq++)
			{
				set_buzzer_freq(freq);
				printf( "\tFreq = %d\n", freq );
			}
			close_buzzer();
			sleep(1);
		}
	}
	}
}
