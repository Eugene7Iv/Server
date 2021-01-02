#pragma once

#include <QtWidgets/QWidget>
class QTcpServer;
class QTextEdit;
class QTcpSocket;

class Server : public QWidget
{
	Q_OBJECT

public:
	Server(int nPort, QWidget* pwgt = 0);

private:
	void sendToClient(QTcpSocket* pSocket, const QString& str);

public slots:
	virtual void slotNewConnection();
	void slotReadClient();

private:
	QTcpServer* m_ptcpServer;
	QTextEdit*  m_ptxt;
	quint16     m_nNextBlockSize;
};
