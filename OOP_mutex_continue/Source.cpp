#include <iostream>
#include <thread>
#include <mutex>
#include <string>
// Mutex, deadlock, recursive_mutex
class Logger {
public:
	void log(const std::string& message) {
		std::lock_guard <std::mutex> lock(_mutex);
		std::cout << '[' << _counter << ']' << message << "\n";
		_counter++;
		std::lock_guard<std::mutex> lock2(_secondMutex);
	}

	/*void successMessage() {
		std::lock_guard<std::mutex> lock(_mutex);
		std::cout << "Success message" << std::endl;
	}*/

	void someFunc() {
		std::lock_guard<std::mutex> lock(_mutex);	// если блокировка 2 будет, а внизу первого, то deadlock
		std::cout << "some text\n";
		std::lock_guard<std::mutex> lock2(_secondMutex);
	}

private:
	std::mutex _mutex;
	std::mutex _secondMutex;
	
	int _counter = 0;
};

int main() {

	Logger logger;
	logger.log("Hello");
	logger.log("Info message");

	std::thread t1 = std::thread([&logger]() {
		std::string threadName = "t1";
		for (size_t i = 0; i < 100; i++) {
			logger.log(threadName + " doing " + std::to_string(i) + " iteration of work\n");
			logger.someFunc(); // deadlock
		}
	});

	std::thread t2 = std::thread([&logger]() {
		std::string threadName = "t2";
		for (size_t i = 0; i < 100; i++) {
			logger.log(threadName + " doing " + std::to_string(i) + " iteration of work\n");
			logger.someFunc();	// deadlock
		}
	});

	t1.join();
	t2.join();

}
