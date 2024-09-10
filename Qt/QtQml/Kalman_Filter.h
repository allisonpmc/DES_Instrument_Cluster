// kalmanfilter.h
#ifndef KALAMANFILTER_H
#define KALAMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(double processNoiseQ, double measurementNoiseR, double initialEstimateErrorP, double initialSpeed) {
        X[0][0] = initialSpeed;
        X[1][0] = 0;

        P[0][0] = initialEstimateErrorP;
        P[1][1] = initialEstimateErrorP;
        P[0][1] = 0;
        P[1][0] = 0;

        Q[0][0] = processNoiseQ;
        Q[1][1] = processNoiseQ;
        Q[0][1] = 0;
        Q[1][0] = 0;

        R = measurementNoiseR;
    }

    double update(double measurement, double dt) {
        X[0][0] += X[1][0] * dt;

        P[0][0] += P[1][0] * dt + P[0][1] * dt + P[1][1] * dt * dt + Q[0][0];
        P[1][0] += P[1][1] * dt;
        P[0][1] += P[1][1] * dt;
        P[1][1] += Q[1][1];

        if(X[0][0] < 0){
            X[0][0] = 0;
        }

        double S = P[0][0] + R;
        double K[2];
        K[0] = P[0][0] / S;
        K[1] = P[1][0] / S;

        double y = measurement - X[0][0];
        X[0][0] += K[0] * y;
        X[1][0] += K[1] * y;

        double P00_temp = P[0][0];
        double P01_temp = P[0][1];

        P[0][0] -= K[0] * P00_temp;
        P[0][1] -= K[0] * P01_temp;
        P[1][0] -= K[1] * P00_temp;
        P[1][1] -= K[1] * P01_temp;

        if(X[0][0] < 0){
            X[0][0] = 0;
        }
        return X[0][0];
    }

private:
    double X[2][1];
    double P[2][2];
    double Q[2][2];
    double R;
};

#endif // KALMANFILTER_H
