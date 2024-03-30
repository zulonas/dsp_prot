#include <stdio.h>
#include <math.h>

#define SAMPLES_QTY 1024
#define SAMPLING_FREQ 10000

/**
 * Forward Forier transform (Cooley-Tukey)

 * @input_re: Input signal array.
 * @output_re: Transform result (real part)
 * @output_im: Transform result (imaginary part)
 * @N: Length of both input data and result (must be power of 2 - N = 2^x).

 *
 * This fft implementation is based on librow library.
 * ref: http://www.librow.com/articles/article-10
 */
int fft(const double input_re[], double output_re[], double output_im[], const unsigned int N)
{
	unsigned int mask;
	unsigned int target = 0;
	unsigned int jump;
	double delta;
	double sine;
	double multiplier_re, multiplier_im;
	double factor_re, factor_im;
	double factor_re_tmp;
	unsigned int match;
	double product_re, product_im;

	if (!input_re || !output_re || !output_im || N < 1 || N & (N - 1))
		return 0;

	for (unsigned int position = 0; position < N; ++position) {
		output_re[target] = input_re[position];
		output_im[target] = 0;
		mask = N;
		while (target & (mask >>= 1))
			target &= ~mask;
		target |= mask;
	}

	for (unsigned int step = 1; step < N; step <<= 1) {
		jump = step << 1;
		delta = M_PI / (double)step;
		sine = sin(delta * .5);
		multiplier_re = -2. * sine * sine;
		multiplier_im = sin(delta);
		factor_re = 1.;
		factor_im = 0.;

		for (unsigned int group = 0; group < step; ++group) {
			for (unsigned int pair = group; pair < N; pair += jump) {
				match = pair + step;
				product_re = factor_re * output_re[match] - factor_im * output_im[match];
				product_im = factor_re * output_im[match] + factor_im * output_re[match];
				output_re[match] = output_re[pair] - product_re;
				output_im[match] = output_im[pair] - product_im;
				output_re[pair] += product_re;
				output_im[pair] += product_im;
			}
			factor_re_tmp = factor_re;
			factor_re = multiplier_re * factor_re - multiplier_im * factor_im + factor_re;
			factor_im = multiplier_re * factor_im + multiplier_im * factor_re_tmp + factor_im;
		}
	}

	return 1;
}

int main() {
	double Input[SAMPLES_QTY];
	double complexoutput_re[SAMPLES_QTY];
	double complexoutput_im[SAMPLES_QTY];

	for (int index = 0; index < SAMPLES_QTY; index++) {
		Input[index] = 1000 * sin(1000 * 2 * M_PI * index / SAMPLING_FREQ) +
			       1000 * sin(3000 * 2 * M_PI * index / SAMPLING_FREQ);
	}

	if (!fft(Input, complexoutput_re, complexoutput_im, SAMPLES_QTY)) {
		fprintf(stderr, "FFT computation failed.\n");
		return 1;
	}

	for (int i = 0; i < SAMPLES_QTY / 2; i++) {
		double magnitude = sqrt(complexoutput_re[i] * complexoutput_re[i] +
					complexoutput_im[i] * complexoutput_im[i]);
		printf("Frequency %d Hz: Magnitude = %f\n", i * SAMPLING_FREQ / SAMPLES_QTY, magnitude);
	}

	return 0;
}
