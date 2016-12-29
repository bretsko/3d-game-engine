#pragma once

#include "Color.h"

struct material {
  Color amb;
  Color diff;
  Color emiss;
  Color spec;
  double pow;
};

