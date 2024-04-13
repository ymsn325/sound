#include <string>

#include "fft.hpp"

using namespace std;

class Sound {
 public:
  Sound(string fname);
  ~Sound();

 private:
  int m_fs;
  int m_nSamples;
  int m_nChannels;
  double m_duration;
  double *m_x;
  FFT *m_fft;
};