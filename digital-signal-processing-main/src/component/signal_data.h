#ifndef SIGNALDATA_H
#define SIGNALDATA_H

struct SignalData {
    std::vector<double> time;
    std::vector<double> frequency;

    void addData(double t, double f) {
        time.push_back(t);
        frequency.push_back(f);
    }
};

#endif // SIGNALDATA_H
