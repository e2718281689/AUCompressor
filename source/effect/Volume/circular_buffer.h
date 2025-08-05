#ifndef CIRCULAR_BUFFER_FLOAT_H
#define CIRCULAR_BUFFER_FLOAT_H

#include <stddef.h> // for size_t

// 专用于 float 的环形缓冲区结构体
typedef struct {
    float *buffer;     // 指向实际存储 float 的内存区域
    size_t capacity;   // 缓冲区的总容量（以 float 元素数量计）
    size_t size;       // 当前存储的 float 元素数量
    size_t read_pos;   // 读指针（索引）
    size_t write_pos;  // 写指针（索引）
} circular_buffer_float_t;

/**
 * @brief 初始化一个 float 环形缓冲区
 *
 * @param cb 指向要初始化的环形缓冲区结构体的指针
 * @param buffer 指向用户提供的用于存储 float 的内存块的指针
 * @param capacity 内存块的容量（能存储多少个 float）
 */
void circular_buffer_float_init(circular_buffer_float_t *cb, float *buffer, size_t capacity);

/**
 * @brief 重置一个环形缓冲区，清空所有数据
 *
 * @param cb 指向环形缓冲区的指针
 */
void circular_buffer_float_reset(circular_buffer_float_t *cb);

/**
 * @brief 向环形缓冲区写入 float 数据
 *
 * @param cb 指向环形缓冲区的指针
 * @param data 指向要写入的 float 数组的指针
 * @param count 要写入的 float 元素的数量
 * @return size_t 实际写入的 float 元素的数量
 */
size_t circular_buffer_float_write(circular_buffer_float_t *cb, const float *data, size_t count);

/**
 * @brief 从环形缓冲区读取 float 数据
 *
 * @param cb 指向环形缓冲区的指针
 * @param data 指向用于存放读取数据的 float 数组的指针
 * @param count 想要读取的 float 元素的数量
 * @return size_t 实际读取的 float 元素的数量
 */
size_t circular_buffer_float_read(circular_buffer_float_t *cb, float *data, size_t count);

/**
 * @brief 获取缓冲区中当前存储的 float 元素数量
 *
 * @param cb 指向环形缓冲区的指针
 * @return size_t 当前数据大小（元素数量）
 */
size_t circular_buffer_float_get_size(const circular_buffer_float_t *cb);

/**
 * @brief 获取缓冲区的剩余可用空间大小（还能存多少个 float）
 *
 * @param cb 指向环形缓冲区的指针
 * @return size_t 剩余空间大小（元素数量）
 */
size_t circular_buffer_float_get_free_space(const circular_buffer_float_t *cb);

/**
 * @brief 从环形缓冲区“窥视”数据，但不移动读指针。
 *
 * 这个函数用于读取缓冲区中的数据副本，而不会将其从缓冲区中移除。
 *
 * @param cb 指向环形缓冲区的指针 (const, 因为不修改其状态)
 * @param data 指向用于存放读取数据的 float 数组的指针
 * @param count 想要窥视的 float 元素的数量
 * @return size_t 实际窥视到的 float 元素的数量 (如果数据不足，会小于 count)
 */
size_t circular_buffer_float_peek(const circular_buffer_float_t *cb, float *data, size_t count);


#endif // CIRCULAR_BUFFER_FLOAT_H
