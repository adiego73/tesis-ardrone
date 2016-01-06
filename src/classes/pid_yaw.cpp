#include "classes/pid_yaw.hpp"

using namespace tesis;

PIDYaw::PIDYaw ( PIDValues constants, float derivator, float integrator, float max_integrator, float min_integrator, float max_pid_value )
    : PID ( constants,  derivator,  integrator,  max_integrator,  min_integrator,  max_pid_value )
{

}

PIDYaw::PIDYaw ( const PIDYaw &an_object )
    : PID ( an_object.constants,  an_object.derivator,  an_object.integrator,  an_object.max_integrator,  an_object.min_integrator, an_object.max_pid_value )
{

}



PIDYaw::~PIDYaw()
{

}

float PIDYaw::update ( float current_value )
{
    float temp_error = this->set_point - current_value;

    if ( temp_error > MAX_ANGLE || temp_error < -MAX_ANGLE )
    {
        if ( current_value > 0 )
            current_value = -MAX_ANGLE - ( MAX_ANGLE - current_value );
        else
            current_value = MAX_ANGLE + ( MAX_ANGLE + current_value );
    }

    this->error = this->set_point - current_value;


    this->P_value = this->constants.Kp * this->error;

    if ( this->P_value > this->max_pid_value )
        this->P_value = this->max_pid_value;
    else if ( this->P_value < -this->max_pid_value )
        this->P_value = -this->max_pid_value;

    float changes = 0;

    float change = this->error - this->last_error;
    this->integrators.push_back ( change );

    const size_t COUNT = 3;

    if ( this->integrators.size() > COUNT )
        this->integrators.erase ( this->integrators.begin() );

    for ( size_t i = 0; i < this->integrators.size(); i++ )
        changes += this->integrators[i];

    changes = changes / this->integrators.size();

    //aumento Kd   si me alejo
    this->D_value = this->constants.Kd * changes;

    this->derivator = error;
    this->integrator += error;

    if ( this->integrator > this->max_integrator )
        this->integrator = this->max_integrator;
    else if ( this->integrator < this->min_integrator )
        this->integrator = this->min_integrator;

    this->I_value = this->constants.Ki * this->integrator;

    this->last_error = this->error;

    float total = this->P_value + this->I_value + this->D_value;

    if ( total > this->max_pid_value )
        total = this->max_pid_value;
    else if ( total < -this->max_pid_value )
        total = -this->max_pid_value;

    return total;
}

void PIDYaw::setPoint ( float set_point )
{
    this->set_point = set_point;
    this->integrator = .0f;
    this->derivator = .0f;
    this->integrators.clear();

    this->last_error = .0f;
    this->error = .0f;
}
