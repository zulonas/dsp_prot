#include <math.h>
#include <stdio.h>

#define SAMPLES_QTY 1024 * 20
#define SAMPLING_FREQ 10000

#define BL 21
float B[21] = {
	-0, -0.0001953169703, -0.001555188675, -0.004481135402, -0.006398354657,
	2.709078131e-18, 0.02436965145, 0.07107453048, 0.1313721538, 0.183613494,
	0.2044003457, 0.183613494, 0.1313721538, 0.07107453048, 0.02436965145,
	2.709078131e-18, -0.006398354657, -0.004481135402, -0.001555188675, -0.0001953169703,
	-0
};

/**
 * Applies an FIR filter to an input signal.

 * @input: Pointer to input signal array.
 * @coefs: Pointer to filter coefficients array.
 * @output: Pointer to output signal array.
 * @input_size: Number of elements in input signal array.
 * @coefs_size: Number of filter coefficients.
 */
void fir(const float *input, const float *coefs, float *output, size_t input_size, size_t coefs_size)
{
	float sum;

	for (size_t i = 0; i < input_size; i++)
	{
		sum = 0;
		for (size_t y = 0; y < coefs_size; y++)
		{
			if (i + y >= input_size)
				continue;
			sum += input[i + y] * coefs[y];
		}
		output[i] = sum;
	}
}

int main() {
	FILE *fp;

	float input[SAMPLES_QTY];
	float output[SAMPLES_QTY] = {0};

	for (int index = 0; index < SAMPLES_QTY; index++) {
		input[index] = 1000 * sin(1000 * 2 * M_PI * index / SAMPLING_FREQ) +
			       1000 * sin(3000 * 2 * M_PI * index / SAMPLING_FREQ);
	}

	fir(input, B, output, SAMPLES_QTY, BL);

	fp = fopen("signal.csv", "w");
	if (!fp) {
		printf("Failed to open file for writing.\n");
		return -1;
	}

	for (int i = 0; i < SAMPLES_QTY; i++) {
		fprintf(fp, "%f,%f\n", ((float)i / SAMPLING_FREQ), output[i]);
	}

	fclose(fp);
	printf("Output successfully written to analog.csv\n");

	return 0;
}
