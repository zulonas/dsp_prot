import time
import math
import multiprocessing as mp
import numpy as np
from scipy.fft import fft

def generate_ac(ac_voltage, ac_current):
    samp_rate = 10000000
    sleep_delay = 1 / samp_rate
    frequency = 50

    V_rms = 230
    V_phase = 0
    V_peak = V_rms * math.sqrt(2)

    I_rms = 20
    I_phase = 0
    I_peak = I_rms * np.sqrt(2)

    while True:
        current_time = time.time()
        ac_vol = V_peak * np.sin(2 * np.pi * frequency * current_time + V_phase)
        ac_cur = I_peak * np.sin(2 * np.pi * frequency * current_time + I_phase)
        ac_voltage.value = ac_vol
        ac_current.value = ac_cur
        time.sleep(sleep_delay) # XXX: short delay to avoid high cpu usage


def analyse_ac(ac_voltage, ac_current):
    samp_rate = 1000
    sleep_delay = 1 / samp_rate

    report_time = 1

    last_time = time.time()
    v_max, v_rms = 0, 0
    i_max, i_rms = 0, 0

    vol_samples = []
    cur_samples = []

    while True:
        current_time = time.time()
        vol_samples.append(ac_voltage.value)
        cur_samples.append(ac_current.value)

        if abs(ac_current.value) > i_max:
            i_max = abs(ac_current.value)

        if abs(ac_voltage.value) > v_max:
            v_max = abs(ac_voltage.value)

        if (current_time - last_time) >= report_time:
            v_rms = v_max / math.sqrt(2)
            i_rms = i_max / math.sqrt(2)

            vol_fft = fft(vol_samples)
            cur_fft = fft(cur_samples)
            vol_phase = np.angle(vol_fft[1])  # skip dc component
            cur_phase = np.angle(cur_fft[1])  # skip dc component
            phase_diff = np.rad2deg(cur_phase - vol_phase)
            phase_diff = ((phase_diff + 180) % 360) - 180

            P = v_rms * i_rms * math.cos(cur_phase - vol_phase)
            Q = abs(v_rms * i_rms * math.sin(cur_phase - vol_phase))
            S = v_rms * i_rms

            print("V_max: {:.4f} V".format(v_max))
            print("V_rms: {:.4f} V".format(v_rms))
            print("I_max: {:.4f} A".format(i_max))
            print("I_rms: {:.4f} A".format(i_rms))
            print("Phase: {:.4f} deg".format(phase_diff))
            print("P: {:.4f} W".format(P))
            print("Q: {:.4f} VAR".format(Q))
            print("S: {:.4f} VA".format(S))
            print("")

            last_time = current_time
            i_max, v_max = 0, 0
            vol_samples.clear()
            cur_samples.clear()

        time.sleep(sleep_delay)

def main():
    ac_voltage = mp.Value('d', 0.0)
    ac_current = mp.Value('d', 0.0)

    p1 = mp.Process(target=generate_ac, args=(ac_voltage, ac_current))
    p1.start()
    p2 = mp.Process(target=analyse_ac, args=(ac_voltage, ac_current))
    p2.start()

if __name__ == '__main__':
    main()