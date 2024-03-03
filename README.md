# English  

📁 BallOfData  
├── 📁 BallOfData  // Main project file  
│   ├── 📄 FileThread.cpp // Implementation of the FileThread class      
│   ├── 📄 FileThread.hpp // Declaration of the FileThread class  
│   ├── 📄 // ADD CMAKE HERE      
│   └── 📄 main.cpp    
└──  📁 DataStructure      
│       └── 📄 CircularBuffer.hpp // Circular buffer definition and implementation   

***Project Assignment***
**General Description:**

    Develop a multithreaded application in a programming language capable of asynchronously writing data to a file.
    The application should have two threads of execution: one for generating data and sending it to a message queue, and another for receiving messages from the queue and writing data to a file.
    Upon reaching a certain volume of written data, the application terminates its operation.
    Functional Requirements:
        One thread of the application allocates 256MB of memory and fills a circular buffer with 1MB of data in the allocated cyclic buffer according to a user-set timer.
        Another thread of the application waits for messages from the message queue and, upon receiving a message, appends data to the file.
        The message queue has a finite length of 256, and upon reaching this length, it issues an error.
        Upon reaching the user-defined volume of written data, the application terminates.

**Main Idea of the FileThread Class**  
Input:

    Name of the binary file from which information is read.
    Name of the file to which information will be written.

Output:
A binary file with a copy of the information.
````
class FileThread
{
public:
    FileThread(std::string InNameFile, std::string OutNameFile); // Constructor
    // 1. Initializes all counters, flags with initial values
    // 2. Opens the source file (checks), opens the output file (checks)
    // 3. If the check is successful, two threads are started in the constructor
    // 4. The main thread in the constructor will wait for the completion of the work of the two auxiliary threads  

private:
    void main_process();    // First thread function
    void get_inform();      // Function collects data for writing - writing speed/size of writing called first thread
    void outputInFile();    // Second thread function  

    std::fstream current_file; // Source file object  
    std::fstream out_file;     // Output file object  

    std::queue<CircularBufferInfo> QueueCircular; // Write queue  

    int period;         // Variable stores writing speed X MB/s
    int limit_data;     // Variable stores the amount of read information;  
    int counter_QueueCircular;  // Variable stores the number of filled cells in the queue  

    bool end;           // Flag
                        // true - at the start of the constructor
                        // false - when writing is completed
                        // controls the loop in the second write thread
    std::thread actually_thread; // Thread 1 - READ  
    std::thread output_thread;   // Thread 2 - WRITE
````

**Thread 1 (Filling the Circular Buffer)**
````
std::chrono::time_point<std::chrono::high_resolution_clock> starttime; // Declaring time variables to measure the time of one assignment
std::chrono::time_point<std::chrono::high_resolution_clock> endtime; // 1 write to file = 4 BYTES
std::chrono::duration<float> duration; // For numerical time calculation

CircularBufferInfo Info_Main; // Element of the circular buffer cell
CircularBuffer<int> *CircularBufferPtr = nullptr; // Pointer to the circular buffer
int counterCircle = 0; // Counter tracking the maximum amount of data being written

get_inform(); // Function that reads input data
// 1. limit_data
// Initially, the user inputs the amount of data to be read in MB
// but in the function, this number is converted into the maximum number of times the write loop is executed
// 2. period
// Initially, the user inputs the write speed in MB/s
// I convert the expression so that period stores the write time for size(int) MB
// all time values are in NANOSECONDS

int data; // Temporary data is stored here
// !!! you need to get rid of this variable and assign directly, performance drops significantly

while (counter_QueueCircular != 256 && !current_file.eof() && counterCircle < limit_data) // Loop executes until 1. the queue is filled
{                                                                                     // 2. the file reaches the end
    CircularBufferPtr = new CircularBuffer<int>(MB_INT256); // dynamic memory allocation                                      // 3. the amount of data read does not exceed the limit

    if (CircularBufferPtr == nullptr) // check for successful memory allocation
    {
        std::cout << "MEMORY ERROR";
        break;
    }

    while (!CircularBufferPtr->isFilled() && counterCircle < limit_data) // Loop executes until 1. the circular buffer is filled
    {                                                                      // 2. the limit of write values is not reached
        starttime = std::chrono::high_resolution_clock::now(); // start time measurement
        current_file.read((char *)&data, sizeof(int)); // write to file
        if (current_file.eof())
            break; // ???

        CircularBufferPtr->write(data);
        endtime = std::chrono::high_resolution_clock::now(); // end time measurement
        duration = endtime - starttime; // calculating the time interval

        if (duration.count() * GIGA < period) // time is compared in nanoseconds, if the clock time for assignment is less than required, wait
            std::this_thread::sleep_for(std::chrono::nanoseconds((int)duration.count() * GIGA - period)); // if it is not possible to write at the required speed, there will be no delays
        counterCircle++; // counter
    }

    Info_Main.size_info_buffer = CircularBufferPtr->getSizeFilled(); // writing template structure
    Info_Main.CircularBufferEnd = CircularBufferPtr->getObjectCircular();

    QueueCircular.push(Info_Main); // adding structure
    ++counter_QueueCircular; // filled cell counter ++
}

end = false; // flag guaranteeing that the first thread's work is finished
````
**Thread 2 (Writing to Output File)**
````
void FileThread::outputInFile()
{
    int data;

    std::chrono::time_point<std::chrono::high_resolution_clock> starttime; // Similarly, time variables
    std::chrono::time_point<std::chrono::high_resolution_clock> endtime;
    std::chrono::duration<float> duration;

    double speed; // Stores the time taken to write 1 byte in nanoseconds

    while (end || QueueCircular.size()) // Executed while
    {                                    // 1. the main thread 1 has not finished execution
                                         // 2. the queue is not empty
        while (QueueCircular.size())
        {
            starttime = std::chrono::high_resolution_clock::now(); // Start time measurement

            while (QueueCircular.front().CircularBufferEnd->read(data)) // Write loop
                out_file.write((char *)&data, sizeof(int)); // Write

            endtime = std::chrono::high_resolution_clock::now();
            duration = endtime - starttime;
            speed = (duration.count() * GIGA) / (QueueCircular.front().CircularBufferEnd->getSizeFilled() * sizeof(int));
            // Time taken to write 1 byte in nanoseconds
            QueueCircular.pop();
        }

        std::cout << "Filled cells: \n"
                  << counter_QueueCircular << // Report
                  "Speed(1B/nS)" << speed;
    }
}

};
````


