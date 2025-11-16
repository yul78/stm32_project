#include "command.h"
#include "usart.h"
#include <stdio.h>

#define BUFFER_SIZE 128

uint8_t buffer[BUFFER_SIZE];

uint8_t readIndex = 0;
uint8_t writeIndex = 0;

//数据帧格式：一位包头 + 一位数据（代表数据长度） + 数据内容 + 一位校验和

/*
*@brief 增加读索引
*param length 要增加的长度
*/
void Command_AddReadIndex(uint8_t length)
{
    readIndex += length;
    readIndex %= BUFFER_SIZE;
}

/*
*@brief 读取缓冲区的第i个数据，超过缓冲区长度自动循环
*@param i 要读取的数据索引
*/
uint8_t Command_Read(uint8_t i)
{
    uint8_t index = i % BUFFER_SIZE;
    return buffer[index];
}

// uint8_t Command_GetLength(void)
// {
//     if(readIndex == writeIndex) //读索引等于写索引，缓冲区为空
//     {
//         return 0;
//     }
//     if(writeIndex + 1 == readIndex || writeIndex == BUFFER_SIZE - 1 && readIndex == 0)
//     {
//         return BUFFER_SIZE; //缓冲区已满，返回BUFFER_SIZE

//     }
//     if(readIndex < writeIndex)
//     {
//         return writeIndex - readIndex;//缓冲区未满，返回缓冲区中剩余的数据长度

//     }
//     else
//     {
//         return BUFFER_SIZE - (readIndex - writeIndex);
//     }
// }

/*
*@brief 获取缓冲区中剩余数据的长度，也就是可读数据长度
*/
uint8_t Command_GetLength(void)
{
    return (writeIndex + BUFFER_SIZE - readIndex) % BUFFER_SIZE; //等效于上面的函数实现
}

/*
*获取缓冲区中剩余可写入的数据长度
*/
uint8_t Command_GetRemain(void)
{
    return BUFFER_SIZE - Command_GetLength();
}

/*
*@brief 向缓冲区写入数据
*@param data 要写入的数据的指针
*@param length 要写入的数据长度
*@return 实际写入的数据长度，写入失败返回0
*/
uint8_t Command_Write(uint8_t *data, uint8_t length)
{
    if(Command_GetRemain() < length)
    {
        return 0; //缓冲区剩余空间不足，写入失败
    }
    if(writeIndex + length < BUFFER_SIZE)
    {
        memcpy(buffer + writeIndex, data, length);
        writeIndex += length;
    }
    else
    {
        uint8_t firstLength = BUFFER_SIZE - writeIndex;
        memcpy(buffer + writeIndex, data, firstLength);
        memcpy(buffer, data + firstLength, length - firstLength);
        writeIndex = length - firstLength;
    }


    return length;

}

uint8_t Command_GetCommand(uint8_t *command)
{
    while(1)
    {
        if(Command_GetLength() == 0)
        {
            return 0;
        }
        if(Command_Read(readIndex) != 0xAA)
        {
            // uint8_t read_error_msg[] = "Command_Read firt not 0xAA\r\n";
            // HAL_UART_Transmit(&huart2, read_error_msg, sizeof(read_error_msg)-1, HAL_MAX_DELAY);
            Command_AddReadIndex(1);
            continue;
        }
        uint8_t length = Command_Read(readIndex + 1);//这里是数据规则，包头后的数据代表数据长度

        // uint8_t length_str[4];
        // uint8_t length_msg[] = "Command length = ";
        // sprintf((char*)length_str, "%d /r/n", length);
        // HAL_UART_Transmit(&huart2, length_msg, sizeof(length_msg)-1, HAL_MAX_DELAY);
        // HAL_UART_Transmit(&huart2, length_str, sizeof(length_str)-1, HAL_MAX_DELAY);

        if(Command_GetLength() < length)
        {
            // uint8_t length_str[4];
            // uint8_t length_msg[] = "Command_GetLength = ";
            // sprintf((char*)length_str, "%d /r/n", Command_GetLength());
            // HAL_UART_Transmit(&huart2, length_msg, sizeof(length_msg)-1, HAL_MAX_DELAY);
            // HAL_UART_Transmit(&huart2, length_str, sizeof(length_str)-1, HAL_MAX_DELAY);

            // uint8_t read_error_msg[] = "readable data length is less than the data length to be read\r\n";
            // HAL_UART_Transmit(&huart2, read_error_msg, sizeof(read_error_msg)-1, HAL_MAX_DELAY);
            return 0;
        }

        uint8_t sum = 0;
        for(uint8_t i = 0; i < length - 1; i++)
        {
            sum += Command_Read(readIndex + i);
        }
        if(sum != Command_Read(readIndex + length - 1))
        {
            Command_AddReadIndex(1);
            continue;
        }
        for(uint8_t i = 0; i< length; i++)
        {
            command[i] = Command_Read(readIndex + i);
        }
        Command_AddReadIndex(length);
        return length;
    }
}
