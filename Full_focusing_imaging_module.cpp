/*
文档说明：
此部分是后处理的全聚焦成像算法的模块。
23.11.21
*/
/*宏定义部分*/

/*引入标准库*/
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

/*自定义函数引用*/
void origin_data_get (double datas[32][32][1000]);   
void time_matrix_gener (double time_matrix[32][2][3]);
void amplitude_sum (double time_matrix[32][2][3], double datas[32][32][1000]);


int main()
{
    
    /*次要问题，定义的变量没有在函数中使用*/
    double sampling_frequency = 100; //MHz  采样频率
    double array_element_width = 0.5; //mm  探头阵元的宽度
    double array_element_distance = 0.6; //mm 探头阵元的距离
    double mesh_length = 8; //mm 网格的长度  确定依据是 【32*0.5+31*0.6】/3
    double sampling_time = 0.00001; //s 采样时间 = 总采样点数/采样频率  计算有误


    static double datas[32][32][1000] = {0}; //模拟接收探头传来的32*32*1000的幅值信号数据
    double time[32][2][3] = {0};//时间参数空数组；
    
    /*自定义函数在主函数中的调用*/
    origin_data_get (datas); //主函数生成归一化数据
    time_matrix_gener (time);//主函数中时间矩阵生成
    amplitude_sum (time, datas);//幅值叠加函数
    
    return 0;
}


/*功能函数*/

/*
原始幅值数据模拟生成函数：
目的：生成用于模拟探头接收的超声波幅值数据（经过归一化处理，范围0-1）
功能：生成随机数，利用三层for循环嵌套，同时为三维空数组赋值
输入参数：空的32*32*1000的空三维数组用于存储随机生成的数据
返回值：无
*/

void origin_data_get (double datas[32][32][1000])
{
    srand(time(0)); //初始化种子
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            for (int k = 0; k < 1000; k++)
            {
                datas[i][j][k] = (double)rand()/RAND_MAX;  //生成0-1之间的随机数
            }
        }
    }
}

// 时间数组生成
/*
探头激励到各个网格点时间数据生成函数：
目的：计算出三十二个探头到各个网格点的时间，并赋值到三维时间数组中
功能：先利用平面两点距离公式，算出各个探头到各个网格点的距离，
然后利用距离除以超声波速度得到使用的时间。
输入参数：空的32*2*3的空三维数组用于存储计算好的时间数据
返回值：无
*/

void time_matrix_gener (double time_matrix[32][2][3])
{   float ultrasonic_wave_velocity = 5100000; //mm/s 用于探测的超声波在材料中的波速
    double d;
    int i, j, k;
    for(i = 0; i < 32; i++)
    {
        for(j = 0; j < 2; j++)
        {
            for(k = 0; k<3; k++)
            {
                double delx = pow(((8/2 + 8*k) - (0.5/2 + (0.5 + 0.6)*i)), 2); 
                double dely = pow((8/2 + 8*j), 2); 
                //分别是网格点与探头横，纵坐标之差的平方
                
                /*注意这一部分后期可能要改，目前的表达式是参考v1.0
                平面示意图， 参考图中的坐标系和探头放置位置*/
                
                d=pow((delx + dely),0.5);
                time_matrix [i][j][k] = d / ultrasonic_wave_velocity; //计算出时间
            }
        }
    }
}

// 幅值叠加数组生成
/*
探头激励到各个网格点时间数据生成函数：
目的：通过嵌套循环将对同一网格点，不同激励接受探头组合接收到的幅值数据进行代数累加
功能：先利用for循环，对探头以一激励一接收的方式两两组合，得到不同组合下超声波传播
时间ti，然后利用ti/取样时间 = 目标幅值数据位置/总取样点数 得到目标幅值数据，然后
叠加在probe_sum数组对应位置，最后打印2*3的幅值叠加结果数组。 
输入参数：三维时间矩阵（各个探头到各个网格点的时间） 存储探头接收的幅值数据的三维数组
返回值：无
*/

void amplitude_sum (double time_matrix[32][2][3], double datas[32][32][1000])//包含最终结果输出
{
    /*变量和结构体定义定义*/
    double time_point = 0;
    int tar = 0;
    double probe_sum[2][3] = {0};
   

    for (int i = 0; i < 32; i++) //前两层循环用来遍历激励和接收探头
    {
        for (int j = 0; j < 32; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int h = 0; h < 3; h++)
                {
                    time_point = time_matrix[i][k][h] + time_matrix[j][k][h]; //遍历传播时长
                    tar = 1000*(time_point / 0.00005); //根据比例，得到第几个采样点与上面的传播时长对应
                    probe_sum[k][h] += datas[i][j][tar-1];

                }
                   
                
            }
        }
    }
    
    
    
    for (int i = 0; i < 2; i++) //打印各个网格点的叠加后的幅值
    {
        for (int j = 0; j < 3; j++)
        {
            printf("Amplitude:probe(%d,%d):%lf\n", i+1, j+1, probe_sum[i][j]);//分网格点打印
        }
    }
    
    
}


