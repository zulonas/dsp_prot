import numpy as np
import matplotlib.pyplot as plt

# Reading the data efficiently
times = []
values = []
with open('../signal.csv', 'r') as f:
    for line in f.readlines()[2:]:
        time, value = line.strip().split(',')
        times.append(float(time))
        values.append(float(value))


values_fft = np.fft.fft(values)
frequencies = np.fft.fftfreq(len(values), d=(times[1]-times[0]))

positive_freq_idxs = np.where(frequencies > 0)
values_magnitude = np.abs(values_fft[positive_freq_idxs])
frequencies_positive = frequencies[positive_freq_idxs]

peak_frequency = frequencies_positive[np.argmax(values_magnitude)]

# time domain
plt.figure()
plt.plot(times, values)
plt.title("Signal Over Time")
plt.xlabel("Time, s")
plt.ylabel("Amplitude")
plt.xlim(0, 0.05)
plt.show()

# freq domain
plt.figure()
plt.plot(frequencies_positive, values_magnitude)
plt.fill_between(frequencies_positive, values_magnitude, color='skyblue', alpha=0.4)
plt.title(f"Dominant Frequency: {peak_frequency:.2f} Hz")
plt.xlabel('Frequency, Hz')
plt.ylabel('Magnitude')
plt.yscale('log')
plt.show()