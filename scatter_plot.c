#include <stdio.h>
#include <math.h>

/******************************************************************************
 * This program takes an initial estimate of m and c and finds the associated 
 * rms error. It is then as a base to generate and evaluate 8 new estimates, 
 * which are steps in different directions in m-c space. The best estimate is 
 * then used as the base for another iteration of "generate and evaluate". This 
 * continues until none of the new estimates are better than the base. This is
 * a gradient search for a minimum in mc-space.
 * 
 * To compile:
 *   cc -o scatter_plot scatter_plot.c -lm
 * 
 * To run:
 *   ./scatter_plot
 * 
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/
typedef struct point_t {
  double x;
  double y;
} point_t;

int n_data = 1000;
point_t data[];


void print_data() {
  int i;
  for(i=0; i<n_data; i++) {
    printf("%0.2lf,%0.2lf\n", data[i].x, data[i].y);
  }
}
int main() {
  printf("X-axis,y_axis\n");
  print_data();
  return 0;
}
point_t data[] = {
  {65.77,103.17},{66.66,115.07},{68.04,113.77},{73.02,110.29},
  {82.45,135.91},{73.76,103.04},{90.76,148.34},{65.33,97.44},
  {75.33,112.93},{72.32,108.50},{73.09,114.08},{82.21,133.12},
  {83.75,131.80},{73.91,106.80},{60.25,88.80},{83.96,153.61},
  {92.69,144.22},{32.54,68.18},{71.86,115.82},{56.19,103.37},
  {78.68,127.29},{ 0.35,31.54},{28.09,85.61},{ 3.09,20.34},
  {18.38,83.70},{29.84,77.58},{76.85,118.94},{63.64,92.79},
  {61.50,106.92},{85.08,140.96},{27.72,59.21},{30.82,63.24},
  {18.91,69.26},{12.29,47.23},{11.13,47.68},{89.13,145.64},
  {14.92,42.34},{97.39,152.14},{61.64,134.55},{78.64,125.56},
  {12.30,37.91},{ 1.63,23.71},{84.73,137.51},{71.00,107.53},
  {41.16,73.61},{30.61,79.11},{66.18,106.89},{ 0.41,16.82},
  {45.86,69.48},{27.65,65.35},{72.68,137.84},{14.27,37.45},
  {44.51,84.86},{46.35,76.14},{37.69,88.49},{49.26,74.66},
  { 8.18,53.70},{99.85,163.82},{26.59,54.90},{51.94,109.76},
  {70.89,118.08},{ 3.18,48.75},{ 7.80,49.29},{83.02,114.45},
  {92.82,157.59},{24.80,69.54},{38.36,73.69},{93.60,154.66},
  {70.53,122.81},{37.67,61.82},{58.06,102.25},{ 3.65,41.44},
  {52.43,96.34},{65.23,121.17},{57.84,102.28},{44.35,64.02},
  {85.82,132.38},{50.53,87.74},{88.17,144.48},{97.23,146.11},
  {55.53,99.61},{35.08,98.07},{89.75,140.88},{66.73,108.88},
  {38.06,80.05},{26.92,37.08},{45.46,94.12},{81.64,134.65},
  {75.28,106.90},{16.76,58.23},{59.42,97.67},{20.75,71.57},
  {34.08,75.69},{36.98,70.84},{67.55,110.33},{76.72,137.76},
  {44.57,83.73},{30.68,69.29},{49.51,66.07},{19.17,48.65},
  {61.23,108.20},{ 8.92,54.31},{42.48,95.04},{80.79,140.36},
  {64.87,112.87},{ 8.13,37.56},{59.77,97.27},{16.26,65.89},
  {38.06,83.18},{71.88,116.56},{16.34,54.18},{17.30,60.03},
  {42.92,90.89},{70.44,129.42},{50.64,73.67},{72.85,124.06},
  {100.00,154.50},{23.62,63.11},{45.41,85.47},{73.11,127.97},
  {97.32,148.97},{85.22,132.78},{99.01,140.74},{61.07,102.76},
  {12.47,49.19},{43.05,90.06},{33.18,77.98},{47.45,73.35},
  {98.70,131.60},{ 7.81,52.51},{89.99,135.38},{74.26,120.60},
  {62.79,98.80},{67.59,102.28},{46.73,102.80},{28.14,61.02},
  {83.41,126.10},{48.71,96.02},{69.36,125.89},{89.15,154.47},
  {45.02,78.14},{57.09,95.26},{ 4.13,36.98},{74.37,111.26},
  {84.96,148.19},{42.52,95.53},{53.26,99.68},{55.32,92.34},
  {27.31,74.02},{93.08,139.09},{82.76,113.47},{31.92,83.12},
  {15.06,60.05},{71.29,125.77},{51.73,102.28},{19.49,63.28},
  {18.44,59.18},{38.28,67.34},{96.08,150.36},{ 4.14,35.67},
  {65.91,110.67},{ 1.10,23.11},{42.66,92.22},{76.20,118.66},
  {90.15,122.88},{17.56,33.36},{ 8.27,36.60},{70.83,114.31},
  {59.74,122.20},{77.04,141.71},{85.19,142.04},{49.98,106.00},
  {11.94,51.96},{19.02,54.53},{45.42,85.57},{85.87,133.83},
  {75.84,85.03},{66.20,129.78},{60.95,106.32},{42.68,78.93},
  {15.55,36.11},{80.36,123.79},{15.48,47.20},{26.98,62.69},
  {13.26,53.55},{97.72,150.14},{42.85,86.35},{61.01,110.77},
  {10.24,41.83},{55.05,99.23},{47.51,92.32},{90.34,142.59},
  {83.50,130.13},{21.99,62.66},{ 9.66,21.35},{83.86,150.40},
  {74.14,115.92},{65.81,107.08},{39.73,79.82},{34.41,74.53},
  { 0.16,16.92},{92.50,127.79},{22.29,68.43},{79.27,127.22},
  { 8.54,40.84},{71.21,114.90},{49.00,64.75},{ 1.05,34.94},
  {46.59,93.15},{80.89,133.51},{99.58,156.24},{53.00,97.62},
  {83.79,129.39},{ 3.77,23.48},{76.98,125.41},{87.12,131.66},
  {32.34,74.83},{51.69,112.98},{53.35,117.18},{92.45,151.77},
  {84.49,130.86},{84.05,120.72},{71.63,122.69},{47.11,86.93},
  {37.29,63.74},{74.57,125.43},{76.22,127.12},{38.23,63.11},
  {88.49,153.64},{66.60,122.56},{78.47,119.13},{65.91,107.44},
  { 9.25,25.09},{39.86,80.09},{47.55,94.87},{98.61,151.74},
  { 2.69,29.18},{26.41,54.13},{86.16,115.41},{29.62,56.51},
  {76.24,104.44},{70.46,125.62},{28.43,66.37},{14.35,48.26},
  {16.82,59.15},{80.56,121.00},{65.33,107.35},{14.10,60.46},
  {92.67,149.12},{99.60,156.95},{26.30,57.33},{65.01,100.52},
  {74.16,113.75},{35.78,75.41},{40.38,70.56},{77.36,116.87},
  {10.85,32.42},{18.97,54.56},{82.43,135.12},{95.14,151.90},
  {46.39,85.91},{90.92,114.09},{ 5.48,38.41},{74.40,121.21},
  {70.05,111.64},{24.52,51.25},{10.75,41.62},{68.75,122.83},
  {87.86,130.06},{85.31,133.71},{53.60,117.89},{ 4.22,33.62},
  {44.05,85.89},{25.84,57.66},{74.65,121.53},{94.83,148.35},
  {99.49,157.77},{86.18,125.42},{44.00,94.04},{83.42,131.71},
  {43.19,73.85},{69.31,111.56},{64.80,116.99},{ 1.40,15.37},
  {69.99,126.03},{71.02,101.94},{16.05,35.13},{22.73,60.64},
  {84.12,137.98},{97.45,147.54},{86.69,128.34},{71.59,109.89},
  {43.47,73.00},{30.52,76.86},{78.60,126.19},{ 9.69,37.95},
  {65.41,105.16},{ 2.76,42.98},{39.41,77.67},{55.34,89.06},
  {83.51,132.52},{39.28,67.13},{36.30,59.54},{18.15,55.98},
  {58.92,126.54},{75.95,126.29},{95.26,166.01},{97.87,132.00},
  {60.32,103.19},{21.41,62.93},{ 9.25,61.15},{78.05,125.47},
  {28.77,83.14},{81.59,140.53},{90.25,138.17},{56.87,93.23},
  {63.07,111.84},{33.74,65.37},{52.69,106.62},{94.46,133.90},
  {17.98,43.25},{ 6.34,54.08},{19.22,47.62},{42.61,82.96},
  {71.03,128.26},{ 4.48,39.77},{97.45,154.07},{57.89,89.07},
  { 7.30,34.80},{95.20,145.28},{16.32,59.02},{19.14,49.39},
  {73.62,108.80},{70.30,89.41},{57.92,116.39},{ 4.81,36.00},
  {98.09,136.32},{64.91,112.36},{63.37,109.41},{20.60,73.03},
  {74.33,104.33},{44.04,68.55},{ 5.44,44.34},{30.21,70.08},
  { 6.75,43.07},{34.15,82.16},{75.01,111.84},{45.88,96.30},
  {37.32,83.45},{48.08,88.76},{ 6.04,25.92},{88.36,163.22},
  {67.05,115.71},{29.27,57.17},{13.76,47.86},{45.10,91.57},
  {54.08,100.19},{59.82,91.03},{42.94,91.46},{ 5.54,47.67},
  {36.43,95.08},{71.10,122.78},{57.26,102.31},{20.03,53.70},
  {80.26,142.83},{56.95,115.65},{83.29,132.26},{ 7.29,41.92},
  {52.40,99.03},{10.83,40.13},{72.84,133.72},{ 5.75,45.32},
  {63.33,116.96},{10.21,29.81},{29.53,58.29},{26.63,52.33},
  {21.40,42.72},{41.13,103.43},{75.96,122.99},{82.66,122.12},
  {68.22,114.37},{82.13,123.21},{82.87,145.94},{27.83,59.69},
  {81.36,142.95},{40.40,65.98},{74.99,140.34},{39.78,71.88},
  {24.15,58.28},{26.33,43.97},{89.59,141.88},{19.37,45.91},
  {66.67,110.53},{13.73,39.48},{33.23,79.42},{11.64,51.42},
  {35.64,56.43},{98.65,148.80},{43.14,88.28},{90.56,138.01},
  {71.74,114.79},{ 2.56,40.33},{24.94,55.18},{ 2.53,36.32},
  {87.63,144.73},{60.09,96.77},{66.32,107.28},{ 8.84,38.99},
  {49.49,86.62},{ 7.20,42.52},{59.06,64.49},{94.03,164.06},
  {28.25,71.25},{11.01,64.51},{40.75,75.34},{10.11,45.78},
  {87.18,134.04},{78.23,121.22},{82.65,134.80},{ 3.01,46.72},
  {98.94,152.06},{ 7.24,43.79},{39.01,84.74},{75.28,125.76},
  {60.43,93.21},{ 2.17,31.11},{ 1.70,27.34},{79.50,122.85},
  {29.45,66.44},{97.41,143.46},{45.40,99.21},{29.56,68.15},
  {72.21,128.50},{95.95,139.59},{94.42,157.34},{63.19,98.08},
  {11.72,41.20},{20.63,59.08},{81.00,130.44},{18.63,52.35},
  {43.88,72.93},{20.05,61.43},{85.75,144.94},{18.59,43.76},
  {46.77,96.89},{59.97,98.22},{26.51,64.44},{65.33,115.61},
  {14.10,55.66},{91.72,142.97},{30.37,74.01},{71.64,123.92},
  {29.39,56.18},{59.86,96.16},{12.70,62.20},{25.02,62.19},
  {59.02,108.29},{ 7.84,50.88},{21.26,63.46},{53.63,106.03},
  {14.68,37.16},{80.88,140.53},{ 8.41,55.50},{60.80,109.26},
  {23.24,59.87},{91.98,149.68},{ 9.04,38.33},{46.28,92.27},
  {40.68,81.85},{36.51,63.35},{20.70,57.45},{14.41,41.92},
  {50.88,112.55},{47.65,85.11},{52.26,84.29},{64.40,108.21},
  {59.21,92.34},{20.78,57.55},{91.44,139.58},{59.40,100.03},
  {59.72,111.63},{50.07,88.22},{64.12,94.18},{ 5.54,60.74},
  {30.26,52.57},{58.99,100.15},{ 7.07,31.74},{94.93,151.52},
  {71.85,113.02},{87.67,123.00},{ 5.55,49.48},{79.29,118.08},
  {20.34,48.62},{ 0.48,24.98},{36.22,91.53},{17.20,53.74},
  {29.60,61.04},{56.08,107.97},{54.47,103.19},{69.35,115.07},
  { 4.76,32.31},{45.07,97.83},{14.53,66.36},{21.65,54.46},
  {38.73,83.16},{80.96,118.73},{57.11,102.90},{36.82,78.69},
  {47.69,68.78},{ 0.05,29.29},{33.27,85.91},{38.91,96.94},
  {31.80,72.19},{91.87,150.57},{23.74,63.45},{63.51,126.96},
  {97.61,163.28},{32.15,84.64},{84.90,128.78},{95.62,140.15},
  { 7.99,35.24},{70.87,104.87},{86.19,130.50},{20.84,49.97},
  {24.74,61.46},{91.05,144.13},{ 0.98,25.98},{32.50,66.16},
  {14.44,57.60},{22.42,66.02},{91.38,142.46},{19.84,57.17},
  {33.62,67.50},{58.57,103.35},{25.13,72.83},{17.31,34.97},
  {76.99,110.70},{71.86,128.58},{17.29,63.02},{94.08,129.27},
  {50.43,90.01},{23.63,61.34},{67.11,110.97},{14.88,44.54},
  {37.76,72.45},{92.88,140.30},{78.57,127.46},{68.67,107.11},
  {25.33,53.83},{31.06,67.79},{ 6.69,27.38},{12.30,50.66},
  {26.00,67.45},{25.89,69.85},{22.50,61.54},{72.63,118.25},
  {76.90,136.66},{24.37,71.36},{47.29,87.92},{30.76,82.17},
  {70.83,119.63},{37.67,67.23},{ 2.92,27.50},{71.40,112.23},
  {15.30,43.37},{52.29,103.00},{63.47,111.07},{12.99,59.26},
  {60.71,97.46},{90.70,164.44},{25.41,67.64},{47.78,94.72},
  {41.06,79.19},{59.46,99.82},{37.09,89.59},{19.52,47.87},
  {24.92,70.44},{49.14,88.71},{53.69,101.00},{97.36,165.67},
  {88.73,151.39},{43.66,67.63},{22.57,47.01},{77.66,124.39},
  {90.58,123.38},{32.42,75.76},{26.47,64.65},{97.98,148.35},
  {74.32,124.99},{45.54,87.44},{60.62,86.97},{36.59,74.95},
  { 2.65,39.52},{85.56,124.04},{16.05,47.77},{96.80,128.91},
  {30.03,69.82},{57.59,89.24},{98.12,146.14},{62.42,102.56},
  {17.52,50.54},{40.80,72.44},{18.65,48.68},{34.59,73.93},
  {93.03,146.90},{22.68,70.39},{47.00,86.77},{49.78,116.04},
  {40.08,75.36},{22.91,48.48},{71.74,98.57},{78.77,121.33},
  {42.69,80.48},{59.05,113.31},{42.85,94.04},{56.53,125.57},
  {81.31,136.85},{13.86,43.15},{22.44,55.97},{ 1.24,25.86},
  {89.18,141.93},{83.07,127.94},{32.33,75.70},{41.94,99.04},
  {71.08,115.94},{32.78,70.54},{87.40,142.53},{61.36,96.57},
  {54.66,87.78},{53.19,106.08},{53.54,100.35},{27.55,69.39},
  {50.02,95.74},{69.10,123.43},{87.24,135.44},{61.57,106.46},
  {45.19,79.09},{34.40,75.29},{88.91,124.80},{75.33,139.61},
  {72.72,112.59},{24.58,64.69},{35.28,55.38},{35.88,79.24},
  { 0.47,16.66},{70.32,114.69},{53.21,110.96},{ 6.04,52.43},
  {35.03,76.40},{29.59,71.85},{47.27,90.89},{11.21,59.47},
  {17.03,70.47},{85.69,131.92},{12.54,65.73},{29.02,85.10},
  {37.88,68.83},{33.51,77.61},{37.69,73.04},{42.90,71.74},
  { 2.98,23.38},{36.21,77.22},{21.57,72.38},{ 9.36,53.03},
  {80.31,136.00},{12.10,43.60},{95.79,144.53},{96.39,158.95},
  {15.98,28.75},{ 8.48,40.43},{74.62,131.88},{85.68,132.76},
  {85.15,143.59},{57.94,105.64},{26.56,76.48},{15.12,56.58},
  {97.62,136.54},{56.90,105.77},{57.28,107.05},{70.58,131.09},
  {24.56,77.07},{45.24,84.88},{86.06,139.75},{80.19,135.38},
  { 4.40,46.01},{ 0.80,37.36},{18.67,32.66},{49.93,100.09},
  {67.17,108.56},{78.10,141.56},{71.44,112.18},{58.19,110.45},
  { 2.13,36.72},{82.52,105.53},{96.94,158.48},{47.74,94.69},
  {72.48,119.25},{52.67,83.24},{35.07,66.27},{82.09,118.09},
  {65.44,126.96},{66.96,121.99},{ 7.94,30.47},{ 8.51,42.20},
  {71.37,101.37},{67.76,100.37},{26.35,76.18},{ 7.28,40.36},
  {90.59,157.93},{ 7.39,50.31},{56.78,102.18},{34.68,61.38},
  {41.17,73.70},{53.35,100.09},{73.75,126.89},{96.48,148.31},
  {73.33,129.61},{78.59,126.46},{58.30,102.73},{20.20,40.65},
  {46.85,104.85},{ 9.04,42.25},{99.42,159.59},{93.21,125.42},
  {29.58,54.15},{29.32,60.87},{15.22,37.74},{35.23,68.65},
  {44.57,90.12},{62.59,117.43},{21.16,77.27},{21.75,62.81},
  {97.07,172.71},{48.24,104.32},{73.52,117.27},{59.40,90.80},
  {55.95,123.10},{ 6.78,31.26},{50.66,99.87},{ 7.12,43.71},
  {85.29,137.75},{22.16,60.48},{98.65,124.55},{15.33,44.09},
  {90.07,141.91},{66.01,111.34},{59.90,101.65},{38.76,86.38},
  {14.03,57.19},{96.81,141.87},{ 8.88,42.66},{80.24,116.28},
  {57.50,90.11},{75.21,144.04},{99.00,150.86},{ 8.77,57.59},
  {84.30,128.82},{61.42,109.16},{15.52,46.98},{36.42,95.48},
  {73.77,135.33},{33.09,72.22},{87.52,144.03},{50.41,105.91},
  {79.08,119.14},{44.87,104.66},{82.90,128.20},{45.96,110.94},
  {96.42,134.11},{45.65,87.51},{55.77,111.11},{39.74,67.97},
  {12.29,41.79},{49.43,70.04},{99.59,168.88},{69.91,95.39},
  {24.39,76.42},{82.74,127.00},{50.11,80.19},{ 5.93,39.53},
  {27.35,74.87},{ 9.68,38.49},{71.77,113.22},{91.43,136.23},
  {70.68,126.01},{45.76,89.37},{ 9.24,19.89},{12.96,44.06},
  {77.94,120.32},{23.29,56.16},{46.94,78.21},{87.95,125.36},
  {35.77,68.55},{90.25,130.78},{77.52,132.04},{68.83,120.34},
  {54.98,93.74},{87.06,121.11},{58.35,110.51},{65.50,114.36},
  {36.78,98.07},{48.58,85.19},{46.76,109.92},{ 2.44,44.53},
  {33.74,80.06},{ 0.05,39.32},{39.16,79.28},{42.61,87.78},
  {48.08,93.16},{91.66,136.57},{29.92,70.97},{36.89,77.40},
  {56.86,99.42},{ 1.75,44.22},{20.50,57.55},{61.32,113.99},
  {69.97,112.34},{15.51,60.90},{80.22,128.70},{85.77,139.29},
  {10.82,43.40},{ 4.92,39.18},{58.21,111.46},{ 5.81,45.55},
  {85.88,154.43},{93.93,136.64},{66.94,123.28},{53.59,77.42},
  { 8.99,42.55},{23.88,67.91},{ 6.69,13.44},{37.62,91.90},
  {29.27,61.26},{ 6.29,35.74},{46.27,84.48},{14.91,50.12},
  {29.06,62.32},{70.16,100.79},{26.91,55.01},{75.22,127.27},
  {95.04,158.75},{24.10,69.52},{80.01,109.62},{24.25,56.81},
  {14.83,56.47},{75.28,126.37},{30.86,73.36},{94.36,144.53},
  {61.19,109.52},{91.16,141.24},{74.87,103.78},{ 6.31,54.19},
  {11.19,58.23},{12.03,60.78},{ 8.44,41.31},{56.11,89.24},
  {21.02,53.45},{38.93,74.13},{93.64,145.20},{94.31,137.23},
  {44.12,80.44},{ 7.10,46.16},{53.60,76.43},{34.92,66.94},
  {52.58,110.90},{ 3.05,41.00},{64.02,96.84},{14.26,52.94},
  {17.38,55.65},{71.95,129.09},{62.67,116.49},{60.56,98.66},
  {50.05,82.96},{87.39,140.29},{69.83,104.48},{39.62,68.16},
  {24.56,71.93},{33.32,81.02},{87.68,136.71},{79.02,120.89},
  {43.41,94.81},{97.98,152.76},{ 8.22,54.90},{28.60,50.79},
  {39.47,91.32},{84.90,141.70},{39.51,86.98},{56.69,90.42},
  {91.25,138.19},{52.51,95.03},{88.84,138.39},{49.43,93.68},
  {68.29,112.72},{83.78,127.27},{74.39,113.84},{33.33,76.97},
  {79.09,115.54},{ 1.71,20.25},{96.94,127.17},{45.37,90.65},
  {47.12,91.24},{63.30,107.76},{65.19,106.23},{27.84,68.58},
  {71.13,120.42},{15.18,46.12},{30.56,79.90},{21.69,61.65},
  {76.01,127.67},{94.67,135.84},{58.92,110.17},{42.06,99.29},
  {35.78,76.41},{13.09,36.91},{79.82,129.03},{13.47,38.05},
  { 0.47,44.26},{42.94,95.60},{ 5.91,33.28},{14.51,41.14},
  {21.93,53.87},{87.89,132.77},{47.35,75.25},{57.42,112.81},
  {90.13,133.24},{12.77,46.29},{47.37,100.83},{ 4.88,30.32},
  {21.27,63.80},{78.44,126.29},{95.77,142.11},{40.88,83.83},
  {74.79,128.38},{61.52,118.65},{67.51,117.23},{78.36,130.30},
  {63.96,98.76},{96.38,153.44},{47.88,70.40},{14.40,43.37},
  { 8.52,43.81},{ 6.44,26.98},{18.50,35.08},{74.78,122.99},
  {67.55,123.19},{37.45,71.36},{ 3.85,18.35},{38.84,73.52},
  {29.51,61.93},{38.63,84.42},{77.63,128.96},{75.18,128.67},
  {71.28,120.48},{ 5.83,61.37},{27.02,79.54},{79.03,120.53},
  {79.22,124.19},{58.20,104.06},{38.59,98.95},{42.16,82.21},
  {83.70,123.90},{36.89,75.18},{47.77,94.42},{80.30,118.38},
  { 0.42,34.00},{74.55,130.64},{ 9.88,31.50},{64.72,123.00},
  {39.60,75.94},{68.76,112.79},{72.69,114.21},{58.20,90.93},
  {94.21,128.80},{69.74,119.12},{99.48,150.63},{63.85,119.08},
  {54.56,99.61},{97.73,142.85},{62.48,85.27},{57.76,111.23},
  {55.21,107.38},{89.40,132.81},{66.27,141.28},{25.08,56.02},
  {67.03,108.34},{83.70,128.19},{56.98,95.40},{87.17,148.70},
  {21.31,51.59},{ 7.56,42.71},{50.45,81.20},{50.50,100.72},
  {98.22,151.25},{49.49,76.59},{39.03,84.60},{90.03,140.25},
  {97.86,162.06},{80.83,129.97},{88.18,129.07},{64.29,95.35}
};
