#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void Renew(float t, float i, float a);

  static float i_;
  static float t_;
  static float a_;

  // TODO: Declare any necessary private members
 private:
};

#endif