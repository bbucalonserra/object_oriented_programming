#include "Candlestick.h"


// Escope from Candlestick.h
Candlestick::Candlestick(std::string _date,
                        double _open,
                        double _high,
                        double _low,
                        double _close)

// Initialization list. It's basically to initialize with those variables. It get the data from above and store in the variables below (date, open, etc.).
: date(_date), 
  open(_open), 
  high(_high), 
  low(_low), 
  close(_close)

// Nothing goes inside the function because it will only keep data.
{

}