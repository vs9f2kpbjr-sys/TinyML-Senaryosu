#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef enum {
    TENSOR_FLOAT32,
    TENSOR_FLOAT16,
    TENSOR_INT8
} TensorType;

typedef struct {
    void* data;
    size_t size;
    TensorType type;
} Tensor;

Tensor tensor_create(size_t size, TensorType type) {
    Tensor t;
    t.size = size;
    t.type = type;

    size_t bytes = 0;

    switch(type) {
        case TENSOR_FLOAT32:
            bytes = sizeof(float) * size;
            break;
        case TENSOR_FLOAT16:
            bytes = sizeof(uint16_t) * size;
            break;
        case TENSOR_INT8:
            bytes = sizeof(int8_t) * size;
            break;
    }

    t.data = malloc(bytes);

    if (!t.data) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    return t;
}

void tensor_free(Tensor* t) {
    free(t->data);
    t->data = NULL;
}

void tensor_quantize_float32_to_int8(Tensor* src, Tensor* dst, float scale) {
    float* input = (float*)src->data;
    int8_t* output = (int8_t*)dst->data;

    for (size_t i = 0; i < src->size; i++) {
        output[i] = (int8_t)roundf(input[i] / scale);
    }
}

int main(void) {
    
    size_t size = 10;
    
    Tensor floatTensor = tensor_create(size, TENSOR_FLOAT32);
    float* data = (float*)floatTensor.data;
    
    for (int i = 0; i < size; i++) {
        data[i] = i * 0.5f;
    }
    
    Tensor int8Tensor = tensor_create(size, TENSOR_INT8);
    
    tensor_quantize_float32_to_int8(&floatTensor, &int8Tensor, 0.1f);
    
    printf("Float32 Tensor:\n");
    for (int i = 0; i < size; i++) {
        printf("%.2f ", data[i]);
    }
    
    printf("\n\nInt8 Quantized Tensor:\n");
    
    int8_t* qdata = (int8_t*)int8Tensor.data;
    for (int i = 0; i < size; i++) {
        printf("%d ", qdata[i]);
    }
    
    printf("\n\nMemory Usage:\n");
    printf("Float32: %lu bytes\n", sizeof(float) * size);
    printf("Int8: %lu bytes\n", sizeof(int8_t) * size);
    
    tensor_free(&floatTensor);
    tensor_free(&int8Tensor);
    return 0;
}


