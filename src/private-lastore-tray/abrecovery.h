#ifndef ABRECOVERY_H
#define ABRECOVERY_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

class ABRecoveryPrivate
{
public:
   ABRecoveryPrivate() = default;

    bool BackingUp;
    qlonglong BackupTime;
    QString BackupVersion;
    bool ConfigValid;
    bool HasBackedUp;
    bool Restoring;

public:
    QMap<QString, QDBusPendingCallWatcher *> m_processingCalls;
    QMap<QString, QList<QVariant>> m_waittingCalls;
};

class ABRecovery: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.ABRecovery"; }

public:
    ABRecovery(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~ABRecovery();

    Q_PROPERTY(bool BackingUp READ backingUp NOTIFY BackingUpChanged)
    inline bool backingUp() const
    { return qvariant_cast<bool>(internalPropGet("BackingUp")); }

    Q_PROPERTY(qlonglong BackupTime READ backupTime NOTIFY BackupTimeChanged)
    inline qlonglong backupTime() const
    { return qvariant_cast<qlonglong>(internalPropGet("BackupTime")); }

    Q_PROPERTY(QString BackupVersion READ backupVersion NOTIFY BackupVersionChanged)
    inline QString backupVersion() const
    { return qvariant_cast<QString>(internalPropGet("BackupVersion")); }

    Q_PROPERTY(bool ConfigValid READ configValid NOTIFY ConfigValidChanged)
    inline bool configValid() const
    { return qvariant_cast<bool>(internalPropGet("ConfigValid")); }

    Q_PROPERTY(bool HasBackedUp READ hasBackedUp NOTIFY HasBackedUpChanged)
    inline bool hasBackedUp() const
    { return qvariant_cast<bool>(internalPropGet("HasBackedUp")); }

    Q_PROPERTY(bool Restoring READ restoring NOTIFY RestoringChanged)
    inline bool restoring() const
    { return qvariant_cast<bool>(internalPropGet("Restoring")); }

public Q_SLOTS:
    inline QDBusPendingReply<bool> CanBackup()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanBackup"), argumentList);
    }

    inline QDBusPendingReply<bool> CanRestore()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanRestore"), argumentList);
    }

    inline QDBusPendingReply<> StartBackup()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("StartBackup"), argumentList);
    }

    inline QDBusPendingReply<> StartRestore()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("StartRestore"), argumentList);
    }

Q_SIGNALS:
    void serviceValidChanged(const bool valid) const;
    void propertyChanged(const QString &propertyName, const QVariant &value);
    void JobEnd(const QString &kind, bool success, const QString &errMsg);
    void BackingUpChanged(bool  value) const;
    void BackupTimeChanged(qlonglong  value) const;
    void BackupVersionChanged(const QString & value) const;
    void ConfigValidChanged(bool  value) const;
    void HasBackedUpChanged(bool  value) const;
    void RestoringChanged(bool  value) const;

public Q_SLOTS:
    void CallQueued(const QString &callName, const QList<QVariant> &args);

private Q_SLOTS:
    void onPendingCallFinished(QDBusPendingCallWatcher *w);
    void onPropertyChanged(const QString& interfaceName,
                           const QVariantMap& changedProperties,
                           const QStringList& invalidatedProperties);

private:
    ABRecoveryPrivate *d_ptr;
};

#endif
