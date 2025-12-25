#pragma once
#include <string>


class Candlestick
{
    public:
        Candlestick(
            std::string _date,
            double _open,
            double _high,
            double _low,
            double _close
        );

        // Variables that will store the data
        std::string date;
        double open;
        double high;
        double low;
        double close;
    
    private:
};