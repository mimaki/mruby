mruby-rtos-toppers
===

RTOS (TOPPERS) library for mruby.

## Classes and methods

### RTOS module
|method|description|
|-|-|
|RTOS::delay|Delay the processing of a task for the specified number of milliseconds|
|RTOS::millis|Get system time in milliseconds|

### RTOS::Task class
|method|description|
|-|-|
|Task.new|Create Task object|
|Task#activate|Activate task|
|Task#suspend|Suspend task|
|Task#resume|Resume task|
|Task#terminate|Terminate task|

### RTOS::EventFlag class
|method|description|
|-|-|
|EventFlag.new|Create EventFlag object|
|EventFlag#set|Set event flag signal|
|EventFlag#wait|Wait event flag signal|
|EventFlag#clear|Clear event flag signal|

### RTOS::DataQueue class
|method|description|
|-|-|
|DataQueue.new|Create DataQueue object|
|DataQueue#send|Send message to data queue|
|DataQueue#get|Get message from data queue|

## How to use
Coming soon.

## License
MIT
