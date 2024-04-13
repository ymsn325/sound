#include "sound.hpp"

#include <climits>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

Sound::Sound(string fname) {
  ifstream fin;
  char tag[4];
  int buf4;
  short buf2;
  int blockSize;
  m_x = nullptr;
  cerr << "Read file: " << fname << endl;
  fin.open(fname, ios::in | ios::binary);
  if (!fin) {
    cerr << "Cannot open file: " << fname << endl;
    return;
  }
  fin.read(tag, 4);  // "RIFF"
  if (strncmp(tag, "RIFF", 4)) {
    cerr << "File is not RIFF format." << endl;
    return;
  }
  fin.read((char *)&buf4, 4);  // RIFFチャンクサイズ
  fin.read(tag, 4);            // "WAVE"
  if (strncmp(tag, "WAVE", 4)) {
    cerr << "File is not WAVE format." << endl;
    return;
  }
  fin.read(tag, 4);  // "fmt "
  if (strncmp(tag, "fmt ", 4)) {
    cerr << "fmt chunk is invalid." << endl;
    return;
  }
  fin.read((char *)&buf4, 4);  // fmtチャンクサイズ
  if (buf4 != 16) {
    cerr << "Unsupported format." << endl;
    return;
  }
  fin.read((char *)&buf2, 2);  // 音声フォーマット
  fin.read((char *)&buf2, 2);  // チャネル数
  m_nChannels = buf2;
  fin.read((char *)&buf4, 4);  // サンプルレート
  m_fs = buf4;
  cerr << m_fs << " Hz" << endl;
  fin.read((char *)&buf4, 4);  // bytes / sec
  fin.read((char *)&buf2, 2);  // ブロックサイズ
  blockSize = buf2;
  fin.read((char *)&buf2, 2);  // ビット深度
  fin.read(tag, 4);            // "data"
  if (strncmp(tag, "data", 4)) {
    cerr << "data chunk not found." << endl;
    return;
  }
  fin.read((char *)&buf4, 4);  // dataチャンクサイズ
  m_nSamples = buf4 / blockSize;
  m_duration = (double)m_nSamples / m_fs;
  cerr << m_duration << " sec" << endl;
  m_x = new double[m_nSamples];
  for (int n = 0; n < m_nSamples; n++) {
    fin.read((char *)&buf2, 2);
    m_x[n] = (double)(buf2 + (SHRT_MAX + 1.0) + 0.5) / (SHRT_MAX + 1.0) - 1.0;
  }
  fin.close();
  // m_fft = new FFT(16, Window::Gaussian, m_fs);
}

Sound::~Sound() {
  if (m_x != nullptr) {
    delete[] m_x;
  }
}

int main(int argc, char **argv) {
  string fname = "/home/ymsn325/audio/ichimoji_PF02_0501_033.wav";
  Sound sound(fname);
  int nFFT = 2048;
  double fs = 44100.0;
  FFT fft(nFFT, Window::Gaussian, fs);
  double x[nFFT];
  complex<double> X[nFFT];
  double f = 1000.0;
  for (int n = 0; n < nFFT; n++) {
    x[n] = sin(2.0 * M_PI * f * n / fs);
  }
  fft.exec(x, X);
  for (int k = 0; k < nFFT; k++) {
    cout << abs(X[k]) << endl;
  }
  return 0;
}