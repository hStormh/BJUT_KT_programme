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

/*自定义函数引用*/
void origin_data_get (double datas[32][32][1000]);   
void time_matrix_gener ()

int main()
{
    /*变量和数组定义部分*/
    double ultrasonic_wave_velocity = 5100000; //mm/s 用于探测的超声波在材料中的波速
    double sampling_frequency = 100; //MHz  采样频率
    double array_element_width = 0.5; //mm  探头阵元的宽度
    double array_element_distance = 0.6; //mm 探头阵元的距离
    double mesh_length = 8; //mm 网格的长度  确定依据是 【32*0.5+31*0.6】/3
    double sampling_time = 0.00005; //s 采样时间 = 总采样点数/采样频率
    
    double datas[32][32][1000] = {0}; //模拟接收探头传来的32*32*1000的幅值信号数据

    origin_data_get(datas);



    return 0;
}


/*功能函数*/
void origin_data_get (double datas[32][32][1000])
{
    for (int i = 0; i < 32; i++)
    {
        srand(time(0)); //初始化种子
        for (int j = 0; j < 32; j++)
        {
            for (int k = 0; k < 1000; k++)
            {
                datas[i][j][k] = (double)rand()/RAND_MAX;  //生成0-1之间的随机数
            }
        }
    }
}

//void 