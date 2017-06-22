/*
Copyright (C) 2010 Srivats P.

This file is part of "Ostinato"

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef _PORT_STATS_MODEL_H
#define _PORT_STATS_MODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class QTimer;

typedef enum {
    // Info
    e_INFO_START = 0,

    e_INFO_USER = e_INFO_START,

    e_INFO_END = e_INFO_USER,

    // State
    e_STATE_START,

    e_LINK_STATE = e_STATE_START,
    e_TRANSMIT_STATE,
    e_CAPTURE_STATE,

    e_STATE_END = e_CAPTURE_STATE,

    // Statistics
    e_STATISTICS_START,

    e_STAT_FRAMES_RCVD = e_STATISTICS_START,
    e_STAT_FRAMES_SENT,
    e_STAT_FRAME_SEND_RATE,
    e_STAT_FRAME_RECV_RATE,
    e_STAT_BYTES_RCVD,
    e_STAT_BYTES_SENT,
    e_STAT_BYTE_SEND_RATE,
    e_STAT_BYTE_RECV_RATE,
#if 0
    e_STAT_FRAMES_RCVD_NIC,
    e_STAT_FRAMES_SENT_NIC,
    e_STAT_BYTES_RCVD_NIC,
    e_STAT_BYTES_SENT_NIC,
#endif

    // Rx Errors 
    e_STAT_RX_DROPS,
    e_STAT_RX_ERRORS,
    e_STAT_RX_FIFO_ERRORS,
    e_STAT_RX_FRAME_ERRORS,

    // NetTest
    e_STAT_NT_PKTS,
    e_STAT_NT_BYTES,
    e_STAT_NT_BPS,
    e_STAT_NT_AVG_DELAY,
    e_STAT_NT_MMO_DELAY,
    e_STAT_NT_MAX_DELAY,
    e_STAT_NT_MIN_DELAY,
    e_STAT_NT_AVG_JITTER,
    e_STAT_NT_MMO_JITTER,
    e_STAT_NT_MAX_JITTER,
    e_STAT_NT_MIN_JITTER,
    e_STAT_NT_LOSS_COUNT,
    e_STAT_NT_OUT_WND_COUNT,
    e_STAT_NT_LOSS_KOEFF,
    e_STAT_NT_MMO_LOSS,
    e_STAT_NT_OUT_WND_KOEFF,
    e_STAT_NT_MMO_OUT_WND,


    e_STATISTICS_END = e_STAT_NT_MMO_OUT_WND,

    e_STAT_MAX
} PortStat;

static QStringList PortStatName = (QStringList()
    << QObject::trUtf8("Пользователь")                       //"User"

    << QObject::trUtf8("Состояние линка")                    //"Link State"
    << QObject::trUtf8("Передача потока")                    //"Transmit State"
    << QObject::trUtf8("Захват потока")                      //"Capture State"
    << QObject::trUtf8("Принято кадров")                     //"Frames Received"
    << QObject::trUtf8("Отправлено кадров")                  //"Frames Sent"
    << QObject::trUtf8("Скорость передачи (кадр/с)")         //"Frame Send Rate (fps)"
    << QObject::trUtf8("Скорость приема (кадр/с)")           //"Frame Receive Rate (fps)"
    << QObject::trUtf8("Принято байт")                       //"Bytes Received"
    << QObject::trUtf8("Отправлено байт")                    //"Bytes Sent"
    << QObject::trUtf8("Скорость передачи (байт/с)")         //"Byte Send Rate (Bps)"
    << QObject::trUtf8("Скорость приема (байт/с)")           //"Byte Receive Rate (Bps)"
#if 0
    << "Frames Received (NIC)"
    << "Frames Sent (NIC)"
    << "Bytes Received (NIC)"
    << "Bytes Sent (NIC)"
#endif
    << QObject::trUtf8("Отброшено на приеме")                //"Receive Drops"
    << QObject::trUtf8("Ошибок передачи")                    //"Receive Errors"
    << QObject::trUtf8("Ошибок fifo на приеме")              //"Receive Fifo Errors"
    << QObject::trUtf8("Ошибок в принятых кадрах")           //"Receive Frame Errors"

    << QObject::trUtf8("NetTest: принято пакетов")
    << QObject::trUtf8("NetTest: принято байт")
    << QObject::trUtf8("NetTest: скорость потока (Мбит/с)")
    << QObject::trUtf8("NetTest: ср. задержка (мкс)")
    << QObject::trUtf8("NetTest: MMO ср. задержки (мкс)")
    << QObject::trUtf8("NetTest: макс. задержка (мкс)")
    << QObject::trUtf8("NetTest: мин. задержка (мкс)")
    << QObject::trUtf8("NetTest: ср. вариация задержки (мкс)")
    << QObject::trUtf8("NetTest: инкремент. вариац. задержки (мкс)")
    << QObject::trUtf8("NetTest: макс. вариац. задержки (мкс)")
    << QObject::trUtf8("NetTest: мин. вариац. задержки (мкс)")
    << QObject::trUtf8("NetTest: потеряно пакетов")
    << QObject::trUtf8("NetTest: пакетов за предел. окна")
    << QObject::trUtf8("NetTest: коэфф. потерь (%)")
    << QObject::trUtf8("NetTest: MMO потерь (%)")
    << QObject::trUtf8("NetTest: коэфф. за пред. окна (%)")
    << QObject::trUtf8("NetTest: MMO за пред. окна (%)")
);

static QStringList LinkStateName = (QStringList()
    << QObject::trUtf8("Неизвестно")
    << QObject::trUtf8("Выкл.")
    << QObject::trUtf8("Вкл.")
);

static QStringList BoolStateName = (QStringList()
    << QObject::trUtf8("Выкл.")
    << QObject::trUtf8("Вкл.")
);

class PortGroupList;

class PortStatsModel : public QAbstractTableModel
{
    Q_OBJECT

    public:

        PortStatsModel(PortGroupList *p, QObject *parent = 0);
        ~PortStatsModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, 
            int role = Qt::DisplayRole) const;

        class PortGroupAndPortList {
            public:
            uint portGroupId;
            QList<uint> portList;
        };
        void portListFromIndex(QModelIndexList indices, 
                QList<PortGroupAndPortList> &portList);

    public slots:
        void when_portListChanged();
        //void on_portStatsUpdate(int port, void*stats);
        void when_portGroup_stats_update(quint32 portGroupId);

    private slots:
        void updateStats();

    private:
        PortGroupList    *pgl;

        // numPorts stores the num of ports per portgroup
        // in the same order as the portgroups are index in the pgl
        // Also it stores them as cumulative totals
        QList<quint16>    numPorts;

        QTimer *timer;

        void getDomainIndexes(const QModelIndex &index,
              uint &portGroupIdx, uint &portIdx) const;

};

#endif
