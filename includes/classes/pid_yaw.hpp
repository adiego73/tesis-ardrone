#ifndef PIDYAW_H
#define PIDYAW_H

#include "classes/pid.hpp"

namespace tesis
{

class PIDYaw : public PID
{
    public:
        PIDYaw ( PIDValues constants, float derivator, float integrator, float max_integrator, float min_integrator, float max_pid_value );
        PIDYaw ( const PIDYaw &an_object );
        ~PIDYaw();
        float update ( float current_value );
        void setPoint ( float set_point );
};

};

#endif // PIDYAW_H
