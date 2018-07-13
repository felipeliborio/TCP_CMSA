#pragma once

#include "MPrim.h"
#include "FInstance.h"
#include "CPLEX_TCP.h"

FInstance CMSA(FInstance &Instance, int PrimExecutions, int AgeMax, float const Alpha);
void SimplifyInstance(FInstance & Instance);