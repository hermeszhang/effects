#!/usr/bin/python
#
# This program is used to show the frequency response of a FIR filter function.
#
# You should plot the spectrum with gnuplot using commands such as:
#
# plot "foo.txt" with lines smooth cspline

import math


data1 = [
     0.004986, -0.002537,  0.006342, -0.004659,  0.007869, -0.006331, 
     0.008377, -0.006550,  0.007738, -0.005698,  0.007768, -0.005335, 
     0.007260, -0.004664,  0.006612, -0.003812,  0.005838, -0.004405, 
     0.007113, -0.007115,  0.015692,  0.002627,  0.031569,  0.008532, 
     0.052333, -0.008365,  0.048718, -0.141173,  0.046972,  0.095599, 
    -0.135736,  0.362844,  0.151924, -0.065013,  0.050873, -0.075390, 
    -0.118970,  0.049817, -0.033563, -0.031216, -0.003656, -0.064643, 
     0.041940,  0.043165, -0.052521,  0.004099,  0.002140,  0.026028, 
    -0.032987, -0.000736,  0.006253,  0.009038,  0.006009,  0.002601, 
     0.020352, -0.004335, -0.013565, -0.003661, -0.037846, -0.018088, 
    -0.011721, -0.002367, -0.014035, -0.014858,  0.006459, -0.000723, 
     0.001116,  0.000881,  0.007328, -0.005296,  0.001751, -0.000011, 
     0.001881, -0.006634, -0.000740, -0.000638, -0.002598, -0.001574, 
    -0.003153, -0.004078,  0.002361,  0.004277,  0.003830, -0.002545, 
     0.000357, -0.003216,  0.000099, -0.009948,  0.000373, -0.005445, 
     0.001305, -0.003067,  0.001903, -0.002884,  0.007837, -0.002687, 
     0.004453, -0.002268,  0.001768, -0.004447,  0.001257, -0.004451, 
    -0.002659,  0.000009, -0.001593,  0.003743,  0.004007,  0.007372, 
    -0.007478, -0.004597, -0.010553, -0.000331, -0.001606, -0.004110, 
    -0.006081, -0.005544, -0.000924, -0.004018, -0.000264, -0.001317, 
    -0.000391, -0.000088, -0.003487,  0.001793, -0.005928,  0.000223, 
    -0.003907,  0.000850, -0.004002, -0.000072,  0.001499,  0.002396, 
    -0.000067,  0.003475, -0.001126,  0.003663, -0.007653,  0.004747, 
    -0.006614,  0.004946, -0.010606,  0.006166, -0.012170,  0.005453, 
    -0.007773,  0.006034, -0.004576, -0.000806, -0.002190,  0.001022, 
    -0.001369, -0.000459, -0.003006,  0.004121, -0.000308,  0.003019, 
    -0.000629,  0.001537, -0.000121,  0.000501, -0.000376,  0.002009, 
    -0.003096,  0.002983, -0.003085,  0.002803, -0.006194,  0.002104, 
    -0.004601,  0.002636, -0.003811,  0.002692, -0.001917,  0.001934, 
    -0.000446, -0.000198,  0.002128, -0.001341,  0.002213, -0.001165, 
     0.000270, -0.000428,  0.001611,  0.000235, -0.000267, -0.000445, 
     0.000717,  0.002290,  0.000963,  0.001353,  0.001430,  0.002659, 
     0.001525,  0.002227,  0.001569,  0.001708,  0.000435,  0.003848, 
     0.000040,  0.002717, -0.001538,  0.003731, -0.002316,  0.004003, 
    -0.002589,  0.002970, -0.000807,  0.000739,  0.000828, -0.000311, 
     0.002407, -0.003592,  0.003870, -0.004227,  0.005896, -0.006244, 
     0.005276, -0.003241,  0.003548, -0.001975,  0.001244,  0.000247, 
     0.001449,  0.002070, -0.000934,  0.001864, -0.003167,  0.004715, 
    -0.003413,  0.002943, -0.003883,  0.002145,  0.001310, -0.000263, 
     0.002094, -0.000394,  0.001682, -0.000176,  0.002402,  0.000941, 
     0.001438,  0.000827,  0.000600,  0.000739,  0.000954,  0.000670, 
     0.000176,  0.000804, -0.001085,  0.000983,  0.000113,  0.000774, 
    -0.000607,  0.000371, -0.000725, -0.000457,  0.000086, -0.000236, 
     0.000388, -0.000619, -0.000873,  0.001711, -0.001330,  0.001986, 
    -0.002752,  0.001821, -0.000252,  0.000362,  0.000577,  0.000194, 
     0.000272,  0.001544, -0.000736,  0.003076, -0.003034,  0.004632, 
    -0.002424,  0.006036, -0.004375,  0.006907, -0.004149,  0.006446, 
    -0.005684,  0.005229, -0.002195,  0.003366, -0.001150,  0.002120, 
    -0.005468,  0.001399, -0.006858,  0.001277, -0.008155,  0.002656, 
    -0.007389,  0.003861, -0.006449,  0.003597, -0.007443,  0.003552, 
    -0.005872,  0.004476, -0.005406,  0.001727, -0.002273,  0.001239, 
    -0.001131, -0.000035, -0.000383,  0.000735, -0.001772,  0.001074, 
    -0.000316, -0.000332, -0.000244,  0.000510,  0.000857, -0.000813, 
     0.002306, -0.001799,  0.001791, -0.003041,  0.001414,  0.000456, 
     0.000222,  0.000654, -0.000117,  0.001286, -0.000535,  0.000812, 
     0.000832,  0.000541,  0.001015, -0.002062,  0.002021, -0.003607, 
     0.002391, -0.003901,  0.001636, -0.002181, -0.000415, -0.000009, 
    -0.001380,  0.001080, -0.001001,  0.002247,  0.000157,  0.000398, 
     0.000729,  0.000715,  0.000331, -0.000550, -0.000303, -0.000226, 
     0.001433, -0.002536,  0.003281, -0.002742,  0.003052, -0.002602, 
     0.000775,  0.001682, -0.001782,  0.002897, -0.002197,  0.002564, 
    -0.001854,  0.002035,  0.001353, -0.000908,  0.002578, -0.001640, 
     0.004051, -0.003708,  0.004829, -0.003698,  0.005006, -0.003679, 
     0.004645, -0.002783,  0.003994, -0.001520,  0.003792, -0.001602, 
     0.003020, -0.001412,  0.003974, -0.002360,  0.003491, -0.003097, 
     0.005432, -0.002969,  0.005489, -0.001542,  0.004612, -0.000032, 
     0.002528,  0.002237,  0.000158,  0.003487, -0.001433,  0.004678, 
    -0.002473,  0.005487, -0.003248,  0.006239, -0.003514,  0.005194, 
    -0.002860,  0.005420, -0.002700,  0.004747, -0.003003,  0.005028, 
    -0.002970,  0.005215, -0.003600,  0.005973, -0.004319,  0.006377, 
    -0.003837,  0.004876, -0.001299,  0.003895,  0.001137,  0.000831, 
     0.003010, -0.001466,  0.003993, -0.003247,  0.004341, -0.002529, 
     0.004678, -0.002593,  0.004261, -0.001317,  0.003824, -0.001016, 
     0.002330,  0.000866,  0.002090,  0.001641,  0.001166,  0.001881, 
     0.000105,  0.002708,  0.000787,  0.002107,  0.001609,  0.000366, 
     0.001659,  0.001724,  0.000208,  0.002550, -0.001355,  0.004096, 
    -0.002703,  0.006199, -0.003374,  0.005959, -0.005133,  0.006852, 
    -0.003091,  0.006356, -0.003176,  0.005023,  0.000708, -0.000238, 
     0.001755, -0.002490,  0.005652, -0.002824,  0.004907, -0.002937, 
     0.003675, -0.002641,  0.001764, -0.000289,  0.000521,  0.001367, 
    -0.000805,  0.002453, -0.001817,  0.003065, -0.000283,  0.002092, 
    -0.000486,  0.002674,  0.000476,  0.002815,  0.000614,  0.004250, 
    -0.002612,  0.006363, -0.003495,  0.005580, -0.003429,  0.008358, 
    -0.006497,  0.009114, -0.008782,  0.007405, -0.007974,  0.006180, 
    -0.005819,  0.005378, -0.001898,  0.003414,  0.000971,  0.000783, 
     0.002331,  0.000010
];

