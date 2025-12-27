#include "OrderBookEntry.h"

/** * Constructor definition: This instructs the compiler that to instantiate an OrderBookEntry, 
 * five specific parameters must be provided. 
 * * Note: The repetition of parameter types (e.g., double _price) between the header (.h) 
 * and implementation (.cpp) is necessary because the C++ compiler treats the 
 * declaration (the 'signature') and the definition as independent instructions.
 */
OrderBookEntry::OrderBookEntry( double _price,
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType,
                        std::string _username)

/** * Initialisation List: We utilise the colon (:) operator here to map parameters to 
 * the internal member variables (the permanent memory allocated to the class).
 * * Performance Note: In datasets containing millions of entries, using an initialisation 
 * list is more efficient. It ensures variables are created with the correct values 
 * immediately, rather than being default-initialised and subsequently reassigned 
 * within the function body.
 */
: price(_price), 
  amount(_amount), 
  timestamp(_timestamp),
  product(_product), 
  orderType(_orderType),
  username(_username)

/** * Function Body: This block executes logic upon instantiation. Since the 
 * initialisation list handles all variable assignments, the body remains empty.
 */
{
    
}

/** * Static Helper: Converts a string literal into a structured OrderBookType enum.
 * * The structure follows: <return_type> <scope>::<function_name>(<parameters>).
 * Note: As this is a 'static' method, it belongs to the class itself rather than 
 * a specific object instance, allowing it to be called without instantiating the class.
 */
OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  if (s == "ask")
  {
    return OrderBookType::ask;
  }
  if (s == "bid")
  {
    return OrderBookType::bid;
  }
  return OrderBookType::unknown;
}