#ifndef PID_H
#define PID_H

#include "structures.hpp"

namespace tesis
{

class PID
{
    public:
        PID ( PIDValues constants, float derivator, float integrator, float max_integrator, float min_integrator, float max_pid_value );
        PID ( PID &an_object );
        virtual float update ( float current_value, float change, long dt );
        virtual void setPoint ( float set_point );
        virtual void reset();
        ~PID();

    protected:
        PIDValues constants;
        float derivator;
        float integrator;
        float max_integrator;
        float min_integrator;
        float max_pid_value;

        float P_value;
        float D_value;
        float I_value;

        float set_point;

        std::vector<float> integrators;

        float last_error;
        float error;

        void integratorIncrease ( float error, float change, float max_value );
};



};
#endif // PID_H
