#include "Client.h"
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>

Client::Client(const QString& strHost, int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt), m_nNextBlockSize(0)
{
	m_pTcpSocket = new QTcpSocket(this);

	m_pTcpSocket->connectToHost(strHost, nPort);
	connect(m_pTcpSocket, &QTcpSocket::connected, this, &Client::slotConnected);
	connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
	connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(slotError(QAbstractSocket::SocketError))
	);

	m_ptxtInfo = new QTextEdit;
	m_ptxtInput = new QLineEdit;

	connect(m_ptxtInput, &QLineEdit::returnPressed, this, &Client::slotSendToServer);
	m_ptxtInfo->setReadOnly(true);

	QPushButton* pcmd = new QPushButton("Send");
	connect(pcmd, &QPushButton::clicked, this, &Client::slotSendToServer);

	//Layout setup
	QVBoxLayout* pvbxLayout = new QVBoxLayout;
	pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
	pvbxLayout->addWidget(m_ptxtInfo);
	pvbxLayout->addWidget(m_ptxtInput);
	pvbxLayout->addWidget(pcmd);
	setLayout(pvbxLayout);
}

void Client::slotReadyRead()
{
	QDataStream in(m_pTcpSocket);
	in.setVersion(QDataStream::Qt_4_2);
	for (;;) {
		if (!m_nNextBlockSize) {
			if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
				break;
			}
			in >> m_nNextBlockSize;
		}

		if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
			break;
		}
		QTime   time;
		QString str;
		in >> time >> str;

		m_ptxtInfo->append(time.toString() + " " + str);
		m_nNextBlockSize = 0;
	}
}

void Client::slotError(QAbstractSocket::SocketError err)
{
	QString strError =
		"Error: " + (err == QAbstractSocket::HostNotFoundError ?
			"The host was not found." :
			err == QAbstractSocket::RemoteHostClosedError ?
			"The remote host is closed." :
			err == QAbstractSocket::ConnectionRefusedError ?
			"The connection was refused." :
			QString(m_pTcpSocket->errorString())
			);
	m_ptxtInfo->append(strError);
}

void Client::slotSendToServer()
{
	QByteArray  arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);
	out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));

	m_pTcpSocket->write(arrBlock);
	m_ptxtInput->setText("");
}

void Client::slotConnected()
{
	m_ptxtInfo->append("Received the connected() signal");
}
