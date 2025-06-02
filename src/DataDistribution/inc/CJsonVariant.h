#ifndef CJSONVARIANT_H
#define CJSONVARIANT_H

#include <DataDistribution_export.h>
#include <QObject>
#include <QJsonDocument>
#include <QJsonValue>

class DATADISTRIBUTION_API CJsonVariant : public QObject
{
    Q_OBJECT
public:
    explicit CJsonVariant(QObject *parent = nullptr);
    CJsonVariant(QString str);
    /**
     * @brief SetJsonString
     * @param jsonstr 要设置的json 字符串
     */
    void SetJsonString(QString jsonstr);
    /**
     * @brief isValid 判断是否键值可用 aaa/bbb/3/ccc 字符串表示属性 数字表示数组索引
     * @param str
     * @return
     */
    bool isValid(QString str);
    /**
     * @brief getValue 获取指定键值的值
     * @param keystr
     * @return
     */
    QJsonValue getValue(QString keystr);
    /**
     * @brief setValue 设置指定键值的值
     * @param keystr
     * @param val
     */
    void setValue(QString keystr, QJsonValue val);
    /**
     * @brief GetJsonString 获取json字符串
     * @return
     */
    QString GetJsonString();
signals:
private:
    QJsonDocument m_document;
};

#endif // CJSONVARIANT_H
