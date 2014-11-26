#include <qthread.h>

class QThread;

class ServerThread : public QThread
{  
    Q_OBJECT  
signals:  
    void sendStatus(const QString &status);  
    void addLog(const QString &log);

private:  
    void run();  
    int clientSocket[10];
    int serverSocket;

public:
    ServerThread(int server_fd);
    virtual ~ServerThread();

public slots:
    void sendAction(char *action);

};  