data2 = [
    -0.000189,  0.000715,  0.001812, -0.000824,  0.003071, -0.002079, 
     0.003706, -0.001812,  0.003872, -0.002606,  0.004387, -0.002567, 
     0.004045, -0.002537,  0.004243, -0.001992,  0.002922, -0.000807, 
     0.002657, -0.000120,  0.001249,  0.000524,  0.001815,  0.000055, 
     0.002084, -0.000917,  0.002936, -0.000540,  0.001620,  0.000469, 
     0.000200,  0.002726, -0.002590,  0.004874, -0.001980,  0.009824, 
     0.000128,  0.012643,  0.006762,  0.014304,  0.002623, -0.016973, 
     0.012154,  0.030928, -0.013435,  0.026578,  0.077807,  0.028773, 
     0.009564,  0.018950,  0.007704, -0.024006,  0.002463, -0.006232, 
     0.005804, -0.010392, -0.006241,  0.000307,  0.001374,  0.001475, 
     0.010350,  0.009264,  0.008589,  0.005485,  0.005148,  0.016395, 
     0.011917,  0.003181, -0.004850,  0.003499, -0.006517,  0.003459, 
    -0.006264, -0.001742, -0.009215,  0.000556,  0.004310,  0.001187, 
    -0.008976, -0.002051, -0.005926, -0.007058, -0.008729, -0.003348, 
     0.000790, -0.010990, -0.000602, -0.000571, -0.004771, -0.005736, 
    -0.000771,  0.002579, -0.000931, -0.000677,  0.003272, -0.000344, 
     0.001553, -0.001767,  0.002292, -0.000328, -0.001676, -0.000304, 
    -0.004241, -0.001410, -0.004366,  0.001851, -0.003948,  0.003756, 
    -0.002030,  0.005996, -0.000305,  0.002765, -0.005001, -0.002755, 
    -0.007613, -0.006425, -0.005400, -0.005894, -0.000789, -0.004381, 
     0.001047, -0.008135,  0.003241,  0.000244,  0.005444,  0.000833, 
     0.006678, -0.000250,  0.006306,  0.001519,  0.003256, -0.001559, 
    -0.006782, -0.008985, -0.006470, -0.001903, -0.001244, -0.002387, 
    -0.006516,  0.000110,  0.001564,  0.000012, -0.004215, -0.002853, 
    -0.001775,  0.001602, -0.000098, -0.001978, -0.001299,  0.003134, 
    -0.004131, -0.000529, -0.004475,  0.001646, -0.003803,  0.002032, 
    -0.002654,  0.001471,  0.000668, -0.002201,  0.001390, -0.004674, 
     0.003044, -0.005187,  0.003633, -0.002881,  0.000261, -0.000307, 
    -0.002164,  0.001539, -0.002332,  0.002360, -0.001929, -0.000245, 
     0.001240, -0.001716,  0.004168, -0.004589,  0.006130, -0.007319, 
     0.007756, -0.006227,  0.006150, -0.005879,  0.004490, -0.004607, 
     0.003965, -0.002816,  0.002476, -0.002067,  0.002574, -0.001443, 
     0.002912, -0.002548,  0.003896, -0.004174,  0.004958, -0.004470, 
     0.005309, -0.003830,  0.003246, -0.000021, -0.000151,  0.000991, 
    -0.001268,  0.001666, -0.000656, -0.000181,  0.001784, -0.002624, 
     0.005418, -0.005864,  0.007160, -0.006259,  0.009187, -0.006433, 
     0.007811, -0.006206,  0.007339, -0.003999,  0.002998, -0.002299, 
     0.001092, -0.000313, -0.000869, -0.001150, -0.000298, -0.001926, 
     0.003068, -0.004190,  0.004604, -0.006212,  0.005975, -0.005545, 
     0.005274, -0.004991,  0.002979, -0.003504,  0.001712, -0.001779, 
    -0.000507,  0.000409, -0.000835,  0.000962, -0.000596,  0.001076, 
     0.000910,  0.002313,  0.001825,  0.001551,  0.001339,  0.000481, 
     0.001071,  0.001334,  0.000754,  0.001245, -0.001027,  0.001993, 
    -0.002590,  0.002178, -0.002359,  0.002313, -0.002619,  0.001515, 
    -0.001286,  0.000789, -0.000485,  0.000971, -0.000386,  0.002185, 
    -0.000754,  0.002176,  0.000302,  0.000346, -0.000467,  0.000024, 
    -0.000597,  0.000793, -0.002316,  0.002942, -0.003481,  0.003883, 
    -0.004267,  0.004193, -0.001867,  0.003096,  0.000604,  0.002498, 
     0.001879,  0.000134,  0.001103, -0.000387,  0.000354, -0.001985, 
     0.000028,  0.001551, -0.000087,  0.002567, -0.002000,  0.003147, 
    -0.002535,  0.003445, -0.002822,  0.003193, -0.002458,  0.003162, 
    -0.001836,  0.001725, -0.001676,  0.001188, -0.000389,  0.000348, 
     0.000677, -0.000192,  0.000943, -0.000192, -0.001406, -0.000570, 
    -0.002264,  0.002071, -0.002204,  0.000602, -0.003084, -0.001689, 
    -0.001620, -0.000346, -0.000560,  0.000199, -0.000425,  0.000229, 
    -0.001539, -0.000296, -0.000752,  0.000278,  0.000091, -0.001563, 
    -0.001390, -0.001322, -0.000776, -0.001006, -0.000575, -0.002256, 
     0.001436, -0.002446,  0.002247, -0.002535,  0.002777, -0.002486, 
     0.002959, -0.003181,  0.003497, -0.002994,  0.004370, -0.002840, 
     0.002926, -0.002445,  0.001929, -0.001425,  0.001319, -0.000998, 
     0.001033, -0.000770,  0.001364, -0.002037,  0.002645, -0.002400, 
     0.002718, -0.002709,  0.003207, -0.002664,  0.003546, -0.002839, 
     0.003607, -0.002140,  0.002293, -0.000204, -0.001228,  0.003810, 
    -0.002803,  0.005041, -0.002800,  0.003130, -0.002150,  0.000677, 
    -0.000488, -0.001138,  0.002364, -0.002398,  0.003633, -0.004140, 
     0.004926, -0.005028,  0.004546, -0.004512,  0.003845, -0.002430, 
     0.003645, -0.001746,  0.002535, -0.001191,  0.001037,  0.000797, 
     0.000693,  0.000674,  0.001396, -0.000133,  0.002662, -0.000100, 
     0.002104, -0.001333,  0.003747, -0.000984,  0.002698, -0.001142, 
     0.001981, -0.001788,  0.003494,  0.000756,  0.002881,  0.001243, 
     0.003017,  0.000244,  0.002689, -0.001627,  0.004369, -0.001800, 
     0.006625, -0.002837,  0.004470, -0.000859,  0.003200, -0.001234, 
     0.002140,  0.000023,  0.000651,  0.001171, -0.000079,  0.001368, 
    -0.001146,  0.001683, -0.000403,  0.001012,  0.001692,  0.001196, 
     0.002926, -0.000099,  0.003110, -0.002738,  0.002719, -0.001694, 
     0.003056, -0.000721,  0.002327,  0.000089,  0.001521, -0.000488, 
     0.002946,  0.000629,  0.003816, -0.001426,  0.003982, -0.000227, 
     0.000664,  0.001697, -0.000901,  0.004240, -0.002338,  0.004112, 
    -0.001701,  0.004662, -0.000688,  0.003788, -0.000132,  0.002335, 
     0.000426,  0.002432,  0.000823,  0.001291,  0.000754,  0.002009, 
    -0.000091,  0.002282, -0.001517,  0.001730, -0.001535,  0.001709, 
    -0.001472,  0.001384,  0.001563, -0.000319,  0.002418, -0.001489, 
     0.003322, -0.002023,  0.003038, -0.000676,  0.003328,  0.000080, 
     0.002080,  0.000700,  0.001002,  0.001397,  0.000088,  0.002208, 
     0.001015,  0.000624,  0.000602,  0.001440,  0.000323,  0.001003, 
    -0.000139,  0.001987
];

