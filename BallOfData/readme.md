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
***Основная идея класса FileThread***
Вход:  
1. Название открываемого бинарного файла,из которого считывается информация.
2. Название файла куда будет записываться информация.
Выход:
Бинарный файл с копией информации  
```
class FileThread
{
public:
	FileThread(std::string InNameFile, std::string OutNameFile);//конструктор
        //1.Инициализирует все счётчики, флаги начальными значениями  
        //2.Открывает файл исходный файл (проверка) , открывает файл записи (проверка)
        //3.Если проверка прошла успешно, то в конструкторе запускаюся 2 потока
        //4.Главный поток в конструкторе будет ожидать заверщения конца работы 2ух доп потоков  
private:

	void main_process();//функция первого потока
        void get_inform();//функция соберает данные для записи скорость записи/размер записи вызывается первыйм потоком   
	void outputInFile();//функция второго потока  

	std::fstream current_file;//объект исходного файла  
	std::fstream out_file;//объект файла записи  

	std::queue<CircularBufferInfo> QueueCircular;//очередь записи  

	int period;  //переменная хранит хранит скорость записи X МБ/с
	int limit_data;//переменная хранит колличество считываемой информации ;  
	int counter_QueueCircular;//переменная хранит колличество заполненых ячеек в очереди  

	bool end;//флаг  
                 //true - при запуске конструктора  
                 //false - при завершении записи  
                 //контролирует цикл в втором потоке записи  
	std::thread actually_thread; //поток1 - ЧТЕНИЕ  
	std::thread output_thread;   //поток2 - ЗАПИСЬ  

};
````
***ПОТОК 1 (заполнение циклического буфера)***   
````
	std::chrono::time_point<std::chrono::high_resolution_clock> starttime;//обявляю переменные которое будут отсчитывать время одного присваивания 
	std::chrono::time_point<std::chrono::high_resolution_clock> endtime;//1 запись в файл = 4 БАЙТА 
	std::chrono::duration<float> duration;//для численного вычисления времени

	CircularBufferInfo Info_Main;//элемент ячейки циклического буфера
	CircularBuffer<int> *CircularBufferPtr = nullptr;//указатель на циклический буффер
	int counterCircle = 0;//счётчик отслеживания предельного колличества записываемых данных


	get_inform();//функция которая считывает входные данные
        //1.limit_data
        //изначально пользователь вводит колличество считываемых МБ
        //но в фунцкии это число преобразуется в максимальное колличество выполнения цикла записи
        //2.period
        //изначально пользователь вводит скорость записи MB/s
        //преобразую выражение так, чтобы в period хранилось время записи для size(int) MB
        //все значения времени в НАНОСЕКУНДАХ
        
	int data;//тут хранятся временные данные
        //!!!нужно избавиться от этой переменной и присваивать напрямую , сильно падает производительность 

	while( counter_QueueCircular != 256 && !current_file.eof() && counterCircle < limit_data)//цикл будет выполняется пока 1.не заполнится очередь
	{                                                                                                                    //2.файл не достигнет конца 
		CircularBufferPtr = new CircularBuffer<int>(MB_INT256);//выделение дин памяти                                //3.количество считываемых данных не привысит ограничения 

		if(CircularBufferPtr == nullptr)//проверка на успех выделенной памяти
		{
			std::cout << "MEMORY ERROR";
			break;
		}



		while( !CircularBufferPtr->isFilled() && counterCircle < limit_data)//цикл выполняется пока 1. не заполнится циклический буффер
			{                                                                                 //2. не достигнул лимит записываемых значений                       
			    starttime = std::chrono::high_resolution_clock::now();//начало отсчёта времени
                            current_file.read((char*) &data, sizeof(int));//запись в файл
                            if(current_file.eof())break;//???

                            CircularBufferPtr->write(data);
                            endtime = std::chrono::high_resolution_clock::now();//конец отсчёта времени
                            duration = endtime - starttime;//вычесление промежутка времени


                            if( duration.count() * GIGA < period)//время сравнивается в наносекундах, если тактовое время на присваиване меньше, чем нужно то ожидаем
				std::this_thread::sleep_for(std::chrono::nanoseconds(int)duration.count() * GIGA - period )));//если нет возможности записывать с необходимой скоростью то задержки                                                                                                                                //не будет 
				counterCircle++;//счётчик 
			}

		Info_Main.size_info_buffer = CircularBufferPtr->getSizeFilled();//запись шаблонной структуры 
		Info_Main.CircularBufferEnd = CircularBufferPtr->getObjectCircular();

		QueueCircular.push(Info_Main);//добавление структуры 
		++counter_QueueCircular;//счётчик заполненых ячеек ++
	}

	end = false;//флаг гарантирующий то, что работа первого потока закончена   
````
***ПОТОК 2 (запись в выходной файл )***
````
void FileThread::outputInFile()
{
	int data;

	std::chrono::time_point<std::chrono::high_resolution_clock> starttime;//аналогично переменные времени 
	std::chrono::time_point<std::chrono::high_resolution_clock> endtime;
	std::chrono::duration<float> duration;

	double speed;//хранит время за которое записывается 1Байт в наносекундах 

	while(end || QueueCircular.size())//выполняется пока 
	{                                 //1.не выполнился основной поток 1
                                          //2.не пустая очередь
		while( QueueCircular.size())
		{
			starttime = std::chrono::high_resolution_clock::now();//начало отсчёта времени

			while(QueueCircular.front().CircularBufferEnd->read(data))//цикл записи
			out_file.write((char *)&data, sizeof(int));//запись

			endtime = std::chrono::high_resolution_clock::now();
			duration = endtime - starttime;
			speed = (duration.count() * GIGA) / (QueueCircular.front().CircularBufferEnd->getSizeFilled() * sizeof(int));
			//время записи 1 байта в наносекундах
			QueueCircular.pop();
		}

		std::cout << "Filled cells: \n" << counter_QueueCircular <<//отчёт
				     "Speed(1B/nS)" << speed;
	}
}
````
