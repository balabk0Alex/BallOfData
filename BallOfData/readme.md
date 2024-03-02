# СПРАВКА ПО ПРОЕКТУ    
***Дерево каталогов***    
📁 BallOfData  
├── 📁 BallOfData  //главный файл проект  
│   ├── 📄 FileThread.cpp //реализация класса FileThread      
│   ├── 📄 FileThread.hpp //объявление класса FileThread  
│   ├── 📄 //СЮДА ДОБАВИТЬ CMAKE      
│   └── 📄 main.cpp    
└──  📁 DataStructure      
│       └── 📄 CircularBuffer.hpp //тут определён и реализован циклический буфер   
**Исходное задание**    
Общее описание:
 - Разработать многопоточное приложение на языке программирования, способное асинхронно записывать данные в файл.  
 - Приложение должно иметь две нити выполнения: одна для генерации данных и отправки их в очередь сообщений, другая для приема сообщений из очереди и записи данных в файл.  
 - При достижении определенного объема записанных данных приложение завершает свою работу.  
 - Функциональные требования:  
 - Одна нить приложения выделяет 256МБ памяти и по таймеру, задаваемому пользователем, заполняет счетчиком 1МБ данных в выделенном циклическом буфере.  
 - Другая нить приложения ожидает сообщений из очереди сообщений и по получении сообщения дописывает данные в файл.  
 - Очередь сообщений имеет конечную длину 256 и при достижении этой длины выдает ошибку.  
 - При достижении заданного пользователем объема записанных данных приложение завершает свою работу.
**Основная идея класса FileThread**
```
class FileThread
{
public:
	FileThread(std::string InNameFile, std::string OutNameFile);

private:

	void main_process();
	void outputInFile();
	void get_inform();

	std::fstream current_file;
	std::fstream out_file;

	CircularBufferInfo Info_Main;
	std::queue<CircularBufferInfo> QueueCircular;
	CircularBuffer<int> *CircularBufferPtr;

	int period;
	int limit_data;
	int counterCircle;
	int counter_QueueCircular;

	bool end;

	std::thread actually_thread;
	std::thread output_thread;

};
```` 
