#include <stdio.h>
#include <math.h>

int quad(float a, float b, float c, float *r1, float *r2)
{
    float det = b * b - 4.0f * a * c;
    if(det < 0.0f)
        return 0;
    *r1 = (-b + sqrt(det)) / (2.0f * a);
    if(det == 0.0f)
        return 1;
    *r2 = (-b - sqrt(det)) / (2.0f * a);
    return 2;
}

int main(void)
{
    FILE *fp = fopen("test.txt", "r");
    if(!fp) {
        fprintf(stderr, "File not found\n");
        return 1;
    }
    float largest_root = -1.0f;
    float largest_a = -1.0f;
    float largest_b = -1.0f;
    float largest_c = -1.0f;
    for(int i = 0; i < 10000; i++) {
        float a;
        float b;
        float c;
        int num_read = fscanf(fp, "%f %f %f", &a, &b, &c);
        if(num_read != 3) {
            fprintf(stderr, "Unable to read values: could only read %d\n", num_read);
            fclose(fp);
            return 1;
        }
        float r1, r2;
        int num_roots = quad(a, b, c, &r1, &r2);
        if(num_roots == 2) {
            if(r2 > largest_root) {
                largest_root = r2;
                largest_a = a;
                largest_b = b;
                largest_c = c;
            }
        }
        if(num_roots != 0) {
            if(r1 > largest_root) {
                largest_root = r1;
                largest_a = a;
                largest_b = b;
                largest_c = c;
            }
        }
    }
    fclose(fp);
    printf("Largest root is %.2f. The function is (%.3f, %.3f, %.3f).\n",
            largest_root, largest_a, largest_b, largest_c);
}

