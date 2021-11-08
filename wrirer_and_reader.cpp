#include <thread>
#include <iostream>
#include <random>
#include <chrono>
#include <mutex>
#include <shared_mutex>

#define COUNT 5
#define WRITING 1
#define READING 1
#define WAITING 0

using namespace std;

const string NAMES[] = {"Socrates","Plato","Aristotle","Democritus","Xenophanes"};
static string BOOK = "Book ->";
mutex readers;
shared_mutex mtx;
static int a = 0;
static int COUNT_OF_READ = 0;

int THREAD_LOCAL_RANDOM_INT(int min, int max){
	thread_local random_device rd;
	thread_local mt19937 rng(rd());
	uniform_int_distribution<int> uni(min,max);
	return uni(rng);
}

class WRITER{
private:
	string NAME;
	int STATUS;
public:
	void SET_WRITER(string NAME, int STATUS){
		this->NAME = NAME;
		this->STATUS = STATUS;
	};
	string GET_NAME(){
		return NAME;
	};
	int GET_STATUS(){
		return STATUS;
	};
	void SET_STATUS(int STATUS){
		this->STATUS = STATUS;
	};
};

class READER{
private:
        string NAME;
        int STATUS;
public:
        void SET_READER(string NAME, int STATUS){
                this->NAME = NAME;
                this->STATUS = STATUS;
 	};
        string GET_NAME(){
                return NAME;
        };
        int GET_STATUS(){
                return STATUS;
        };
        void SET_STATUS(int STATUS){
                this->STATUS = STATUS;
        };
	
};

void WRITE_INFO(WRITER writer) {
	cout << "Writer " << writer.GET_NAME() << " is now avaliable!" <<  endl;
	this_thread::sleep_for(chrono::milliseconds(2000ms));
	while(a < THREAD_LOCAL_RANDOM_INT(3,4)){
		switch (writer.GET_STATUS()) {
			case 1:
				cout << "------> Writer " << writer.GET_NAME() << " want to wititing something" <<  endl;
				mtx.lock();
				cout << "<====== Writer " << writer.GET_NAME() << " start create masterpieces" << endl;
				BOOK = BOOK + "new redaction by " + writer.GET_NAME() + "|";
				this_thread::sleep_for(chrono::milliseconds(THREAD_LOCAL_RANDOM_INT(2000,3000)));
				cout << "------> Writer " << writer.GET_NAME() << " leaves to look for inspiration" << endl;
				a++;
				writer.SET_STATUS(WAITING);
				mtx.unlock();
  				break;
			case 0:
				this_thread::sleep_for(chrono::milliseconds(THREAD_LOCAL_RANDOM_INT(3000,4000)));
				writer.SET_STATUS(WRITING);
  				break;
		default:
  			cout << "EL PROBLEMO" << endl;
  			break;
		}
	}
}

void READ_INFO(READER reader) {
        cout << "Reader " << reader.GET_NAME() << " is now avaliable!" <<  endl;
        this_thread::sleep_for(chrono::milliseconds(2000ms));
        while(COUNT_OF_READ < THREAD_LOCAL_RANDOM_INT(4,10)){
		readers.lock();
		cout << "---> Reader " << reader.GET_NAME() << " want to read info" <<  endl;
		mtx.lock_shared();
		cout << "<=== Reader " << reader.GET_NAME() << " start reading book right now" << endl;
		this_thread::sleep_for(chrono::milliseconds(THREAD_LOCAL_RANDOM_INT(500,1000)));
		cout << "---> Reader " << reader.GET_NAME() << " finish reading book right now" << endl;
		COUNT_OF_READ++;
        	mtx.unlock_shared();
		readers.unlock();
		this_thread::sleep_for(chrono::milliseconds(THREAD_LOCAL_RANDOM_INT(1000,2000)));
	}
	/*if(COUNT_OF_READ < 0){
                cout << "------------------------------------------------" << endl;
                cout << "Finally book redaction history:" << endl;
                cout << BOOK;
        }*/
}

int main() {
	READER READERS[COUNT];
	for(int id = 0; id < COUNT; id++){
		READERS[id].SET_READER(NAMES[id],READING);
	}
	WRITER WRITTER_1, WRITTER_2;
	WRITTER_1.SET_WRITER("Dostoevskiy",WAITING);
	WRITTER_2.SET_WRITER("Tyrgenyev",WAITING);
	thread WRITER_1(WRITE_INFO,WRITTER_1);
	thread WRITER_2(WRITE_INFO,WRITTER_2);
	thread READING_1(READ_INFO,READERS[0]);
	thread READING_2(READ_INFO,READERS[1]);
	thread READING_3(READ_INFO,READERS[2]);
	thread READING_4(READ_INFO,READERS[3]);
	thread READING_5(READ_INFO,READERS[4]);
	WRITER_1.join();
	WRITER_2.join();
	READING_1.join();
	READING_2.join();
	READING_3.join();
	READING_4.join();
	READING_5.join();
        cout << "------------------------------------------------" << endl;
        cout << "Finally book redaction history:" << endl;
        cout << BOOK << endl;
        cout << "------------------------------------------------" << endl;
	cout << "Book writed " << a << " times!" << endl;
	cout << "Book readed " << COUNT_OF_READ << " times";
}
 
