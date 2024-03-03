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

};
````


