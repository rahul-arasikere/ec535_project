/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QUdpSocket>
#include <QFile>

#include "arrowpad.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    arrowPad = new ArrowPad;
    socket = new QUdpSocket(this);
    socket->connectToHost(QHostAddress("192.168.1.36"), 14123); // fix this
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    setCentralWidget(arrowPad);

    connect(arrowPad->forwardButton, SIGNAL(pressed()), this, SLOT(moveForward()));
    connect(arrowPad->backwardButton, SIGNAL(pressed()), this, SLOT(moveBack()));
    connect(arrowPad->leftButton, SIGNAL(pressed()), this, SLOT(moveLeft()));
    connect(arrowPad->rightButton, SIGNAL(pressed()), this, SLOT(moveRight()));
    connect(arrowPad->forwardButton, SIGNAL(released()), this, SLOT(moveStop()));
    connect(arrowPad->backwardButton, SIGNAL(released()), this, SLOT(moveStop()));
    connect(arrowPad->leftButton, SIGNAL(released()), this, SLOT(moveStop()));
    connect(arrowPad->rightButton, SIGNAL(released()), this, SLOT(moveStop()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::moveForward()
{
    // QFile robot("/dev/robot");
    // if(!robot.open(QIODevice::WriteOnly | QIODevice::Text))
    //     return;

    // QTextStream out(&robot);
    // out << "F";

    // robot.close();

    socket->write("F");
}
void MainWindow::moveBack()
{
    // QFile robot("/dev/robot");
    // if(!robot.open(QIODevice::WriteOnly | QIODevice::Text))
    //     return;

    // QTextStream out(&robot);
    // out << "B";

    // robot.close();

    socket->write("B");
}
void MainWindow::moveLeft()
{
    // QFile robot("/dev/robot");
    // if(!robot.open(QIODevice::WriteOnly | QIODevice::Text))
    //     return;

    // QTextStream out(&robot);
    // out << "L";

    // robot.close();

    socket->write("L");
}
void MainWindow::moveRight()
{
    // QFile robot("/dev/robot");
    // if(!robot.open(QIODevice::WriteOnly | QIODevice::Text))
    //     return;

    // QTextStream out(&robot);
    // out << "R";

    // robot.close();

    socket->write("R");
}
void MainWindow::moveStop()
{
    // QFile robot("/dev/robot");
    // if(!robot.open(QIODevice::WriteOnly | QIODevice::Text))
    //     return;

    // QTextStream out(&robot);
    // out << "S";

    // robot.close();

    socket->write("S");
}

void MainWindow::readyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer;
}
