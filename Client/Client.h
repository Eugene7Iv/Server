#pragma once

#include <QtWidgets/QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class Client : public QWidget
{
	Q_OBJECT

public:
	Client(const QString& strHost, int nPort, QWidget* pwgt = 0);

private slots:
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError err);
	void slotSendToServer();
	void slotConnected();

private:
	QTcpSocket* m_pTcpSocket;
	QTextEdit*  m_ptxtInfo;
	QLineEdit*  m_ptxtInput;
	quint16     m_nNextBlockSize;

};
