class PID{
    public:
    double Kp;
    double Ki;
    double Kd;
    double upperbound;
    double lowerbound;
    double MaxSpeed;
    double MaxPWM;
    PID();

    void setPID(double P, double I, double D);
    void setboundary(double upper, double lower);
    void setMaxSpeedPWM(double MaxSpeed);
    double calPID(double targetValue, double currentValue, long dT);
    int readKp();
    
};