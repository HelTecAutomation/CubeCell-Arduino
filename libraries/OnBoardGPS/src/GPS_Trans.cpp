#include "Arduino.h"
#include "GPS_Trans.h"
HardwareSerial GPSSerial(UART_NUM_1);

double gps_x_pi = 3.14159265358979324 * 3000.0 / 180.0;
double gps_pi = 3.14159265358979324;
double gps_a = 6378245.0;
double gps_ee = 0.00669342162296594323;

uint32_t bauds[]={9600,19200,38400,57600,115200,230400,460800,921600};
uint32_t bauds_array = sizeof(bauds) / sizeof(bauds[0]);

double transformLon(double x, double y) {
	double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * gps_pi) + 20.0 * sin(2.0 * x * gps_pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(x * gps_pi) + 40.0 * sin(x / 3.0 * gps_pi)) * 2.0 / 3.0;
	ret += (150.0 * sin(x / 12.0 * gps_pi) + 300.0 * sin(x / 30.0 * gps_pi)) * 2.0 / 3.0;
	return ret;
};
double transformLat(double x, double y) {
	double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * gps_pi) + 20.0 * sin(2.0 * x * gps_pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(y * gps_pi) + 40.0 * sin(y / 3.0 * gps_pi)) * 2.0 / 3.0;
	ret += (160.0 * sin(y / 12.0 * gps_pi) + 320 * sin(y * gps_pi / 30.0)) * 2.0 / 3.0;
	return ret;
}

double outOfChina(double lat, double lon) {
	if (lon < 72.004 || lon > 137.8347)
		return true;
	if (lat < 0.8293 || lat > 55.8271)
		return true;
	return false;
}

int str_chop(char *s, int buff_size, char separator, uint8_t *idx_ary, int max_idx) {
    int i = 0; /* index in the string */
    int j = 0; /* index in the result array */

    if ((s == NULL) || (buff_size < 0) || (separator == 0) || (idx_ary == NULL) || (max_idx < 0)) {
        /* unsafe to do anything */
        return -1;
    }
    if ((buff_size == 0) || (max_idx == 0)) {
        /* nothing to do */
        return 0;
    }
    s[buff_size - 1] = 0; /* add string terminator at the end of the buffer, just to be sure */
    idx_ary[j] = 0;
    j += 1;
    /* loop until string terminator is reached */
    while (s[i] != 0) {
        if (s[i] == separator) {
            s[i] = 0; /* replace separator by string terminator */
            if (j >= max_idx) { /* no more room in the index array */
                return j;
            }
            idx_ary[j] = i+1; /* next token start after replaced separator */
            ++j;
        }
        ++i;
    }
    return j;
}


