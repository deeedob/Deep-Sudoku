#pragma once
#include <QObject>

class CellData : public QObject
{
    Q_OBJECT
    //! Value of cell
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    //! Status of cell.
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    //! If value of cell is in conflict with another cell.
    Q_PROPERTY(bool conflict READ conflict WRITE setConflict NOTIFY conflictChanged)
    //! If cell is editable
    Q_PROPERTY(bool editable READ editable WRITE setEditable NOTIFY editableChanged)
public:
    CellData(QObject *parent = nullptr);
    CellData(int value, QObject *parent = nullptr);

    int value() const;
    void setValue(int value);
    QString status() const;
    void setStatus(const QString &status);
    bool conflict() const;
    void setConflict(bool b);
    bool editable() const;
    void setEditable(bool b);

signals:
    void valueChanged();
    void statusChanged();
    void conflictChanged();
    void editableChanged();

private:
    int m_value;
    bool m_inConflict;
    bool m_isEditable;
    QString m_status;
};

bool operator ==(const CellData &lhs, const CellData &rhs);
bool operator ==(const CellData &lhs, int value);
