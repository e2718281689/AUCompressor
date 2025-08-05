#include "circular_buffer.h"
#include <string.h> // for memcpy

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void circular_buffer_float_init(circular_buffer_float_t *cb, float *buffer, size_t capacity) {
    cb->buffer = buffer;
    cb->capacity = capacity;
    circular_buffer_float_reset(cb);
}

void circular_buffer_float_reset(circular_buffer_float_t *cb) {
    cb->read_pos = 0;
    cb->write_pos = 0;
    cb->size = 0;
}

size_t circular_buffer_float_get_size(const circular_buffer_float_t *cb) {
    return cb->size;
}

size_t circular_buffer_float_get_free_space(const circular_buffer_float_t *cb) {
    return cb->capacity - cb->size;
}

size_t circular_buffer_float_write(circular_buffer_float_t *cb, const float *data, size_t count) {
    if (data == NULL || count == 0) {
        return 0;
    }

    size_t free_space = circular_buffer_float_get_free_space(cb);
    size_t elements_to_write = MIN(count, free_space);

    if (elements_to_write == 0) {
        return 0;
    }

    // 第一部分：从 write_pos 到缓冲区末尾
    size_t part1_count = cb->capacity - cb->write_pos;
    part1_count = MIN(elements_to_write, part1_count);
    memcpy(&cb->buffer[cb->write_pos], data, part1_count * sizeof(float));

    // 第二部分：如果需要回环，从缓冲区开头继续
    size_t part2_count = elements_to_write - part1_count;
    if (part2_count > 0) {
        // 注意：源指针 data 也需要相应地偏移
        memcpy(&cb->buffer[0], data + part1_count, part2_count * sizeof(float));
    }

    cb->write_pos = (cb->write_pos + elements_to_write) % cb->capacity;
    cb->size += elements_to_write;

    return elements_to_write;
}

size_t circular_buffer_float_read(circular_buffer_float_t *cb, float *data, size_t count) {
    if (data == NULL || count == 0) {
        return 0;
    }

    size_t stored_size = circular_buffer_float_get_size(cb);
    size_t elements_to_read = MIN(count, stored_size);

    if (elements_to_read == 0) {
        return 0;
    }

    // 第一部分：从 read_pos 到缓冲区末尾
    size_t part1_count = cb->capacity - cb->read_pos;
    part1_count = MIN(elements_to_read, part1_count);
    memcpy(data, &cb->buffer[cb->read_pos], part1_count * sizeof(float));

    // 第二部分：如果需要回环，从缓冲区开头继续
    size_t part2_count = elements_to_read - part1_count;
    if (part2_count > 0) {
        // 注意：目标指针 data 也需要相应地偏移
        memcpy(data + part1_count, &cb->buffer[0], part2_count * sizeof(float));
    }

    cb->read_pos = (cb->read_pos + elements_to_read) % cb->capacity;
    cb->size -= elements_to_read;

    return elements_to_read;
}


size_t circular_buffer_float_peek(const circular_buffer_float_t *cb, float *data, size_t count) {
    if (data == NULL || count == 0) {
        return 0;
    }

    size_t stored_size = circular_buffer_float_get_size(cb);
    size_t elements_to_peek = MIN(count, stored_size);

    if (elements_to_peek == 0) {
        return 0;
    }

    // 复制逻辑与 read 函数完全相同
    // 第一部分：从 read_pos 到缓冲区末尾
    size_t part1_count = cb->capacity - cb->read_pos;
    part1_count = MIN(elements_to_peek, part1_count);
    memcpy(data, &cb->buffer[cb->read_pos], part1_count * sizeof(float));

    // 第二部分：如果需要回环，从缓冲区开头继续
    size_t part2_count = elements_to_peek - part1_count;
    if (part2_count > 0) {
        memcpy(data + part1_count, &cb->buffer[0], part2_count * sizeof(float));
    }

    // 关键区别：不更新 cb->read_pos 和 cb->size
    // cb->read_pos = (cb->read_pos + elements_to_peek) % cb->capacity; // 这行被注释掉
    // cb->size -= elements_to_peek;                                  // 这行被注释掉

    return elements_to_peek;
}