#include "classes/pid.hpp"

using namespace tesis;

PID::PID ( PIDValues constants, float derivator, float integrator, float max_integrator, float min_integrator, float max_pid_value )
{
    this->constants = constants;
    this->derivator = derivator;                        //  this is not used
    this->integrator = integrator;
    this->max_integrator = max_integrator;
    this->min_integrator = min_integrator;
    this->max_pid_value = max_pid_value;                //  before was P_limit

    this->P_value = 0.0f;
    this->D_value = 0.0f;
    this->I_value = 0.0f;

    this->set_point = 0.0f;

    this->integrators = std::vector<float> ( 2 );

    this->last_error = 0.0f;
    this->error = 0.0f;

}

PID::PID ( PID &an_object )
  : constants(an_object.constants), 
  derivator(an_object.derivator), 
  integrator(an_object.integrator), 
  max_integrator(an_object.max_integrator), 
  min_integrator(an_object.min_integrator), 
  max_pid_value(an_object.max_pid_value)
{
    
}


PID::~PID()
{

}

void PID::reset()
{
    this->derivator = 0.0f;
    this->integrator = 0.0f;
}

void PID::setPoint ( float set_point )
{
    this->set_point = 0.0f;
    this->integrator = 0.0f;
    this->derivator = 0.0f;
    this->integrators.clear();

    this->last_error = set_point;
    this->error = 0.0f;
}

float PID::update ( float current_value, float d_change, long dt )
{
    this->error = current_value;
    float change = this->error - this->last_error;          //si es positivo me alejo, si es negativo me acerco

    this->P_value = this->constants.Kp * this->error;

    float changes = 0;
    this->integrators.push_back ( change / dt );

    const size_t COUNT = 2;

    if ( this->integrators.size() > COUNT )
        this->integrators.erase ( this->integrators.begin() );

    for ( size_t i = 0; i < this->integrators.size(); i++ )
        changes += this->integrators[i];

    changes = changes / this->integrators.size();

    this->D_value = this->constants.Kd * d_change;          //usp el del robot

    this->integratorIncrease ( this->error * dt, this->max_integrator, changes );

    this->I_value = this->integrator * this->constants.Ki;

    this->last_error = this->error;


    float total = this->P_value + this->I_value + this->D_value;

    if ( total > this->max_pid_value )
        total = this->max_pid_value;
    else if ( total < -this->max_pid_value )
        total = -this->max_pid_value;

    return total;
}

void PID::integratorIncrease ( float error, float change, float max_value )
{
    if ( error < 0 && this->integrator > 0 )
    {
        this->integrator = std::max ( 0.0, this->integrator + 14.0 * error );
    }
    else if ( error > 0 && this->integrator < 0 )
    {
        this->integrator = std::min ( 0.0, this->integrator + 14.0 * error );
    }
    else if ( change < 0 && this->integrator > 0 && error > 0 ) //si me estoy acercando
    {
        this->integrator = std::max ( 0.0f, float ( this->integrator - error ) );
    }
    else if ( change > 0 && this->integrator < 0 && error < 0 ) //si me estoy acercando
    {
        this->integrator = std::min ( 0.0f, float ( this->integrator - error ) );
    }
    else
    {
        this->integrator += error;
    }

    if ( this->integrator > max_value )
    {
        this->integrator  = max_value;
    }
    else if ( this->integrator < -max_value )
    {
        this->integrator  = -max_value;
    }
}

