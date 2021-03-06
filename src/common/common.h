/***************************************************************************
 *   Copyright (C) 2017 by Jeremy Whiting <jeremypwhiting@gmail.com>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation version 2 of the License.                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <QHash>
#include <QString>
#include <QSettings>
#include <QNetworkRequest>

#include <map>
#include <set>
#include <string>
#include <vector>

#define kSAFEJUMPER_HELPER_LABEL "sh.proxy.SafejumperHelper"

bool IsValidIp(const QString & ip);
bool IsValidPort(const QString & s);

enum vpnState {
    vpnStateDisconnected = 0,
    vpnStateConnecting,
    vpnStateConnected,
    vpnStateTotal
};

QString vpnStateWord(vpnState state);

// Only use commands for start and stop. all settings such as server choice,
// protocol choice, credentials use QSettings.
enum commands {
    cmdGetStatus,
    cmdStart,
    cmdStop,
    cmdKillRunningOpenvpn, // Kill openvpn processes that are running
    cmdSetCredentials, // Set vpn username and password. performed once after connecting from gui to service
    cmdNetdown, // Turn off network devices because of kill switch
    notifyStatusChange,
    notifyStatusWord,
    notifyTimeout, // Openvpn timed out, so switch ports or nodes and try again
    notifyError, // Error message from service
    notifyGotIP, // Got new ip address from openvpn management socket
};

#ifdef Q_OS_WIN
static const QString kSocketName = "SafejumperVPN";
#else
static const QString kSocketName = "/var/tmp/SafejumperVPN";
#endif

enum EncryptionType {
    ENCRYPTION_RSA = 0,
    ENCRYPTION_TOR_OBFS2,
    ENCRYPTION_TOR_OBFS3,
    ENCRYPTION_TOR_SCRAMBLESUIT,
    ENCRYPTION_ECC,
    ENCRYPTION_ECCXOR,
    ENCRYPTION_COUNT
};

const QList<QString> encryptionNames = {
    "RSA 4096-bit",
    "RSA + TOR (obfs2)",
    "RSA + TOR (obfs3)",
    "RSA + TOR (scramblesuit)",
    "ECC (secp384r1)",
    "ECC + XOR",
};


enum OpenVPNStateWord {
    ovnStateConnecting,
    ovnStateTCPConnecting,
    ovnStateWait,
    ovnStateExiting,
    ovnStateReconnecting,
    ovnStateAuth,
    ovnStateGetConfig,
    ovnStateAssignIP,
    ovnStateResolve,
    ovnStateUnknown, // Unknown state word from openvpn
};

#define PORT_FORWARD_MAX 5

bool IsValidIp(const QString & ip);
bool IsValidPort(const QString & s);

bool OpenUrl(const char * url);
bool OpenUrl_Support();
bool OpenUrl_Panel();
bool OpenUrl_Earn();
bool OpenUrl_Bug();
bool launchUpdateUrl();

void SaveCb(const char * name, bool val);

struct AServer {
    QString address;    // IP or DNs
    QString name;       // "Chile Hub" - Hub at the end indicates hub
    QString load;       // double
};

typedef QHash<QString, size_t>  HMSI;

typedef std::map<std::string, int> SIMap;
typedef std::map<int, int> IIMap;

typedef std::vector<unsigned int> UVec;
typedef std::set<unsigned int> USet;

// escape \ and "
QString EscapePsw(const QString & raw);

QNetworkRequest BuildRequest(const QUrl & u);

#endif // COMMON_H
