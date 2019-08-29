#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "dlr.h"

int argmax(float *arr, size_t sz);

size_t read_img(const char *path, float *img, size_t sz);

long long ms();

void run(DLRModelHandle *model, const char *input_name, const int64_t *input_shape, float *data, int num_outputs, int *out_sizes, float** outputs);

int main() {
    const char *model_path = "resnet18v2";
    printf("Loading model: %s\n", model_path);
    //dlr::DLRModelHandle *handle;
    DLRModelHandle model;
    if (CreateDLRModel(&model, model_path, 1, 0) != 0) {
        printf("Error CreateDLRModel: %s\n", DLRGetLastError());
        exit(-1);
    }
    printf("CreateDLRModel: OK\n");

    // Get Number of outputs
    int num_outputs;
    GetDLRNumOutputs(&model, &num_outputs);
    printf("NumOutputs: %d\n", num_outputs);
    int out_sizes[num_outputs];
    float* outputs[num_outputs];
    for (int i = 0; i < num_outputs; i++) {
        int64_t cur_size = 0;
        int cur_dim = 0;
        GetDLROutputSizeDim(&model, 0, &cur_size, &cur_dim);
        out_sizes[i] = cur_size;
        float *output = (float *) malloc(cur_size*sizeof(float)); // new float[cur_size];
        for (size_t i=0; i<cur_size; i++) {
          output[i] = 0.0f;
        }
        outputs[i] = output;
        printf("output_id: %d, cur_size: %ld, cur_dim: %d\n", i, cur_size, cur_dim);
    }
    // SetDLRInput
    const char *input_name = "data";
    const int64_t input_shape[4] = {1,3,224,224};
    int data_sz = (int)(input_shape[1]*input_shape[2]*input_shape[3]);
    float *data = (float *) malloc(data_sz*sizeof(float)); // new float[data_sz];
    read_img("dog.txt", data, data_sz);
    for (int i = 0; i < 10; i++) {
        long long t1 = ms();
        run(&model, input_name, input_shape, data, num_outputs, out_sizes, outputs);
        long long t2 = ms();
        printf("%d: Time: %lld ms\n", i, t2-t1);
        //printf("%d: Time: %ld.%06ld\n", i, tdiff.tv_sec, tdiff.tv_usec);
      }
}

void run(DLRModelHandle *model, const char *input_name, const int64_t *input_shape, float *data, int num_outputs, int *out_sizes, float** outputs) {
    //memset(data, -0.34455, data_sz);
    if (SetDLRInput(model, input_name, input_shape, data, 4) != 0) {
        printf("Error SetDLRInput: %s\n", DLRGetLastError());
        exit(-1);
    }
    printf("SetDLRInput: OK\n");

    if (RunDLRModel(model) != 0) {
        printf("Error SetDLRInput: %s\n", DLRGetLastError());
        exit(-1);
    }
    printf("RunDLRModel: OK\n");
    
    for (int i = 0; i < num_outputs; i++) {
      if (GetDLROutput(model, i, outputs[i]) != 0) {
          printf("Error GetDLROutput[%d]: %s\n", i, DLRGetLastError());
          exit(-1);
      }
      printf("Output[%d](%d): ", i, out_sizes[i]);
      for (size_t j=0; j<9; j++) {
          printf("%.4g, ", outputs[i][j]);
      }
      printf("... ");
      for (size_t j=out_sizes[i]-9; j<out_sizes[i]; j++) {
          printf("%.4g, ", outputs[i][j]);
      }
      printf("\n");
      int m = argmax(outputs[i], out_sizes[i]);
      printf("Argmax(id, val): (%d, %.4g)\n", m, outputs[i][m]);
    }
    printf("GetDLROutput: OK\n");
}

int argmax(float *arr, size_t sz) {
    if (sz <= 0) {
        return -1;
    }
    int m = 0;
    float mv = arr[0];
    for (int i = 1; i < sz; i++) {
        if (arr[i] > mv) {
            m = i;
            mv = arr[i];
        }
    }
    return m;
}

size_t read_img(const char *path, float *img, size_t sz) {
    printf("img_name: %s\n", path);


    FILE * fp;
    int max_len = 80;
    char line[max_len+1];

    fp = fopen(path, "r");
    if (fp == NULL)
        exit(-1);

    size_t i = 0;
    while (fgets(line, max_len, fp) != NULL && i < sz) {
        float v = (float) atof(line);
        img[i] = v;
        i++;
    }

    fclose(fp);

    printf("Image read ok, size: %ld\n", i);
    return i;
}

long long ms() {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}
