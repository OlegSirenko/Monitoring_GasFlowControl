#ifndef PID_H
#define PID_H

#include <vector>

typedef enum 
{
  INCREASE_P = 0,
  RESAMPLE_P,
  DECREASE_2P,
  RESAMPLE_2P,
  PARAM_LAST
} PID_PARAM_S;

class PID {
public:
  
  // Errors
  double p_error;
  double i_error;
  double d_error;

  // Coefficients
  double Kp;
  double Ki;
  double Kd;

  // Constant Defines
  const double TOLERANCE = 0.01;
  const double Ki_WINDUP_LIMIT = 40.0;
  const int sample_interval = 50;

  // AutoTuneController variables
  double best_error = 999.9;
  int sample_index;
  std::vector<double> p;
  std::vector<double> dp;
  int tune_state;
  int next_state;
  int K_index;
  int next_index;

  // Constructor
  PID();

  // Destructor.
  virtual ~PID();

  // Initialize PID.
  void Init( double Kp, double Ki, double Kd );

  // Update the PID error variables given cross track error.
  void UpdateError( double cte );

  // Get the Steering Value (total PID error).
  double GetSteerValue();

  void AutoTuneController( double cte );
};

#endif /* PID_H */