def hanning(x, max):
    return 0.50 + 0.50 * math.cos((x - max/2) * math.pi / (max + 1))

def hamming(x, max):
    return 0.54 + 0.46 * math.cos((x - max/2) * math.pi / max)

def blackman(x, max):
    return 0.42 + 0.50 * math.cos((x - max/2) * math.pi / max) + 0.08 * math.cos(2 * (x - max / 2) * math.pi / max)

def sinc(x):
    if x == 0:
        return 1;
    return math.sin(x * math.pi) / (x * math.pi)
        
def reversebitorder(i, fftlen):
    j = 0;
    k = 1;
    while k < fftlen:
        j <<= 1
        k <<= 1
        j |= i & 1
        i >>= 1
    return j

# This is the tersest FFT code I could come up with.
def do_fft(fft, sign):
    fftlen = len(fft)
    for i in range(1, fftlen-1):
        j = reversebitorder(i, fftlen)
        if i < j:
            fft[i], fft[j] = fft[j], fft[i]

    le = 1
    while le < fftlen:
        for j in range(0, le):
            angle = sign * math.pi * j / le
            u = math.cos(angle) + math.sin(angle) * 1j;
            for i in range(j, fftlen, le << 1):
                t = fft[i+le] * u
                fft[i+le] = fft[i] - t
                fft[i]    = fft[i] + t
        le <<= 1

def main():
    spectrum = []
    # sinc with stopband starting at fs/2
    for _ in range(512):
        spectrum += [data1[_]]

    # window the sinc, taking 14 coefficients
    # coeffs = 1024
    #for _ in range(0, 1024):
    #    if _ < coeffs:
    #        spectrum[_] *= hamming(_, coeffs)
    #    else:
    #        spectrum[_] = 0j

    #print "# FIR coefficients"
    #print "#"
    #for _ in range(0, 1024):
    #    if abs(spectrum[_]) > 1e-14:
    #        print "# %d %f" % (_, spectrum[_].real)
    #print

    # -1 is forward, 1 is inverse
    do_fft(spectrum, -1)

    for _ in range(1, len(spectrum)/2 + 1):
        mag = math.log(abs(spectrum[_])) / math.log(10) * 20
        phi = math.atan2(spectrum[_].imag, spectrum[_].real)
        print "%f %f %f" % (44100.0 * (_ - 0.5) / len(spectrum), mag, phi)

if __name__ == '__main__':
    main()
