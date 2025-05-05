#ifndef HARMONICPARAMS_H
#define HARMONICPARAMS_H

class HarmonicParams {
public:
    static const std::vector<std::tuple<double, double, double>> harmonicParams;
};

#endif // HARMONICPARAMS_H

/*
Параметры гармоник
амплитуда, частота, начальная фаза
*/
const std::vector<std::tuple<double, double, double>> HarmonicParams::harmonicParams = {
    {2.0, 50.0,  M_PI * 0},
    {1.5, 100.0, M_PI / 4},
    {1.0, 200.0, M_PI / 2},
    {0.8, 300.0, M_PI * 3 / 4},
    {0.6, 400.0, M_PI},
    {0.5, 500.0, M_PI / 6},
    {0.4, 600.0, M_PI / 3},
    {0.3, 700.0, M_PI * 2 / 3},
    {0.2, 800.0, M_PI * 5 / 6}, 
    {0.1, 900.0, M_PI / 8}
};
