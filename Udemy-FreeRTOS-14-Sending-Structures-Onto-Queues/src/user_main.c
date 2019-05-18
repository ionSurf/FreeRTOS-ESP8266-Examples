//  Add libraries
//  Add profilers

/*  Use these structures

typedef enum
{
  eSender1,
  eSender2,
}DataSource_t;

typedef struct
{
  uint8_t ucValue;
  DataSource_t eDataSource;
}Data_t;


static const Data_t xStructToSend[2]=
{
  {100,eSender1},
  {50,eSender2}
};
*/

// Create queue handle

// Setup function:

// Create queue

// Create 2 tasks sending each element in the array

// Create task receiving data

// Receiving task must print who wrote to the queue.