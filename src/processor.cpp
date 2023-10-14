#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    float jnew, inew, anew, jold, aold, td, ad;
    jnew = LinuxParser::Jiffies();
    inew = LinuxParser::IdleJiffies();
    anew = LinuxParser::ActiveJiffies();

    jold = t_;
    aold = a_;
    

    td = jnew - jold;
    ad = anew - aold;

    Processor::Renew(jnew, inew, anew);

    return (ad / (td));
 }

 void Processor::Renew(float t, float i, float a) {
     Processor::t_ = t;
     Processor::i_ = i;
     Processor::a_ = a;
 }
 float Processor::t_;
 float Processor::i_;
 float Processor::a_;