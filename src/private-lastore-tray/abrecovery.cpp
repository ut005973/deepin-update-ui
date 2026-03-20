#include "abrecovery.h"

ABRecovery::ABRecovery(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
    QDBusConnection::systemBus().connect("com.deepin.ABRecovery", "/com/deepin/ABRecovery", "org.freedesktop.DBus.Properties", "PropertiesChanged", this, SLOT(onPropertyChanged(QString, QVariantMap, QStringList)));
}

ABRecovery::~ABRecovery()
{
}

void ABRecovery::onPropertyChanged(const QString& interfaceName,
                                   const QVariantMap& changedProperties,
                                   const QStringList& invalidatedProperties)
{
    if (interfaceName != staticInterfaceName())
        return;

    if (changedProperties.contains("BackingUp")) {
        Q_EMIT BackingUpChanged(backingUp());
    }
    if (changedProperties.contains("BackupTime")) {
        Q_EMIT BackupTimeChanged(backupTime());
    }
    if (changedProperties.contains("BackupVersion")) {
        Q_EMIT BackupVersionChanged(backupVersion());
    }
    if (changedProperties.contains("ConfigValid")) {
        Q_EMIT ConfigValidChanged(configValid());
    }
    if (changedProperties.contains("HasBackedUp")) {
        Q_EMIT HasBackedUpChanged(hasBackedUp());
    }
    if (changedProperties.contains("Restoring")) {
        Q_EMIT RestoringChanged(restoring());
    }

    return;
}

void ABRecovery::CallQueued(const QString &callName, const QList<QVariant> &args)
{
    if (d_ptr->m_waittingCalls.contains(callName))
    {
        d_ptr->m_waittingCalls[callName] = args;
        return;
    }
    if (d_ptr->m_processingCalls.contains(callName))
    {
        d_ptr->m_waittingCalls.insert(callName, args);
    } else {
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(asyncCallWithArgumentList(callName, args));
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &ABRecovery::onPendingCallFinished);
        d_ptr->m_processingCalls.insert(callName, watcher);
    }
}

void ABRecovery::onPendingCallFinished(QDBusPendingCallWatcher *w)
{
    w->deleteLater();
    const auto callName = d_ptr->m_processingCalls.key(w);
    Q_ASSERT(!callName.isEmpty());
    if (callName.isEmpty())
        return;
    d_ptr->m_processingCalls.remove(callName);
    if (!d_ptr->m_waittingCalls.contains(callName))
        return;
    const auto args = d_ptr->m_waittingCalls.take(callName);
    CallQueued(callName, args);
}
